/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <device.h>
#include <VDAC8.h>
#include <stdlib.h>

volatile uint8 cnt = 0;

CY_ISR(Timer_interrupt_handler)
{
    uint16 v, fv;
    
    /* Read Status register in order to clear the sticky Terminal Count (TC) bit 
	 * in the status register. Note that the function is not called, but rather 
	 * the status is read directly.
	 */
   	Timer_STATUS;
    
    cnt++;
    
    v = rand() % UINT16_MAX;
    // Filterをかける前の波形を出力   
    VDAC8_2_SetValue(v >> 8);
    
    Pin_ISR_Check_Write(1u);
    Filter_Write16(Filter_CHANNEL_A, v);
    /* Poll waiting for the holding register to have data to read */
    while (Filter_IsInterruptChannelA() == 0) ;
    fv = Filter_Read16(Filter_CHANNEL_A);
	Pin_ISR_Check_Write(0u);

    VDAC8_1_SetValue(fv >> 8);
}

int main()
{
    /* Start all components used on schematic */
    VDAC8_1_Start();
    VDAC8_2_Start();
    Opamp_1_Start();
    Opamp_2_Start();
    Filter_Start();
    Timer_Start();
    ISR_Timer_StartEx(Timer_interrupt_handler);
    
	/* Enable the interrupt register bit to poll
     Value 1 for Channel A, Value 2 for Channel B */
    Filter_INT_CTRL_REG |= (1 << Filter_CHANNEL_A);
	
    CyGlobalIntEnable;
    
    for(;;)
    {
        //CyDelay(10);
    }
} /* End of main */

/* [] END OF FILE */
