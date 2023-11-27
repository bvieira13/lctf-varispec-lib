import varispec_lcft as filter
from datetime import datetime
import numpy as np

def main():
    lctf = filter.varispec_module_t() 
    
    wavelenght = np.linspace(400,720,33)
    
    lctf.brief()
    lctf.set_mode(4)

    lctf.clear_palette()
    lctf.define_palette(wavelenght)
    
    execution_time = []

    begin = datetime.now()    
    lctf.select_palette_element(0)
    end = datetime.now()

    execution_time.append(end-begin)

    for wave in wavelenght:
        begin = datetime.now()
        lctf.next_palette_pos()
        end = datetime.now()
        execution_time.append(begin-end)
    
    print(execution_time)
    print(np.sum(execution_time))
    
    lctf.disconneted()

if __name__ == "__main__":
    main()