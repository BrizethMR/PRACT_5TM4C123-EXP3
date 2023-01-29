#include "lib/include.h"

volatile uint8_t red; //volatil para poderla monitorear 
volatile uint8_t green;
volatile uint8_t blue;
volatile uint16_t color;
volatile uint8_t R;
volatile uint8_t G;
volatile uint8_t B;


uint16_t count;
int main(void)
{
    
//Experimento 3
/* Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz.
 * se enviara dato desde interfaz de simulink para controlar la intensidad luminosa 
 * usando un led RGB externa  
 */

//***************** SE SIMULARA LA CREACIÓN DE UN COLOR CON VALORES DE RGB DE 0 A 255 ***************************
    Configurar_PLL(_20MHZ);  //configuracion de velocidad de reloj
    Configurar_UART0();
    Configura_Reg_PWM1(8);

    red = 114;
    green = 103;
    blue = 98;
    
    

    while (1)
    {
        color = readChar();
        
        switch (color)
        {
            case 'r':
                while (red == 114)
                {
                    red = (int)readChar(); //recibe el % del uart para calcular el dc%
                }
                R = ((int)(red*255)/100); //se hará una conversión para crear un color que vaya de 0 a 255 
                PWM0->_0_CMPB = 20000-((int)(R*10000)/50); //PB7 (R)
                red = 114;
            break;

            case 'g':
                while (green == 103)
                {
                    green = (int)readChar();
                }
                G = ((int)(green*255)/100);
                PWM0->_2_CMPA = 20000-((int)(G*10000)/50); //PE4 (G)
                green = 103;
            break;

            case 'b':
                while (blue == 98)
                {
                    blue = (int)readChar();
                }
                B = ((int)(blue*255)/100);
                PWM0->_1_CMPA = 20000-((int)(B*10000)/50); //PB4 (B)
                blue = 98;
            break;
        }   
    }
}

