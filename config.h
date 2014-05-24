/*
 * Copyright (c) 2014 Steven Stallion
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

#ifndef CONFIG_H
#define CONFIG_H

#define _XTAL_FREQ 48000000UL		/* 48MHz */

/* CONFIG1L */
#pragma config PLLSEL = PLL3X		/* PLL Selection */
#pragma config CFGPLLEN = ON		/* PLL Enable */
#pragma config CPUDIV = NOCLKDIV	/* CPU System Clock Postscaler */
#pragma config LS48MHZ = SYS48X8	/* Low Speed USB Mode */

/* CONFIG1H */
#pragma config FOSC = INTOSCIO		/* Oscillator Selection */
#pragma config PCLKEN = ON		/* Primary Oscillator Shutdown */
#pragma config FCMEN = OFF		/* Fail-Safe Clock Monitor */
#pragma config IESO = OFF		/* Internal/External Oscillator Switchover */

/* CONFIG2L */
#pragma config nPWRTEN = OFF		/* Power-up Timer Enable */
#pragma config BOREN = SBORDIS		/* Brown-out Reset Enable */
#pragma config BORV = 190		/* Brown-out Reset Voltage */
#pragma config nLPBOR = OFF		/* Low-Power Brown-out Reset */

/* CONFIG2H */
#pragma config WDTEN = OFF		/* Watchdog Timer Enable */
#pragma config WDTPS = 32768		/* Watchdog Timer Postscaler */

#endif	/* CONFIG_H */
