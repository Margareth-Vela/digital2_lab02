/* 
 * Archivo:   Lab01_main.c
 * Dispositivo: PIC16F887
 * Autor: Margareth Vela 
 */

#include <xc.h>
#include <stdint.h>
#include "USART.h"

void USART() {   
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 207;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;//configuraciones USART y tasa de baudios
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;           
    return;
}
