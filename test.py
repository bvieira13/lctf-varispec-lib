import varispec_lcft as filter
import time as tim

def main():
    print("Entry in main function")
    lctf = filter.varispec_module_t()
    
    print(lctf.status())
    tim.sleep(10)

    temperature = lctf.get_temperature()

    print("Current wavelenght: {}°C".format(temperature))
    
    
    lctf.disconneted()

if __name__ == "__main__":
    main()