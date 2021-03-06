HadCon2 is a credit-card sized general purpose I/O module for detector and experiment controls as well as for small data acquisition systems.

It is the successor of the discontinued first version HadCon ( HADControl/HadShoPoMo general purpose board, HadCon @ Epics Wiki).

The module has an ATMEL AT90CAN128 microcontroller providing a multitude of connectivity:
I2C (8/4 fold (intern/extern) multiplexer), 6 channel 1-wire master, 8-channel 8bit DAC, galvanically isolated CAN - high-speed transceiver, 8-channel 10-bit SAR ADC, byte-oriented SPI, in total up-to 53 programmable I/O lines and optionally a Lattice MachX02 FPGA for fast data processing tasks.

While the discontinued precursor HadCon had an SoC on-board, its successor HadCon2 has broken up this concept in favour of a more open access:
It doesn't have any CPU on board, but a USB connector to directly allow communication with any type and size of computer (e.g. PC, raspberry PI, dreamplug, ...) having an USB port on one side and at the other end the microcontroller and the FGPA. This communication is based on an ASCII-based protocol in view of easy implementation in detector control systems like e.g. EPICS and LabVIEW.

    Summarizing:
        Microcontroller: ATMEL AT90CAN128
            I2C
            CANbus
            SPI
            ADCs
            ... 
        FPGA: Lattice MachX02-1200-HC
        FTDI USB to serial UART interface
            USB 2.0 connector
            Power over USB 
        I2C devices
            6 × Single-Channel 1-Wire Master
            1 × 8-channel I2C-bus multiplexer with reset
            2 × 4-channel 8-Bit DAC - Digital-to-Analog Converter 
        galvanically isolated CAN - High-speed CAN Transceiver
            optional external power supply 
        2 × Rotary Code Switches, hexadecimal coding
        Reset Button for ATMEL
        11 × LED's, free programmable 

https://wiki.gsi.de/EE/HadCon2
