/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// SCIA -> SCI0 Pinmux
//
//
// SCIRXDA - GPIO Settings
//
#define GPIO_PIN_SCIRXDA 43
#define SCI0_SCIRX_GPIO 43
#define SCI0_SCIRX_PIN_CONFIG GPIO_43_SCIRXDA
//
// SCITXDA - GPIO Settings
//
#define GPIO_PIN_SCITXDA 42
#define SCI0_SCITX_GPIO 42
#define SCI0_SCITX_PIN_CONFIG GPIO_42_SCITXDA

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_SCI0_RX
// ISR need to be defined for the registered interrupts
#define INT_SCI0_RX INT_SCIA_RX
#define INT_SCI0_RX_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP9
extern __interrupt void INT_SCI0_RX_ISR(void);

//*****************************************************************************
//
// SCI Configurations
//
//*****************************************************************************
#define SCI0_BASE SCIA_BASE
#define SCI0_BAUDRATE 115200
#define SCI0_CONFIG_WLEN SCI_CONFIG_WLEN_8
#define SCI0_CONFIG_STOP SCI_CONFIG_STOP_ONE
#define SCI0_CONFIG_PAR SCI_CONFIG_PAR_NONE
#define SCI0_FIFO_TX_LVL SCI_FIFO_TX0
#define SCI0_FIFO_RX_LVL SCI_FIFO_RX1
void SCI0_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	INTERRUPT_init();
void	SCI_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition
