#include "lib/include.h"

//configuración UART0, Baud-rate 9600
//PA0->Rx
//PA1->Tx

// Experimento 3
/* Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz , utilizando el uart de la practica 3
 * se enviara dato desde interfaz de simulink para controlar la intensidad luminosa
 * usando un led RGB externa
 *
 */

extern void Configurar_UART0(void)
{
    // HABILITAR E INICIAR EL UART P.344 
    SYSCTL->RCGCUART  = (1<<0); //Paso 1(RCGCUART) P. 344 UART/modulo 0
    //0->deshabilitado
    //1->habilitar

    // HABILITAR EL RELOJ PARA EL MODULO GPIO P. 1351 
    // MODULO 0 ES PARA PUERTO A - P.1351 GPIO PINES DE FUNCIONES 
    SYSCTL->RCGCGPIO |= (1<<0); //habilitar reloj del puerto A (RCGCGPIO) p. 340 
    //UART 0-> PA0 y PA1 p. 1351
    
    // P.671 PARA FUNCIONES ALTERNATIVAS EN LOS PINES A1 Y A0
    GPIOA->AFSEL = (1<<1) | (1<<0); //PARA PIN 0 Y 1, UNO DEBE ENVIAR (TX) Y OTRO DEBE RECIBIR (RX)
    //0->GPIO
    //1->función alternativa

    //CONTROL DEL PUERTO GPIO P.688
    // PA0 -> U0RX
    // PA1 -> U0TX 
    GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00) | 0x00000011; //(1<<0)|(1<<4);//0x00000011
    // SE PONE UN 1 EN EL BIT 0 Y 4 

    // GPIO DIGITAL ENABLE P. 682
    GPIOA->DEN = (1<<0) | (1<<1); // HABILITAR LOS PINES COMO DIGITALES -> PA1 Y PA0
    
    // UART0 UART CONTROL P. 918 DISABLE!!
    UART0->CTL = (0<<9) | (0<<8) | (0<<0); //deshabilitar UART, transmisión y recepción para configurarlo p. 918

    // ******************************************************
    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = fclk/(16*baud-rate) = 20,000,000 / (16*57600) = 21.70
    ******** NO OLVIDAR CAMBIAR LA VELOCIDAD EN SIMULINK POR 57600
    UARTFBRD[DIVFRAC] = integer(.70 * 64 + 0.5) = 45.3 -> REDONDEAR HACIA ARRIBA = 46
    */

    UART0->IBRD = 21; 

    UART0->FBRD = 46;  // UART Fractional Baud-Rate Divisor (UARTFBRD) p. 915
    
    // CONTROL DE LINEA UART P. 916
    // 0X3 PORQUE TIENE UNA LONGITUD DE 8 BITS -> 1<<4 PORQUE SE HABILITA FIFO 
    UART0->LCRH = (0x3<<5)|(1<<4); //  UART Line Control (UARTLCRH) p. 916
    //0x3->longitud de 8 bits
    //4->se habilita el fifo

    // CONFIGURACION DEL RELOJ UART P. 939
    // SE PONE 0 PARA ESPECIFICAR QUE SE TRABAJA CON EL RELOJ DEL SISTEMA 
    UART0->CC =(0<<0); //  UART Clock Configuration (UARTCC) p .939

    // DESHABILITAR UART0 UART CONTROL P. 918
    // SE HABILITA EL UART
    UART0->CTL = (1<<0) | (1<<8) | (1<<9); 
}

// FUNCIÓN PARA RECIBIR UN DATO  
extern char readChar(void) //función para recibir un dato
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART0->FR & (1<<4)) != 0 ); //sabe que hay algo en el buffer, se espera y regresa el dato
    v = UART0->DR & 0xFF;
    c = v;
    return c;
}