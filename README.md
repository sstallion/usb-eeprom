# 28Cxxx EEPROM Programmer

This repository contains the firmware and hardware design for a PIC18F-based
USB programmer for the 28Cxxx series of parallel EEPROMs.

This project meets the requirements of the [OSHW Definition][1]. CadSoft EAGLE
design files and an example bill of materials can be found in the [eagle]
directory. For those without access to EAGLE, a PDF of the schematic and board
layout is provided.

PCBs may be ordered from [OSH Park][2]. Fully assembled and tested units may
also be purchased from [Tindie][3].

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
[Getting Started][4] for more details.

To install the `eeprom` tool, issue:

    $ go get github.com/sstallion/go-eeprom/cmd/eeprom

Once installed, issue `eeprom help` to display usage.

## Compatibility

All 5V single supply chips are supported for read and write operations.
Extended functionality, such as page writes and chip erase are supported for
Atmel AT28C and compatible families.

_High-voltage programming is not supported._

## Contributing

Pull requests are welcome! If a problem is encountered using this repository,
please file an issue on [GitHub][5].

## License

Source code in this repository is licensed under a Simplified BSD License with
additional restrictions. See [LICENSE] and [LICENSE-MLA] for more details.

[1]: https://www.oshwa.org/definition/
[2]: http://oshpark.com/shared_projects/PVCsM6xC
[3]: https://www.tindie.com/products/sstallion/28cxxx-eeprom-programmer
[4]: http://golang.org/doc/install
[5]: https://github.com/sstallion/usb-eeprom/issues

[LICENSE]: LICENSE
[LICENSE-MLA]: LICENSE-MLA
[eagle]: eagle
