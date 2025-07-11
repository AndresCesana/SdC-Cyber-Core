#!/usr/bin/env python3
import re
import os
import sys
import pexpect
import random
import time

SOCK_PATH="/tmp/tmp-gpio.sock"
GPIO_RANGE=[0x3f200000, 0x3f200fff]
IC_RANGE  =[0x3f00b200, 0x3f00b3ff]

GPIO_SET_OFFSET=0x1c
GPIO_RESET_OFFSET=0x28
GPIO_READ_OFFSET=0x34

class VGPIOManager(object):
    fd = None
    def __init__(self, spath=SOCK_PATH):
        self.load(spath)

    def load(self, spath=SOCK_PATH):
        if os.path.exists(SOCK_PATH):
            os.unlink(SOCK_PATH)

        self.fd = pexpect.spawn("socat - UNIX-LISTEN:{}".format(SOCK_PATH))

    def validate_address(self, address):
        if address < GPIO_RANGE[0] or address > GPIO_RANGE[1]:
            return False
        return True

    def writel(self, address, value):
        self._sendline('writel 0x{:x} 0x{:x}'.format(address, value))
        return self._read()

    def readl(self, address):
        self._sendline('readl 0x{:x}'.format(address))
        return self._read()

    def read(self, address, size):
        self._sendline('read 0x{:x} 0x{:x}'.format(address, size))
        return self._read()

    def _sendline(self, s):
        return self.fd.sendline(s)

    def _read(self):
        # Cancel echo
        self.fd.readline()
        return self.fd.readline()

    def read_entire_gpio_area(self):
        v = self.read(GPIO_RANGE[0], 0x1000)
        return v


    def close(self):
        self.fd.close()

    def help(self):
        s  = "[ ] Virtual GPIO manager\n"
        s += "    Usage:\n"
        s += "    help                     -- this help message\n"
        s += "    get <gpionum>            -- read a specific gpio\n"
        s += "    set <gpionum> <value>    -- Set a gpio to a specific value\n"
        s += "    toggle <gpionum>         -- change the value of the gpio\n"
        s += "    read-area                -- read entire gpio area\n"
        s += "    read-ic                  -- read entire interrupt controller area\n"
        s += "    readl <address>          -- read 32 bit from address\n"
        s += "    writel <address> <value> -- read 32 bit from address\n"
        s += "    exit                     -- exit from program\n"
        s += "    reload                   -- restart the initialization\n"
        return s

    def read_ic_area(self):
        v = self.read(IC_RANGE[0], 0x200)
        return v

    def get_gpio_location(self, num):
        if num > 54 or num < 0:
            return 0
        return GPIO_RANGE[0] + int(num / 32)

    def set(self, gpionum, value):
        m = self.get_gpio_location(gpionum)
        if value:
            m += GPIO_SET_OFFSET
        else:
            m += GPIO_RESET_OFFSET
        gpio = 1 << (gpionum % 32)
        return self.writel(m, gpio)

    def get(self, gpionum):
        m = self.get_gpio_location(gpionum) + GPIO_READ_OFFSET
        v = int(self.readl(m).split()[1], 0)
        gpio = 1 << (gpionum % 32)

        return str((v & gpio)!=0)

    def toggle(self, gpionum):
        v = self.get(gpionum)
        print("value: {}".format(v))
        return self.set(gpionum, not v)

    def parse(self, s):
        s = s.split(' ')
        c = s[0]

        if c == 'help':
            return self.help()
        elif c == 'get':
            if len(s) < 2:
                return "Error: get requires 1 argument"
            return self.get(int(s[1], 0))
        elif c == 'set':
            if len(s) < 3:
                return "Error: set requires 2 arguments"
            return self.set(int(s[1], 0), int(s[2], 0))
        elif c == 'read-area':
            return self.read_entire_gpio_area()
        elif c == 'read-ic':
            return self.read_ic_area()
        elif c == 'readl':
            if len(s) < 2:
                return "Error: read requires 1 argument"
            return self.readl(int(s[1], 0))
        elif c == 'writel':
            if len(s) < 3:
                return "Error: write requires 2 arguments"
            return self.writel(int(s[1], 0), int(s[2],0))
        elif c == 'exit':
            self.close()
            sys.exit(0)
        elif c == 'toggle':
            if len(s) < 2:
                return "Error: toggle requires 1 argument"
            return self.toggle(int(s[1]))
        elif c == 'reload':
            return self.load()
        return ""

if __name__=="__main__":
    print('[ ] Virtual GPIO manager')
    print('[ ] Listening for connections')
    vgpio = VGPIOManager()
#    while True:
#        cmd = input('(gpio)> ')
#        print(vgpio.parse(cmd))
#    vgpio.close()
    try:
        while True:
            val_17 = random.randint(0, 1)
            val_27 = random.randint(0, 1)

            print(f"[+] GPIO17 = {val_17}, GPIO27 = {val_27}")

            vgpio.set(17, val_17)
            vgpio.set(27, val_27)

            time.sleep(10)
    except KeyboardInterrupt:
        print("\n[*] Cerrando Virtual GPIO manager")
        vgpio.close()
