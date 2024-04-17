import pytest
from heckler.sudo.sudo_attack import SudoAttack

def test_gadget_success():
    gadgets = {
        0x8e69b000 : 1,
        0x8e944000 : 1,
        0x8e9c9000 : 1,
        0x8e943000 : 1,
        0x8e957000 : 2,
        0x8e941000 : 2,
        0x8e942000 : 2,
        0x8e93e000 : 2,
        0x8e844000 : 4,
        0x8e823000 : 4,
        0x8e94e000 : 4,
        0x8e92a000 : 4,
        0x8e93d000 : 4,
        0x8e936000 : 6,
        0x8e937000 : 9,
        0x8e93f000 : 10,
        0x8e843000 : 11,
        0x8e92c000 : 12,
        0x8e83a000 : 14,
        0x12bc6000 : 36276, # this
        0x113ec000 : 36277, # this
        0x8e92e000 : 40852,
        0x8e92d000 : 40853,
        0xecdc000 : 72553 # this
    }
    truth1 = 0x12bc6000
    truth2 = 0x113ec000
    gad1, gad2 = SudoAttack().find_gadgets(gadgets)

    assert truth1 in [gad1, gad2]
    assert truth2 in [gad1, gad2]
    assert truth1 != truth2
    

    assert True


def  test_gadgets_unsorted():
    gadgets = {
        0x19be3001: 533491,
        0xae1d000:   27745, # this
        0x18b82000:  27746, # this
        0x19be4000:  55491, # this
        0x19be4001: 533491,

    }
    
    gad1, gad2 = SudoAttack().find_gadgets(gadgets)
    assert gad1 is not None
    assert gad2 is not None
    assert gad1 != gad2 

    if gad1 is not None:
        print(f'{hex(gad1)}, {hex(gad2)}')
    else:
        print("gadgets not found!")


def  test_no_gadgets():
    gadgets = {
        0x19be3001: 533491,
        0xae1d000:   27740, # this
        0x18b82000:  27746, # this
        0x19be4000:  55491, # this
        0x19be4001: 533491,
    }
    
    gad1, gad2 = SudoAttack().find_gadgets(gadgets)
    assert gad1 is None
    assert gad2 is None



def  test_empty():
    gadgets = {
    
    }
    
    gad1, gad2 = SudoAttack().find_gadgets(gadgets)
    assert gad1 is None
    assert gad2 is None

