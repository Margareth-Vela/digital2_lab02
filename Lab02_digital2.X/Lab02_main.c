/*
 * Archivo:   Lab02_main.c
 * Dispositivo: PIC16F887
 * Autor: Margareth Vela 
 * 
 * Programa: LCD + UART
 * Hardware: Potenciómetros en PORTA, LCD en PORTD, y UART en PORTC
 * 
 * Creado: Julio 27, 2021
 * Última modificación: Julio 28, 2021
 */

//------------------------------------------------------------------------------
//                          Importación de librerías
//------------------------------------------------------------------------------    
 #include <xc.h>
 #include <stdint.h>
 #include "ADC.h"
 #include "LCD.h"

//------------------------------------------------------------------------------
//                          Directivas del compilador
//------------------------------------------------------------------------------
#define _XTAL_FREQ 8000000 //Para delay

//------------------------------------------------------------------------------
//                          Palabras de configuración
//------------------------------------------------------------------------------    
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT//Oscillator Selection bits(INTOSCIO 
                              //oscillator: I/O function on RA6/OSC2/CLKOUT pin, 
                              //I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled and 
                          //can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR  
                                //pin function is digital input, MCLR internally 
                                //tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                //protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
                                //protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                //Internal/External Switchover mode is disabled
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                //(Fail-Safe Clock Monitor is disabled)
#pragma config LVP = ON         //Low Voltage Programming Enable bit(RB3/PGM pin 
                                //has PGM function, low voltage programming 
                                //enabled)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                //Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                //(Write protection off)

//------------------------------------------------------------------------------
//                          Variables
//------------------------------------------------------------------------------
unsigned int a;
  uint8_t POT1;
  uint8_t POT2;
  uint8_t var_temp;
  
  uint8_t flag;
  uint8_t RC_temp;
  
  uint8_t contador;
  uint8_t cont;
  
  uint8_t display_unidad;
  uint8_t display_decimal;
  uint8_t display_decima1_2;
  uint8_t display_unidad_s2;
  uint8_t display_decimal_s2;
  uint8_t display_decimal_2_s2;
  uint8_t display_unidad_s3;
  uint8_t display_decimal_s3;
  uint8_t display_decimal_2_s3;
  uint8_t flag_1;

//------------------------------------------------------------------------------
//                          Prototipos
//------------------------------------------------------------------------------
void setup(void);  //Configuración

//------------------------------------------------------------------------------
//                          Código Principal
//------------------------------------------------------------------------------
void main(void) {
    setup();
    while(1){
        
    //Conversiones para el display del LCD para los 3 sensores
    display_unidad = POT1 / 51;
    display_decimal = ((POT1 * 100 / 51) - (display_unidad*100))/10;
    display_decima1_2 = ((POT1 * 100 / 51) - (display_unidad*100) - (display_decimal*10));  
    
    display_unidad_s2 = POT2 / 51;
    display_decimal_s2 = (((POT2 * 100) / 51) - (display_unidad_s2*100))/10;
    display_decimal_2_s2 = (((POT2 * 100) / 51) - (display_unidad_s2*100) - (display_decimal_s2*10));
      
    display_unidad_s3 = contador ;
    
    //Aproximación para los decimales del sensor 1 y 2
    if (display_decimal > 9){
        display_decimal = 9;
    }
    if (display_decima1_2 > 9){
        display_decima1_2 = 9;
    }
    if (display_decimal_s2 > 9){
        display_decimal_s2 = 9;
    }
    if (display_decimal_2_s2 > 9){
        display_decimal_2_s2 = 9;
    }
    
    if (display_unidad > 5){
        display_unidad = 5;
    }
    if (display_unidad_s2 > 5){
        display_unidad = 5;
    }
    if (display_unidad_s3 > 5){
        display_unidad = 5;
    }
    
    // Display para Sensor 1
    Lcd_Set_Cursor(2,1);
    Lcd_Write_Char(display_unidad +48);
    Lcd_Set_Cursor(2,3);
    Lcd_Write_Char(display_decimal + 48);
    Lcd_Set_Cursor(2,4);
    Lcd_Write_Char(display_decima1_2 + 48);
    
    //Display para Sensor 2
    Lcd_Set_Cursor(2,7);
    Lcd_Write_Char(display_unidad_s2 +48);
    Lcd_Set_Cursor(2,9);
    Lcd_Write_Char(display_decimal_s2 + 48);
    Lcd_Set_Cursor(2,10);
    Lcd_Write_Char(display_decimal_2_s2 + 48);
     
    //Display para Sensor 3
    Lcd_Set_Cursor(2,13);
    Lcd_Write_Char(display_unidad_s3 +48);
    Lcd_Set_Cursor(2,15);
    Lcd_Write_Char(display_decimal_s3 + 48);
    Lcd_Set_Cursor(2,16);
    Lcd_Write_Char(display_decimal_2_s3 + 48);
    return;}
    
}

//------------------------------------------------------------------------------
//                          Interrupciones
//------------------------------------------------------------------------------
void __interrupt() isr(void){

    if(ADIF == 1){
        // bandera para cambiar de canal
        // hacer 1 canal por interrupcion
        if (flag == 1){
            POT1 = ADRESH;
            ADCON0bits.CHS0 = 1;
            flag = 0;
        } else{
            POT2 = ADRESH;
            ADCON0bits.CHS0 = 0;
            flag = 1;
        }
        
        ADIF = 0;
        __delay_us(60);
        ADCON0bits.GO = 1;
    } 
       
    return;
}

//------------------------------------------------------------------------------
//                          Configuración
//------------------------------------------------------------------------------
void setup(void){
    
    //Configuracion reloj
    OSCCONbits.IRCF2 = 1; //Frecuencia a 8MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    
    //Configurar entradas y salidas
    ANSELH = 0x00;//Pines digitales
    ANSEL = 0x03; //Pin analógico para POT
    
    TRISA = 0x03; //Para salida del contador
    TRISC = 0x00; //Para salida del display
    TRISD = 0x00; //Para transistores
    TRISE = 0x00; //Para led de alarma y potenciometro
    
    PORTA = 0x00; //Se limpian los puertos
    PORTC = 0x00;    
    PORTD = 0x00;
    PORTE = 0x00;
    
    //Configurar ADC
    ADCON1bits.ADFM = 0; //Justificar a la izquierda
    ADCON1bits.VCFG0 = 0; //Vss
    ADCON1bits.VCFG1 = 0; //VDD
    
    ADCON0bits.ADCS = 0b10; //ADC oscilador -> Fosc/32
    ADCON0bits.CHS = 0;     //Comenzar en canal 0
    __delay_us(50);        
    ADCON0bits.ADON = 1;    //Habilitar la conversión ADC
    
   //Configurar la interrupcion
    INTCONbits.GIE = 1;  //Enable interrupciones globales
    INTCONbits.T0IE = 1; //Enable TMR0
    INTCONbits.T0IF = 0; //Se limpia la bandera de interrupción TMR0
    INTCONbits.PEIE = 1; //Enable interrupciones periféricas
    PIE1bits.ADIE = 1;   //Enable interrupción ADC
    PIR1bits.ADIF = 0;   //Se limpia bandera de interrupción ADC
    
    // Inicializar LCD
    Lcd_Init();
    return;
}