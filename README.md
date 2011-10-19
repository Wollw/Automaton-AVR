Automaton AVR
=============

About
-----
This program simulates a Celullar Automaton with an AVR MCU connected to:
	(2) 4021 parallel->serial shift registers for reading
		survival rules from dip switches
	(2) 4021 parallel->serial shift registers for reading
		birth rules from dip switches
	(3) 4021 used to read initial cell state from
	(3) 74HC595 for controlling 24 different LEDs

Configuration
-------------
Before compiling there must be a valid 'config.h' file in 'src' directory.
See 'cfg/3x3grid.h' for an example.

Current state of cells is output on each turn on the serial port.  The baud
rate can be changed in 'serial.h'.

Installation
------------
Pick a configuration:
    $ cp cfg/myconfig.h src/config.h

To build:
    $ make

To upload:
    $ make program
