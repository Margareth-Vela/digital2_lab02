/* 
 * Archivo:   Lab02_main.c
 * Dispositivo: PIC16F887
 * Se utilizó y se adaptaron las librerías de Ligo George 
 * de la página www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef lcd_H
#define lcd_H
#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#ifndef RS
#define RS RE0
#endif

#ifndef EN
#define EN RE1
#endif

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
#endif /*lcd_H */