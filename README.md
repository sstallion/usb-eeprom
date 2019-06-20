# 28Cxxx EEPROM Programmer

This repository contains the firmware and hardware design for a PIC18F-based
USB programmer for the 28Cxxx series of parallel EEPROMs. The firmware and
hardware design are licensed under the Simplified BSD License with some
additional restrictions; see `LICENSE` and `LICENSE-MLA` for more detail.

This project meets the requirements of the OSHW Definition. CadSoft EAGLE
design files and an example bill of materials can be found in the `eagle`
subdirectory. For those without access to EAGLE, a PDF of the schematic and
board layout is provided.

PCBs may be ordered from [OSH Park](http://oshpark.com/shared_projects/PVCsM6xC).

Fully assembled and tested units may be purchased from
[Tindie](https://www.tindie.com/products/sstallion/28cxxx-eeprom-programmer).

## Firmware

The firmware requires the Microchip MPLAB XC8 compiler and the MPLAB X IPE
(Integrated Programming Environment) be installed and added to the user's PATH.

To build the firmware, issue:

    $ make all

To program the firmware using an attached MPLAB PICkit 3, issue:

    $ make run

Other programmers may be used; see `Makefile` for more detail.

## Usage

A cross-platform command line tool was developed to drive the programmer,
written in Go. Installing the `eeprom` tool requires Go 1.2 and libusb 1.0; see
[Getting Started](http://golang.org/doc/install) for more detail.

To install the `eeprom` tool, issue:

    $ go get github.com/sstallion/go-eeprom/cmd/eeprom

Once installed, issue `eeprom help` for more detail about its usage.

## Compatibility

All 5V single supply chips are supported for read and write operations.
Extended functionality, such as page writes and chip erase are supported for
Atmel AT28C and compatible families.

_High-voltage programming is not supported._

## Support

Issues should be reported on [GitHub](https://github.com/sstallion/usb-eeprom/issues).
A [wiki](https://github.com/sstallion/usb-eeprom/wiki) is also maintained,
which contains additional documentation.
