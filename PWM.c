
#include "lib/include.h"

  // Experimento 3
/* Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz , utilizando el uart de la practica 3
 * se enviara dato desde interfaz de simulink para controlar la intensidad luminosa
 * usando un led RGB externa
 *
 */

//modulo 0, generador 0, 1 y 2
//M0PWM1->PB7 (generador 0) p. 1233
//M0PWM2->PB4 (generador 1)
//M0PWM4->PE4 (generador 2)

extern void Configura_Reg_PWM1(uint16_t freq)
{
    // ES ESTA TRAJANDO CON EL PB4 (PW2), PB7(PWM1), PE4(PWM4) Y MODULO 0
    // SE HABILITA MODULO PWM0 P. 354
    SYSCTL->RCGCPWM |= (1<<0);
    
    // HABILITA GPIO PUERTO B Y E PORQUE SE TRABAJA CON GEN. 1 MODULO 0, P. 340 Y 1233 PARA SABER 
    // QUE PUERTOS/PINES CORRESPONDEN A CADA PWM
    SYSCTL->RCGCGPIO |= (1<<1)|(1<<4); //habilitar reloj de GPIO Puerto B y Puerto E (RCGCGPIO) p. 340

    // ACTIVAR O DESACTIVAR EL DIVISOR P. 254                                                                                    
    SYSCTL->RCC &= ~(1<<20); //deshabilitar divisor (RCC) p. 254 
    //# cuentas = 20,000,000/1,000 = 20000
    //el divisor se habilita si el numero de cuentas es mayor a 65 mil (16 bits)
    // EL 20 INDICA SI SE DIVIDE O NO

    // HABILITAR FUNCIONES ALTERNATIVAS (AFSEL) P. 672                                               
    GPIOB->AFSEL |= (1<<4); //PB4
    GPIOB->AFSEL |= (1<<7); //PB7
    GPIOE->AFSEL |= (1<<4); //PE4
    //1 -> función alternativa
    //0 -> GPIO (I/O)

    //CONFIGURAR EL PIN COMO I/O O. 663
    GPIOB->DIR = (1<<4);
    GPIOB->DIR = (1<<7); 
    GPIOE->DIR = (1<<4); 
    //0->entrada
    //1->salida

    // SE PONE UN 4 EN EL LUGAR 4 POR EL AFSEL Y LAS TABLAS DE LA 1351 Y 689
    // EL 4 INDICA QUE ES PWM, LA POSICION QUE ES EL PIN PB4, PB7 Y PE4 
    GPIOB->PCTL |= (GPIOB->PCTL&0xFFF0FFFF) | 0x00040000; //indicar funcion alternativa (PWM) (PCTL) p. 689, 1351
    GPIOB->PCTL |= (GPIOB->PCTL&0x0FFFFFFF) | 0x40000000;
    GPIOE->PCTL |= (GPIOB->PCTL&0xFFF0FFFF) | 0x00040000;
    
    // HABILITAR LA FUNCION ALTERNATIVA, PARA DECIRLE SI ES DIGITAL O NO P.682
    GPIOB->DEN |= (1<<4); //PB4
    GPIOB->DEN |= (1<<7); //PB7
    GPIOE->DEN |= (1<<4); //PE4

    // P. 1266 EL BIT 0 INDICA EL MODO HABILITADO O DESHABILITADO
    //EL NÚMERO DE AQUI INDICA EL GENERADOR 
    // SE DESABILITA PARA CONFIGURARLO 
    PWM0->_0_CTL = (0<<0); //deshabilitar generador (CTL) p. 1266, 1233
    PWM0->_1_CTL = (0<<0); 
    PWM0->_2_CTL = (0<<0);
    //1->se habilitda todo el bloque PWM
    //0->se deshabilita todo el bloque PWM
     
    // MODO DE TRABAJO - CUENTA REGRESIVA P. 1285 Y 1239
    PWM0->_0_GENB = 0x0000080C; ///EL GENERADOR 0 TRABAJA CON EL PWM1 COMPARADOR B
    PWM0->_1_GENA = 0x0000008C; // EL GENERADOR 1 TRABAJA CON EL PWM2 Y CON EL COMPARADOR A
    PWM0->_2_GENA = 0x0000008C; // EL GENERADOR 2 TRABAJA CON EL PWM4 Y CON EL COMPARADOR A 
    //0x0000008C para modo countdown pmA' (en la p. 1248 dice con que comparador trabajan)
    //0x0000080C para modo countdown pmB'

    //**********************************************************
    // PARA LAS CUENTAS Y EL CICLO DE TRABAJO                                                  
    PWM0->_0_LOAD = 20000; //cuentas (carga = fclk/fpwm)
    PWM0->_1_LOAD = 20000;  
    PWM0->_2_LOAD = 20000; 

    PWM0->_0_CMPB = 1; //ciclo de trabajo (%)
    PWM0->_1_CMPA = 400; 
    PWM0->_2_CMPA = 8000; 
    //*********************************************************

    //ACTIVAR GENERADORES
    PWM0->_0_CTL = (1<<0);  
    PWM0->_1_CTL = (1<<0);
    PWM0->_2_CTL = (1<<0);

    PWM0->ENABLE = (1<<1)|(1<<2)|(1<<4); //habilitar el bloque p. 1248
}
