/* 
 * Archivo:   Lab01_main.c
 * Dispositivo: PIC16F887
 * Autor: Margareth Vela 
 */

#include <xc.h>
#include <stdint.h>
#include "ADC.h"

//------------------------------------------------------------------------------
//                          Directivas del compilador
//------------------------------------------------------------------------------
#define _XTAL_FREQ 8000000 //Para delay

void ADC() {
        if(ADCON0bits.GO == 0){ //Si la conversión ya está terminada
            if(ADCON0bits.CHS == 0){
                ADCON0bits.CHS = 1;  //pasa al segundo canal  
            }
            else{
                ADCON0bits.CHS = 0;  //pasa al primer canal 
            }
        }
            __delay_us(50); //Delay para el capacitor sample/hold
            ADCON0bits.GO = 1; //Se vuelve a ejecutar la conversión ADC              
    return;
}

