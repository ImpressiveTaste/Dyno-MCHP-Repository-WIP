/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/*
 * File:   sccp.c
 * Author: C68555
 *
 * Created on August 21, 2023, 9:29 AM
 */


#include "xc.h"
#include <stdint.h>
#include <stdbool.h>
#include "sccp.h"


void SCCP1_setup_for_32Bit_Counter()
{
    CCP1CON1bits.CCSEL = 0;         // Input capture mode
    CCP1CON1bits.CLKSEL = 0;        // Set the clock source (1/2 speed UPB bus, 4MHz)
    CCP1CON1bits.T32 = 1;           // 32-bit Single Timer mode
    CCP1CON1bits.MOD = 0;           // Timer Mode, no Output
    CCP1CON1bits.SYNC = 0;          // Timer is self synchronized
    
    CCP1CON2bits.ICS = 0;           // Capture source is ICS pin
    CCP1CON1bits.OPS = 0;           // Interrupt on every input capture event
    
    CCP1CON1bits.TMRPS = 2;         // Set the clock pre-scaler (1:16)
    CCP1CON1bits.ON = 0;            // Enable CCP/input capture

    _CCT1IP = 7;
    _CCT1IF = 0;
    _CCT1IE = 1;
}

void SCCP2_setup_for_32Bit_Counter()
{
    CCP2CON1bits.CCSEL = 0;         // Input capture mode
    CCP2CON1bits.CLKSEL = 0;        // Set the clock source (1/2 speed UPB bus, 4MHz)
    CCP2CON1bits.T32 = 1;           // 32-bit Single Timer mode
    CCP2CON1bits.MOD = 0;           // Timer Mode, no Output
    CCP2CON1bits.SYNC = 0;          // Timer is self synchronized
    
    CCP2CON2bits.ICS = 0;           // Capture source is ICS pin
    CCP2CON1bits.OPS = 0;           // Interrupt on every input capture event
    
    CCP2CON1bits.TMRPS = 2;         // Set the clock pre-scaler (1:16)
    CCP2CON1bits.ON = 0;            // Enable CCP/input capture
}

void SCCP1_Set_Period_32Bit(uint32_t Period)
{
    CCP1PR = Period;
}

void SCCP2_Set_Period_32Bit(uint32_t Period)
{
    CCP2PR = Period;
}

uint32_t SCCP1_Get_Timer_32Bit()
{
    return(CCP1TMR);
}

uint32_t SCCP2_Get_Timer_32Bit()
{
    return(CCP2TMR);
}

void SCCP1_Start()
{
    CCP1CON1bits.ON = 1;            // Enable CCP/input capture
}

void SCCP1_Stop()
{
    CCP1CON1bits.ON = 0;            // Disable CCP/input capture
}

void SCCP2_Start()
{
    CCP2CON1bits.ON = 1;            // Enable CCP/input capture
}

void SCCP2_Stop()
{
    CCP2CON1bits.ON = 0;            // Disable CCP/input capture
}

void SCCP1_Interrupt_Enable()
{
    _CCT1IE = 1;
}

void SCCP1_Interrupt_Disable()
{
    _CCT1IE = 0;
}

void SCCP2_Interrupt_Enable()
{
    _CCT2IE = 1;
}

void SCCP2_Interrupt_Disable()
{
    _CCT2IE = 0;
}

void SCCP1_Timer_Clear()
{
    CCP1TMR = 0;
}

void SCCP2_Timer_Clear()
{
    CCP2TMR = 0;
}








