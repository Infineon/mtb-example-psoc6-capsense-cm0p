/******************************************************************************
* File Name: led.c
*
* Description: This file contains source code that controls LED.
*
* Related Document: README.md
*
*******************************************************************************
 * (c) 2022-2025, Infineon Technologies AG, or an affiliate of Infineon
 * Technologies AG. All rights reserved.
 * This software, associated documentation and materials ("Software") is
 * owned by Infineon Technologies AG or one of its affiliates ("Infineon")
 * and is protected by and subject to worldwide patent protection, worldwide
 * copyright laws, and international treaty provisions. Therefore, you may use
 * this Software only as provided in the license agreement accompanying the
 * software package from which you obtained this Software. If no license
 * agreement applies, then any use, reproduction, modification, translation, or
 * compilation of this Software is prohibited without the express written
 * permission of Infineon.
 *
 * Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
 * IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
 * THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
 * SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
 * Infineon reserves the right to make changes to the Software without notice.
 * You are responsible for properly designing, programming, and testing the
 * functionality and safety of your intended application of the Software, as
 * well as complying with any legal requirements related to its use. Infineon
 * does not guarantee that the Software will be free from intrusion, data theft
 * or loss, or other breaches ("Security Breaches"), and Infineon shall have
 * no liability arising out of any Security Breaches. Unless otherwise
 * explicitly approved by Infineon, the Software may not be used in any
 * application where a failure of the Product or any consequences of the use
 * thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

/*******************************************************************************
* Header files includes
*******************************************************************************/
#include "cybsp.h"
#include "led.h"

/*******************************************************************************
* Global constants
*******************************************************************************/
led_state_t led_state_cur = LED_ON;

/*******************************************************************************
* Function Name: update_led_state
********************************************************************************
* Summary:
*  This function updates the LED state, based on the touch input.
*
* Parameter:
*  led_data: the pointer to the LED data structure
*
* Return
*  none
*
*******************************************************************************/
void update_led_state(led_data_t *led_data)
{
    if ((led_state_cur == LED_OFF) && (led_data->state == LED_ON))
    {
        /* Enable PWM */
        Cy_TCPWM_PWM_Enable(PWM_HW, PWM_NUM);

        #if(CY_IP_MXTCPWM_VERSION > 1)
        {
            /* Start PWM */
            Cy_TCPWM_TriggerStart_Single(PWM_HW, PWM_NUM);
        }
        #else
        {
            /* Start PWM */
            Cy_TCPWM_TriggerStart(PWM_HW, PWM_MASK);
        }
        #endif

        led_state_cur = LED_ON;
        led_data->brightness = LED_MAX_BRIGHTNESS;
    }
    else if ((led_state_cur == LED_ON) && (led_data->state == LED_OFF))
    {
        /* Disable PWM to turn off the LED */
        Cy_TCPWM_PWM_Disable(PWM_HW, PWM_NUM);
        led_state_cur = LED_OFF;
        led_data->brightness = 0;
    }
    else
    {
    }

    if ((LED_ON == led_state_cur) || ((LED_OFF == led_state_cur) && (led_data->brightness > 0)))
    {
        /* Enable PWM */
        Cy_TCPWM_PWM_Enable(PWM_HW, PWM_NUM);

        #if(CY_IP_MXTCPWM_VERSION > 1)
        {
            /* Start PWM */
            Cy_TCPWM_TriggerStart_Single(PWM_HW, PWM_NUM);
        }
        #else
        {
            /* Start PWM */
            Cy_TCPWM_TriggerStart(PWM_HW, PWM_MASK);
        }
        #endif

        uint32_t brightness = (led_data->brightness < LED_MIN_BRIGHTNESS) ? LED_MIN_BRIGHTNESS : led_data->brightness;

        /* Drive the LED with brightness */
        Cy_TCPWM_PWM_SetCompare0(PWM_HW, PWM_NUM, brightness);

        led_state_cur = LED_ON;
    }
  
}

/*******************************************************************************
* Function Name: initialize_led
********************************************************************************
* Summary:
*  Initializes a PWM resource for driving an LED.
*
* Parameters:
*  none
* 
* Return
*  cy_rslt_t rslt - status of operation 
*
*******************************************************************************/
void initialize_led(void)
{
    /* Configure the TCPWM for PWM operation */
    Cy_TCPWM_PWM_Init(PWM_HW, PWM_NUM, &PWM_config);

    /* Enable PWM */
    Cy_TCPWM_PWM_Enable(PWM_HW, PWM_NUM);

    /* Drive the LED with brightness */
    Cy_TCPWM_PWM_SetCompare0(PWM_HW, PWM_NUM, LED_MAX_BRIGHTNESS);

    #if(CY_IP_MXTCPWM_VERSION > 1)
    {
        /* Start PWM */
        Cy_TCPWM_TriggerStart_Single(PWM_HW, PWM_NUM);
    }
    #else
    {
        /* Start PWM */
        Cy_TCPWM_TriggerStart(PWM_HW, PWM_MASK);
    }
    #endif
}

/* [] END OF FILE */
