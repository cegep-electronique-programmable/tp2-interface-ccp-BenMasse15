/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F25K80
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

/*
 Dans ce lab nous allons mesuré la période, le temps haut, la polarité et le flux magnétique 
 d'un emant a l'aide du DRV5057A1.
 */
int Capture; // stock la valeur d'une periode
int Rising_edge; // stock le temps au front montant
int Falling_edge; // stock le temps au front ddescendant
int Uptime; //temps haut total
int Downtime; // temps bas total
int Flag = 0;//pour nous indiquer on est rendu a un rising or falling edge
void CaptureCallback(int CaptureValue); // fonction pour capturer la periode total
void CCP2_ISR (int CaptureValue);
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();
    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
//    CCP2_SetCallBack(CaptureCallback); //when CCP2 detects something go to CallBack
    CCP2_SetCallBack(CCP2_ISR);//interruption a chaque front descendant
    while (1)
    {
        __delay_ms(500);//update l'ecran a chaque 500ms
        vide_ecran();
        if (Uptime < 488){ // si la temps haut est plus petit que la moitie de la periode total la polarite de l'aimant est north
            curseurPosition(0x40);
            printf ("    North Pole"); 
        }
        if (Uptime > 488){ // si la temps haut est plus grand que la moitie de la periode total la polarite de l'aimant est south
            curseurPosition(0x40);
            printf ("    South Pole");   
        }
        curseurPosition(0x00);
        printf ("Period: %fus",Capture*0.5); //
        printf ("+Width: %fus",Uptime*0.5); // il faut multiplier par le temps de 1 tick
    }
}
//void CaptureCallback (int CaptureValue) 
//{
//    Capture = CaptureValue;
//    TMR1_Reload();
//}
void CCP2_ISR (int CaptureValue)
{
    if (Flag == 0){
        Rising_edge = CaptureValue;
        Downtime = Rising_edge - Falling_edge;
        CCP2CON = 0x04; //set pour capturer au falling edge
        Flag = 1;
    }
    else {//front decsendant detecter
        Falling_edge = CaptureValue;
        Uptime = Falling_edge - Rising_edge;
        Capture = Uptime + Downtime;
        CCP2CON = 0x05; //set pour capturer au rising edge
        Flag = 0;
    }
}


/**
 End of File
*/