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
 #include "USART.h"

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
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
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
#pragma config LVP = OFF         //Low Voltage Programming Enable bit(RB3/PGM pin 
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
unsigned int a; //Para LCD
  uint8_t POT1; //Para ADC
  uint8_t POT2;
  uint8_t var_temp; //Variable que recibe los datos del USART
  
  uint8_t contador = 0; //Contador de la LCD
  uint8_t cont; //Contador TMR0
  
  uint8_t display_unidad; //Para desplegar el valor de los potenciómetros con
  uint8_t display_decimal;//dos decimales
  uint8_t display_decimal_2;
  uint8_t display_unidad_s2;
  uint8_t display_decimal_s2;
  uint8_t display_decimal_2_s2;
  uint8_t centenas; //Para desplegar el valor del contador 
  uint8_t decenas_temp;
  uint8_t decenas;
  uint8_t unidades;
  uint8_t flag; //Bandera para saber que dato enviar por el USART
  uint8_t flag_1; //Bandera para el ADC

//------------------------------------------------------------------------------
//                          Prototipos
//------------------------------------------------------------------------------
void setup(void);  //Configuración
void LCD_display(void); //Subrutina para la LCD

//------------------------------------------------------------------------------
//                          Código Principal
//------------------------------------------------------------------------------
void main(void) {
    setup(); //Configuración
    while(1){     
     if(cont > 15){ //Se reinicia el contador después de 45ms y se enciende
            cont = 0; //el enable para enviar datos via USART
            TXIE = 1; 
        }
     
    //Conversiones para el display del LCD para los 3 sensores
    display_unidad = POT1 / 51;
    display_decimal = ((POT1 * 100 / 51) - (display_unidad*100))/10;
    display_decimal_2 = ((POT1 * 100 / 51) - (display_unidad*100) - (display_decimal*10));  
    
    display_unidad_s2 = POT2 / 51;
    display_decimal_s2 = (((POT2 * 100) / 51) - (display_unidad_s2*100))/10;
    display_decimal_2_s2 = (((POT2 * 100) / 51) - (display_unidad_s2*100) - (display_decimal_s2*10));
    
    //Para obtener el valor del contador para el display
    centenas = contador/100; //Se divide por 100 para obtener las centenas
    decenas_temp = contador%100;//El residuo se almacena en la variable temporal de decenas
    decenas = decenas_temp/10;//Se divide en 10 el valor de decenas_temp para 
                              //obtener el valor a desplegar en el display   
    unidades = contador%10;//El residuo se almacena en unidades
    
    //Aproximación para los decimales del sensor 1 y 2
    if (display_decimal > 9){
        display_decimal = 9;
    }
    if (display_decimal_2 > 9){
        display_decimal_2 = 9;
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
    if (contador > 5){
        display_unidad = 5;
    }
    
    // Display para Sensor 1
    Lcd_Set_Cursor(2,1);
    Lcd_Write_Char(display_unidad +48);//Se suman 48 ya que tiene que comenzar 
    Lcd_Set_Cursor(2,3);               //en el caracter ascii de 0
    Lcd_Write_Char(display_decimal + 48);
    Lcd_Set_Cursor(2,4);
    Lcd_Write_Char(display_decimal_2 + 48);
    
    //Display para Sensor 2
    Lcd_Set_Cursor(2,7);
    Lcd_Write_Char(display_unidad_s2 +48);
    Lcd_Set_Cursor(2,9);
    Lcd_Write_Char(display_decimal_s2 + 48);
    Lcd_Set_Cursor(2,10);
    Lcd_Write_Char(display_decimal_2_s2 + 48);
     
    //Display para Sensor 3
    Lcd_Set_Cursor(2,13);
    Lcd_Write_Char(centenas +48);
    Lcd_Set_Cursor(2,14);
    Lcd_Write_Char(decenas + 48);
    Lcd_Set_Cursor(2,15);
    Lcd_Write_Char(unidades + 48);
   } 
}

//------------------------------------------------------------------------------
//                          Interrupciones
//------------------------------------------------------------------------------
void __interrupt() isr(void){
    if (INTCONbits.T0IF){           // INTERRUPCION TMR0
        cont++;
        INTCONbits.T0IF = 0;        // Limpiar la bandera de interrupción TMR0
    }
    
    if(ADIF == 1){
        if (flag_1 == 1){ 
            POT1 = ADRESH; //Se almacena el valor del primer potenciómetro
            ADCON0bits.CHS0 = 1;//Cambiar de canal
            flag_1 = 0;
        } else{
            POT2 = ADRESH; //Se almacena el valor del segundo potenciómetro
            ADCON0bits.CHS0 = 0;//Cambiar de canal
            flag_1 = 1;
        }
        
        ADIF = 0; //Limpiar la bandera de ADC
        __delay_us(60);
        ADCON0bits.GO = 1; //Inicia la conversión de ADC
    }  
    
    if(PIR1bits.RCIF == 1){ //Empieza a recibir datos del USART
 
        RA7 = 1;//bandera
        if (RCREG ==  0x0D){
        RA7 = 0;
            if (var_temp == 0x2B){
                contador++; //El contador aumenta de valor
                if (contador > 255){
                    contador = 0;
                } }
            
            else if (var_temp == 0x2D){
                contador--; //El contador disminuye de valor             
                if (contador > 255){
                    contador = 0;
                }
            }
        } 
        else {
        var_temp = RCREG; //Si no se envía el caracter + o -, no realiza 
        }                 //alguna acción
    }
    
    if (TXIF == 1){
        if (flag == 0){//Envía los datos al USART de los dos potenciómetros
            TXREG = display_unidad + 48; //Envía las unidades del POT1
            flag = 1;
        } else if (flag == 1){
            TXREG = 0x2E;
            flag = 2;
        } else if (flag == 2){
            TXREG = display_decimal + 48; //Envía el primer decimal del POT1
            flag = 3;
        } else if (flag == 3){
            TXREG = display_decimal_2 + 48; //Envía el segundo decimal del POT1
            flag = 4;
        } else if (flag == 4){
            TXREG = 0x2D;
            flag = 5;
        }
        else if (flag == 5){
            TXREG = display_unidad_s2 + 48;//Envía las unidades del POT2
            flag = 6;
        } else if (flag == 6){
            TXREG = 0x2E;
            flag = 7;
        } else if (flag == 7){
            TXREG = display_decimal_s2 + 48; //Envía el primer decimal del POT2
            flag = 8;
        } else if (flag == 8){
            TXREG = display_decimal_2_s2 + 48; //Envía el segundo decimal del POT2
            flag = 9;
        } else if (flag == 9){
            TXREG = 0x0D;
            flag = 0;
        }       
    TXIF = 0; //Se limpia la bandera
    }   
}

//------------------------------------------------------------------------------
//                          Subrutinas
//------------------------------------------------------------------------------

void LCD_display(){
    //Muestra los valores iniciales de la pantalla LCD
    Lcd_Set_Cursor(1,3);
    Lcd_Write_String("S1");
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("0.00");
    Lcd_Set_Cursor(1,8);
    Lcd_Write_String("S2");
    Lcd_Set_Cursor(2,7);
    Lcd_Write_String("0.00");
    Lcd_Set_Cursor(1,13);
    Lcd_Write_String("S3");
    Lcd_Set_Cursor(2,13);
    Lcd_Write_String("000");
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
    TRISC = 0x80; //Para salida del display
    TRISD = 0x00; //Para transistores
    TRISE = 0x00; //Para led de alarma y potenciometro
    
    PORTA = 0x00; //Se limpian los puertos
    PORTC = 0x00;    
    PORTD = 0x00;
    PORTE = 0x00;
    
    // Inicializar ADC
    ADC();
    
   //Configurar la interrupcion
    INTCONbits.GIE = 1;  //Enable interrupciones globales
    INTCONbits.T0IE = 1;           
    INTCONbits.T0IF = 0; 
    INTCONbits.PEIE = 1; //Enable interrupciones periféricas
    PIE1bits.ADIE = 1;   //Enable interrupción ADC
    PIR1bits.ADIF = 0;   //Se limpia bandera de interrupción ADC
    
    // Inicializar LCD
    Lcd_Init();
    
    // Inicializar usart
    USART();
    
    //desplegar texto en pantalla
    LCD_display();
    
    //Configurar TMR0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1; //Prescaler 1:256
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = 10;  //Se reinicia el TMR0
}