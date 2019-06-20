# Copyright (C) 2017 Steven Stallion <sstallion@gmail.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

DEVICE = 18F45K50
PROGRAMMER = PK3 # MPLAB PICkit 3

CC = xc8
CFLAGS = -G -Q --CHIP=$(DEVICE) --MODE=free --OPT=all
CPPFLAGS =
LDFLAGS	= --OUTPUT=elf,intel
IPECMD = ipecmd.sh
IPEFLAGS = -P$(DEVICE) -TP$(PROGRAMMER)

HEX = usb-eeprom.hex
PFILES = main.p1 usb_descriptors.p1 usb_device.p1 usb_device_generic.p1

.PHONY:	all clean run verify
.SUFFIXES: .p1

all:	$(HEX)

clean:
	-$(RM) *.d *.p1 *.pre funclist l.obj log.* startup.* usb-eeprom.*

run:	$(HEX)
	$(IPECMD) $(IPEFLAGS) -F$< -M -OL

verify:	$(HEX)
	$(IPECMD) $(IPEFLAGS) -F$< -Y

$(HEX):	$(PFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -O$@ $^

%.p1:	%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) --PASS1 $<
