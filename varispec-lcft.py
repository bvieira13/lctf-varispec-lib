"""
varispec-lctf Module

This module was developed to address demands related to the control of 
VariSpec LCTF equipment manufactured by Cambridge Research & Instrumen-
tation, Inc. The library was based on the development of the LCTF VariSpec 
VIS-20-20;however, the implemented functions cover all models available 
in the market from this manufacturer. 

This file contains the usb_controller_t class to interface between the 
computer and the LCTF electronic controller module, and another varispec_module_t 
to implement the communication protocol outlined in the manufacturer's manual.

Copyright (c) 2023 Bruno Freitas Vieira
"""

import numpy as np
import serial 
import time as tim
from serial.tools.list_ports import comports
from enum import Enum, auto

class usb_controller_t:
    """USB Controller Class

    This class provides methods for communication with a USB device.

    Attributes:
        vid (int): Vendor ID of the USB device.
        pid (int): Product ID of the USB device.
        baudrate (int): Baudrate for serial communication.
        timeout (float): Timeout for serial communication operations (default is 100 milliseconds).
    """
    def __init__(self, vid, pid, baudrate, timeout=100e-3):
        """Initializes tUSB Controller class, serving as the constructor 
        for this class. This function automatically searches for the device 
        based on its Vendor ID (VID) and Product ID (PID) and establishes a 
        connection if a device matching the specified specifications is detected. 
        If no device with the provided characteristics is found, the function 
        returns an error message. 

        Args:
            vid (int): Vendor ID of the USB device.
            pid (int): Product ID of the USB device.
            baudrate (int): Baudrate for serial communication.
            timeout (float): Timeout for serial communication operations (default is 100 milliseconds).
        """
        ports = comports()
        self.__device__ = None
        self.__port__ = None
        for port in sorted(ports):
            if(port.vid == vid and port.pid == pid):
                self.__port__ = port.device
        if(self.__port__ != None):
            try:
                self.__device__ = serial.Serial()
                self.__device__.baudrate = baudrate
                self.__device__.port = self.__port__
                self.__device__.timeout = timeout
                self.__device__.open()
                if (self.__device__.is_open):
                    self.__device__.reset_input_buffer()
                    self.__device__.reset_output_buffer()
            except serial.SerialException as err:
                print(f"Error opening serial port: {err}")
        else:
            print("Device [VID:PID={}:{}] is not found".format(hex(vid), hex(pid)))
      
    def send_data(self, data):
        try:
            self.__device__.write(data)
        except serial.SerialException as err:
            print(f"Error when try sending data: {err}")
    def receive_data(self) -> np.uint8:
        timeout = tim.time() + 500e-3
        timeout_flag = False
        while self.__device__.in_waiting == 0:
            if timeout <= tim.time():
                timeout_flag = True
                break
            pass
        
        data_len = self.__device__.in_waiting
        data_vec = []
        if not timeout_flag:
            while data_len != 0:
                 temp = int.from_bytes(self.__device__.read(),'big')
                 data_vec.append(temp)
                 data_len-=1    
        else:
            print("Receive data operation timed out")
        
        return data_vec
    
    def get_port_str(self):
        return self.__device__.port
    
    def close(self) -> None:
        try:
            if self.__device__ is not None:
                self.__device__.close()
        except serial.SerialException as err:
            print(f"Error closing serial port: {err}")      

class varispec_error_t(Enum):
    NO_ERROR = 0
    NOT_OPEN = 100
    BAD_HANDLE = auto()
    NOT_INITIALIZED = auto()
    ALREADY_OPEN = auto()
    INTERFACE_NOT_AVAILABLE = auto()
    COMMUNICATION_ERROR = auto()
    NO_DEVICE_PRESENT = auto()
    COMMUNICATION_SEND_ERROR = auto()
    COMMUNICATION_REPLY_ERROR = auto()
    COMMUNICATION_GENERAL_ERROR = auto()
    BLOCK_SEND_ERROR = auto()
    BLOCK_RECEIVE_ERROR = auto()
    NO_READY_REPLY = auto()
    COMMAND_TOO_LONG = auto()
    SET_PAR_FAILED = auto()
    GET_PAR_FAILED = auto()
    DEVICE_BUSY = auto()
    NO_CHARACTER_ECHO = auto()
    SET_BLOCK_TIMEOUT_FAILED = auto()
    SET_FLOAT_FAILED = auto()
    INITIALIZATION_FAILED = auto()
    GETERRORFAILED = auto()
    VERSION_READBACK_ERROR = auto()
    CONFIG_READBACK_ERROR = auto()
    ATTEMPT_TO_SET_RO_PARAMETER = auto()
    NULL_RETURN_POINTER = auto()
    INTERNAL_RANGE_ERROR = auto()
    TOO_MANY_DEVICES_OPEN = auto()

    NO_WAVEPLATES_PRESENT =256

    INT_OBJECT_CREATION_FAILED =1000
    INT_WAVEPLATESTAGESINVALID =auto()


class varispec_module_t:
    def __init__(self):
        self.__vid__ = 0x0403
        self.__pid__ = 0x6001
        self.__baudrate__ = 115200 # can be 9600, but needs switch in eletronic module
        self.__controller__ = usb_controller_t(self.__vid__,self.__pid__,self.__baudrate__)
        port_name = self.__controller__.get_port_str()
        
        self.__brief_mode__ = self.query_brief()
        self.__wavelenght__ = self.query_pallet()
        self.__current_wavelenght__ : float     = self.query_wavelenght()
        self.__current_palette_pos__: np.uint8  = self.query_palette_pos()
        self.__wavelenght_step__    : float     = self.query_jump()

        self.__fw_rev__, self.__min_wave__, self.__max_wave__, self.__ser_num__ = self.__get_version__()
        
        print("LCTF VariSpec VIS-07-HC-20 is connected in serial port: {}".format(port_name))
        print("Firmware: {}\t Wavelenght interval: {}-{} nm\t Serial number: {}"
              .format(self.__fw_rev__,self.__min_wave__,self.__max_wave__,self.__ser_num__))
    
    def awake(self) -> bool:
        opcode = 'A'
        ser_num_str = str(self.__ser_num__)
        is_data_ok = self.__check__(opcode,ser_num_str)
        if(is_data_ok):
            return True
        return False
    
# This command just works for query fuctions
    def brief(self,mode=1) -> bool:
        self.__brief_mode__ = mode
        opcode = 'B'
        arg = str(mode)
        is_data_ok = self.__check__(opcode,arg,arg)
        if(is_data_ok):
            return True
        return False
    
    def query_brief(self) -> np.uint8:
        opcode = 'B'
        mode = {'b     0\r' :0,
                '1\r'       :1,
                'b     2\r' :2}
        
        self.__brief_mode__ = self.__query__(opcode,mode)

        if(self.__brief_mode__ == None):
            print("Error to return brief command status")
            return None
        return self.__brief_mode__
    
    def clear_palette(self) -> bool:
        opcode = 'C'
        arg = '1'
        is_data_ok = self.__check__(opcode,arg)
        if(is_data_ok):
            return True
        return False
    
    def query_clr_palette(self) -> bool:
        opcode = 'C'
        status = {'c     0\r':0}
        query_rsp = self.__query__(opcode,status)
        if(query_rsp != None):
            return True
        return False
    
    def define_palette(self,wavelenghts:float) -> bool:
        opcode = 'D'
        for wavelenght in wavelenghts:
            wv_str = str(wavelenght)
            cmd = self.__mount_command__(opcode,wv_str)
            self.__controller__.send_data(cmd)
        clear_buffer = self.__controller__.receive_data()
        if clear_buffer != '':
            return True
        return False
    
    def query_pallet(self) -> float:
        brief_mode = self.__brief_mode__ if self.__brief_mode__ != 2 else 0
        opcode = 'D'
        response = self.__send_receive__(opcode,'?')
        split_data = response.split('\r')
        if brief_mode == 1:
            size = int(split_data[1])
        else:
            temp = split_data[1].split(5*' ')
            size = int(temp[1])
        wavelenghts:float = []
        for i in range(size):
            temp = split_data[i+2].split((2-brief_mode)*' ')
            wavelenghts.append(float(temp[1 - brief_mode]))
        self.__wavelenght__ = wavelenghts
        return wavelenghts
    
    def exercise(self,count:np.uint8) -> bool:
        opcode = 'E'
        arg = str(count)
        response = self.__send_receive__(opcode,arg)
        cmd = opcode + arg + '\r'
        if(response != cmd):
            return False
        
        while self.is_busy():
            pass
        response = self.__controller__.__device__.read_all()
        return True
    
    def set_trigger(self, pulses:np.uint8) -> bool:
        opcode = 'G'
        arg = str(pulses)
        is_data_ok = self.__check__(opcode,arg,arg)
        if(is_data_ok):
            return True
        return False
    
    def initialize(self) -> bool:
        opcode = 'I'
        arg = '1'
        response = self.__send_receive__(opcode,arg)
        cmd = opcode + arg + '\r'
        if(response != cmd):
            return False
        
        while self.is_busy():
            pass
        response = self.__controller__.__device__.read_all()
        return True
        
    def set_jump_step(self,step:float) -> bool:
        opcode ='J'
        arg = str(step)
        is_data_ok = self.__check__(opcode,arg,arg)
        if(is_data_ok):
            self.__wavelenght_step__ = step
            return True
        return False

    def query_jump(self) -> float:
        brief_mode = self.__brief_mode__ if self.__brief_mode__ != 2 else 0
        opcode ='J'
        response = self.__send_receive__(opcode,'?')
        split_data = response.split('\r')
        if brief_mode == 1:
            step = float(split_data[1])
        else:
            temp = split_data[1].split('J')
            step = float(temp[1])
        return step

    def set_mode(self,mode=0) -> bool:
        self.__mode__ = mode
        opcode = 'M'
        arg = str(mode)
        is_data_ok = self.__check__(opcode,arg,arg)
        if(is_data_ok):
            return True
        return False
    
    def query_mode(self) -> np.uint8:
        brief_mode = self.__brief_mode__ if self.__brief_mode__ != 2 else 0
        opcode ='M'
        response = self.__send_receive__(opcode,'?')
        split_data = response.split('\r')
        if brief_mode == 1:
            mode = np.uint8(split_data[1])
        else:
            temp = split_data[1].split('m')
            mode = np.uint8(temp[1])
        return mode
    
    def select_palette_element(self,element) -> bool:
        index = element
        if element > 255:
            index = self.__wavelenght__.index(element)
        opcode ='P'
        arg = str(index)
        is_data_ok = self.__check__(opcode,arg,arg)
        if(is_data_ok):
            self.__current_palette_pos__ = index
            return True
        return False

    def next_palette_pos(self) -> bool:
        opcode ='P'
        arg = '>'
        data = str(self.__current_palette_pos__ + 1)
        is_data_ok = self.__check__(opcode,arg,data)
        if(is_data_ok):
            return True
        return False
    
    def previous_palette_pos(self) -> bool:
        opcode ='P'
        arg = '<'
        data = str(self.__current_palette_pos__ - 1)
        is_data_ok = self.__check__(opcode,arg,data)
        if(is_data_ok):
            return True
        return False
    
    def query_palette_pos(self) -> np.uint8:
        brief_mode = self.__brief_mode__ if self.__brief_mode__ != 2 else 0
        opcode ='P'
        response = self.__send_receive__(opcode,'?')
        split_data = response.split('\r')
        if brief_mode == 1:
            pos = np.uint8(split_data[1])
        else:
            temp = split_data[1].split('p')
            pos = np.uint8(temp[1])
        return pos

    def set_wavelenght(self,wavelenght:float) -> bool:
        if wavelenght <= self.__min_wave__ or wavelenght >= self.__max_wave__:
            print("Select wavelenght {} nm is not in filter interval {} a {} nm"
                  .format(wavelenght,self.__min_wave__, self.__max_wave__))
            return False
        opcode ='W'
        arg = str(wavelenght)
        is_data_ok = self.__check__(opcode,arg,arg)
        if(is_data_ok):
            self.__current_wavelenght__ = wavelenght
            return True
        return False

    def next_waveleght(self) -> bool:
        opcode ='W'
        arg = '>'
        data = str(self.__current_wavelenght__ + self.__wavelenght_step__)
        is_data_ok = self.__check__(opcode,arg,data)
        if(is_data_ok):
            return True
        return False
    
    def previous_wavelenght(self) -> bool:
        opcode ='W'
        arg = '<'
        data = str(self.__current_wavelenght__ - self.__wavelenght_step__)
        is_data_ok = self.__check__(opcode,arg,data)
        if(is_data_ok):
            return True
        return False
    
    def query_wavelenght(self) -> float:
        brief_mode = self.__brief_mode__ if self.__brief_mode__ != 2 else 0
        opcode ='W'
        response = self.__send_receive__(opcode,'?')
        split_data = response.split('\r')
        if brief_mode == 1:
            wavelenght = float(split_data[1])
        else:
            temp = split_data[1].split('W')
            wavelenght = float(temp[1])
        return wavelenght

    def sleep(self) -> bool:
        opcode = 'S'
        ser_num_str = str(self.__ser_num__)
        is_data_ok = self.__check__(opcode,ser_num_str)
        if(is_data_ok):
            return True
        return False
    
    def get_temperature(self) -> float:
        brief_mode = self.__brief_mode__ if self.__brief_mode__ != 2 else 0
        opcode ='Y'
        response = self.__send_receive__(opcode,'?')
        split_data = response.split('\r')
        if brief_mode == 1:
            temperature = float(split_data[1])
        else:
            temp = split_data[1].split('Y')
            temperature = float(temp[1])
        return temperature
    
    def is_busy(self) -> bool:
        opcode = b'!'
        self.__controller__.send_data(opcode)
        response = self.__controller__.receive_data()
        resp_str = ''.join(chr(item) for item in response)
        if resp_str[1] == '>':
            return False
        return True
    
    def clear_error(self) -> bool:
        self.__escape__()
        if self.__reset__('1'):
            return True
        return False

    def execute(self) -> bool:
        opcode = 'E'
        arg = '1'
        response = self.__send_receive__(opcode,arg)
        cmd = opcode + arg + '\r'
        if(response != cmd):
            return False
        response = self.__controller__.__device__.read_all()
        return True
    
    def disconneted(self) -> None:
        self.__controller__.close()
        print("LCTF VariSpec VIS-07-HC-20 is disconnect from PC")

    def __get_version__(self):
            brief_mode = self.__brief_mode__ if self.__brief_mode__ != 2 else 0
            opcode ='V'
            response = self.__send_receive__(opcode,'?')
            split_cmd_from_data = response.split('\r')
            data = split_cmd_from_data[1].split(' ')
            if brief_mode == 1:
                fw_version = '.'.join(char for char in data[0])
                min_wavelenght = float(data[1])
                max_wavelenght = float(data[2])
                serial_num = int(data[3])
            else:
                fw_version = '.'.join(char for char in data[3])
                min_wavelenght = float(data[5])
                max_wavelenght = float(data[7])
                serial_num = int(data[8])
                
            return fw_version, min_wavelenght, max_wavelenght, serial_num

    def __reset__(self,arg:str):
        opcode ='R'
        is_data_ok = self.__check__(opcode,arg)
        if(is_data_ok):
            return True
        return False

    def __escape__(self) -> bool:
        opcode = b'\x1b' # Esc symbol 
        self.__controller__.send_data(opcode)
        response = self.__controller__.receive_data()

    def __status__(self) -> np.uint8:
        opcode = b'\x40' # @ 
        self.__controller__.send_data(opcode)
        response = self.__controller__.receive_data()
        return response[1]

    def __mount_command__(self,opcode:str,data:str = None) -> bytes:
        term = '\r'
        cmd = opcode + data + term
        return cmd.encode('utf-8')

    def __send_receive__(self, opcode, arg:str) -> str:
        cmd = self.__mount_command__(opcode,arg)
        self.__controller__.send_data(cmd)
        response = self.__controller__.receive_data()
        resp_str = ''.join(chr(item) for item in response)
        return resp_str
    
    def __check__(self, opcode, arg:str, confirm:str = '0') -> bool:
        cmd = self.__mount_command__(opcode,arg)
        response = self.__send_receive__(opcode,arg)
        expected_resp:str = cmd.decode('utf-8') 
        if(self.__brief_mode__ != 2):
            if(response != expected_resp):
                return False
        else:
            expected_resp += expected_resp[0].lower() + 5*' ' + confirm + '\r'
            if(response != expected_resp):
                return False
        return True
    
    def __query__(self, opcode, table:dict):
        response = self.__send_receive__(opcode,'?')
        return table[response[3:]]

def main():
    print("Entry in main function")
    lctf = varispec_module_t()
    
    print(lctf.status())
    tim.sleep(10)

    temperature = lctf.get_temperature()

    print("Current wavelenght: {}°C".format(temperature))
    
    
    lctf.disconneted()

if __name__ == "__main__":
    main()