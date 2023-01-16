#include "lib/include.h"

extern void Configurar_UART0(void)
{
 //Experimento 3
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz , utilizando el uart de la practica 3
 * se enviara dato desde interfaz de simulink para controlar la intensidad luminosa
 * usando un led RGB externa
 *
 */
    // HABILITAR E INICIALIZAR EL UART P. 344
    SYSCTL->RCGCUART  = (1<<0);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->deshabilitar 1->habilitar 
    
    // HABILITAR EL RELOJ PARA LE MODULO GPIO p. 1351 
    // Modulo 0 es para puerto A - 1351 GPIO pines de funciones 
    SYSCTL->RCGCGPIO |= (1<<0);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A

    //(GPIOAFSEL) pag.671 PARA FUNCIONES ALTERNATIVAS  EN LOS PINES A1 Y A0 
    GPIOA->AFSEL = (1<<1) | (1<<0); // PARA PIN 0 Y 1

    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    // CADA 4 BITS CORRESPONDEN A UN PIN 
    GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00) | 0x00000011; // (1<<0) | (1<<4);//0x00000011
    // SE PONE UN 1 EN EL BIT O Y 4 -> p.688

    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOA->DEN = (1<<0) | (1<<1);//PA1 PA0 -> habilita los pines como digitales

    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    // se desabilita el uart, la transmisión y la recepción -> para poder configurarlo 
    UART0->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = fckl/(16*baud-rate) = 20,000,000 / (16*57600) = 21.7013
    UARTFBRD[DIVFRAC] = integer(.7013 * 64 + 0.5) = 45.3 -> redondear hacia arriba = 46
    */
    UART0->IBRD = 21;

    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART0->FBRD = 46;

    //  UART Line Control (UARTLCRH) pag.916
    // ox3 porque tiene una longitud de 8 bits -> 1<<4 porque se habilita FIFO 
    UART0->LCRH = (0x3<<5)|(1<<4);

    //  UART Clock Configuration(UARTCC) pag.939
    // SE PONE 0 PARA ESPICIFICAR QUE SE TRABA CON EL RELOJ DEL SISTEMA
    UART0->CC =(0<<0);

    //Disable UART0 UART Control (UARTCTL) pag.918
    // SE HABILITA EL UART 
    UART0->CTL = (1<<0) | (1<<8) | (1<<9);



}

/*extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART0->FR & (1<<4)) != 0 );
    v = UART0->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART0->FR & (1<<5)) != 0 );
    UART0->DR = c;
}
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

extern char * readString(char delimitador)
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;

}
//Experimento 2

//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

*/