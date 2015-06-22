/*
 * Copyright (c) 2015 Steven Stallion
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_generic.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define NELEM(x) (sizeof (x) / sizeof *(x))

#define ADDRA(x) ((x) & 0xFF)
#define ADDRB(x) ((x)>>8 & 0x1F)
#define ADDRC(x) ((x)>>13 & 0x3)

#define nCE (1<<0)	/* Chip Enable */
#define nOE (1<<1)	/* Output Enable */
#define nWE (1<<2)	/* Write Enable */

uint8_t in[USBGEN_EP_SIZE] @0x500;
uint8_t out[USBGEN_EP_SIZE] @0x540;

enum {
	STATE_CMD,
	STATE_DATA,
	STATE_STATUS,
} state;

void
ClockInit(void)
{
	/*
	 * Configure the system clock to run at 48MHz via the high-frequency
	 * internal oscillator. This provides sufficient performance and
	 * stability without requiring an external clock source.
	 */
	OSCTUNE = SPLLMULT;	/* 3xPLL */
	OSCCON = 0x70;		/* HFINTOSC (16MHz) */
	OSCCON2 = PLLEN;	/* PLL Enable */

	/* Wait for system clock to settle */
	while (!(OSCCON2 & PLLRDY))
		;

	/* Enable Active Clock Tuning for USB */
	ACTCON = ACTEN | ACTSRC;
}

void
EEPROMInit(void)
{
	/*
	 * Drive outputs low, except for !OE and !WE. A pull-up resistor on !CE
	 * ensures the chip remains disabled until the corresponding output
	 * driver is enabled.
	 */
	LATA = 0;
	LATB = 0;
	LATC = 0;
	LATD = 0;
	LATE = nOE | nWE;

	/* Disable analog inputs */
	ANSELA = 0;
	ANSELB = 0;
	ANSELC = 0;
	ANSELD = 0;
	ANSELE = 0;

	/* Enable output drivers */
	TRISA = 0;
	TRISB = 0;
	TRISC = 0;
	TRISD = 0;
	TRISE = 0;

	/*
	 * Some chips (notably the AT28C and CAT28C families) employ a power on
	 * delay to protect against inadvertent writes.
	 */
	__delay_ms(10);
}

void
EEPROMPoll(uint8_t data)
{
	TRISD = ~0;
	LATE = nWE;
	while (PORTD != data)
		;
	LATE = nOE | nWE;
}

void
EEPROMRead(uint16_t addr, uint8_t *buf, uint16_t len)
{
	TRISD = ~0;
	do {
		LATC = ADDRC(addr);
		LATB = ADDRB(addr);
		LATA = ADDRA(addr);
		LATE = nWE;
		*buf++ = PORTD;
		LATE = nOE | nWE;
	} while (++addr, --len);
}

void
EEPROMWrite(uint16_t addr, uint8_t *buf, uint16_t len)
{
	do {
		TRISD = 0;
		LATC = ADDRC(addr);
		LATB = ADDRB(addr);
		LATA = ADDRA(addr);
		LATD = *buf++;
		LATE = nOE;
		LATE = nOE | nWE;
		EEPROMPoll(buf[-1]);
	} while (++addr, --len);
}

void
EEPROMWritePage(uint16_t addr, uint8_t *buf, uint16_t len)
{
	TRISD = 0;
	LATC = ADDRC(addr);
	LATB = ADDRB(addr);
	do {
		LATA = ADDRA(addr);
		LATD = *buf++;
		LATE = nOE;
		LATE = nOE | nWE;
	} while (++addr, --len);
	EEPROMPoll(buf[-1]);
}

void
EEPROMErase(void)
{
	static uint16_t addr[] = {
		0x5555, 0x2AAA, 0x5555, 0x5555, 0x2AAA, 0x5555,
	};
	static uint8_t data[] = {
		0xAA, 0x55, 0x80, 0xAA, 0x55, 0x10,
	};

	TRISD = 0;
	for (uint8_t i = 0; i < NELEM(addr); ++i) {
		LATC = ADDRC(addr[i]);
		LATB = ADDRB(addr[i]);
		LATA = ADDRA(addr[i]);
		LATD = data[i];
		LATE = nOE;
		LATE = nOE | nWE;
	}
	__delay_ms(10);
	__delay_ms(10);
}

bool
USBEventHandler(USB_EVENT event, void *pdata, uint16_t size)
{
	switch (event) {
	case EVENT_CONFIGURED:
		USBEnableEndpoint(USBGEN_EP_NUM, USB_HANDSHAKE_ENABLED |
		    USB_OUT_ENABLED | USB_IN_ENABLED | USB_DISALLOW_SETUP);
		state = STATE_CMD;
		break;

	case EVENT_EP0_REQUEST:
		USBCheckVendorRequest();
		break;

	case EVENT_RESET:
		if (USBGetDeviceState() == CONFIGURED_STATE) {
			RESET();
		}
		break;
	}
	return true;
}

void interrupt
Interrupt(void)
{
	USBDeviceTasks();
}

void
main(void)
{
	USB_HANDLE h;
	uint8_t cmd;
	uint16_t addr, n;
	uint24_t len;

	ClockInit();
	EEPROMInit();
	USBDeviceInit();
	USBDeviceAttach();

	for (;;) {
		if (USBGetDeviceState() < CONFIGURED_STATE ||
		    USBIsDeviceSuspended())
			continue;

		switch (state) {
		case STATE_CMD:
			h = USBGenRead(USBGEN_EP_NUM, out, sizeof out);
			while (USBHandleBusy(h))
				;
			switch (cmd = *out) {
			case 'R': /* Read */
			case 'W': /* Byte Write */
			case 'P': /* Page Write */
				memcpy(&addr, out+1, sizeof addr);
				memcpy(&n, out+3, sizeof n);
				len = (uint24_t)n + 1;
				state = STATE_DATA;
				break;
			case 'Z': /* Chip Erase */
				addr = 0;
				len = 0;
				EEPROMErase();
				state = STATE_STATUS;
				break;
			}
			break;

		case STATE_DATA:
			switch (cmd) {
			case 'R': /* Read */
				n = MIN(len, sizeof in);
				EEPROMRead(addr, in, n);
				h = USBGenWrite(USBGEN_EP_NUM, in, n);
				while (USBHandleBusy(h))
					;
				break;
			case 'W': /* Byte Write */
			case 'P': /* Page Write */
				h = USBGenRead(USBGEN_EP_NUM, out, sizeof out);
				while (USBHandleBusy(h))
					;
				n = USBHandleGetLength(h);
				if (cmd == 'W')
					EEPROMWrite(addr, out, n);
				else
					EEPROMWritePage(addr, out, n);
				break;
			}
			addr += n;
			len -= n;
			if (len == 0)
				state = STATE_STATUS;
			break;

		case STATE_STATUS:
			memcpy(in, &addr, sizeof addr);
			h = USBGenWrite(USBGEN_EP_NUM, in, sizeof addr);
			while (USBHandleBusy(h))
				;
			state = STATE_CMD;
			break;
		}
	}
}
