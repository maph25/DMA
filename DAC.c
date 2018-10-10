/*
*	DAC.c
*   Author: Andrea Perez & Fernanda Muñoz
*
**/


#include "Bits.h"
#include "DAC.h"
#include "PIT.h"
#include "NVIC.h"
#include "GPIO.h"


void DAC_clockGating(){
	SIM->SCGC2 |= DAC0_clockGating_mask;
}

void DAC_enable(){
	DAC0->C0|=0xC0;
}
void DAC_output(uint16 value){
	uint8 low = value & 0x00FF;
	uint8 high = value>>8;
	DAC0->DAT[0].DATL= low;
	DAC0->DAT[0].DATH= high;
}


uint16_t sineWave[SIZEWAVE] = { 2047, 2176, 2304, 2431, 2556, 2680, 2801, 2919,
		3033, 3144, 3250, 3352, 3448, 3539, 3624, 3703, 3775, 3841, 3899, 3950,
		3994, 4030, 4058, 4078, 4090, 4094, 4090, 4078, 4058, 4030, 3994, 3950,
		3899, 3841, 3775, 3703, 3624, 3539, 3448, 3352, 3250, 3144, 3033, 2919,
		2801, 2680, 2556, 2431, 2304, 2176, 2047, 1918, 1790, 1663, 1538, 1414,
		1293, 1175, 1061, 950, 844, 742, 646, 555, 470, 391, 319, 253, 195, 144,
		100, 64, 36, 16, 4, 0, 4, 16, 36, 64, 100, 144, 195, 253, 319, 391, 470,
		555, 646, 742, 844, 950, 1061, 1175, 1293, 1414, 1538, 1663, 1790, 1918,
		2047 };

uint16_t triangularWave[SIZEWAVE] = { 0, 82, 164, 246, 328, 409, 491, 573, 655,
		737, 819, 901, 983, 1064, 1146, 1228, 1310, 1392, 1474, 1556, 1638,
		1719, 1801, 1883, 1965, 2047, 2129, 2211, 2293, 2375, 2456, 2538, 2620,
		2702, 2784, 2866, 2948, 3030, 3111, 3193, 3275, 3357, 3439, 3521, 3603,
		3685, 3766, 3848, 3930, 4012, 4094, 4012, 3930, 3848, 3766, 3685, 3603,
		3521, 3439, 3357, 3275, 3193, 3111, 3030, 2948, 2866, 2784, 2702, 2620,
		2538, 2456, 2375, 2293, 2211, 2129, 2047, 1965, 1883, 1801, 1719, 1638,
		1556, 1474, 1392, 1310, 1228, 1146, 1064, 983, 901, 819, 737, 655, 573,
		491, 409, 328, 246, 164, 82, 0 };

uint16_t squareWave[SIZEWAVE] = { 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094, 4094,
		4094, 4094, 4094, 4094, 4094, 4094, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4094 };

void sine_led(){
	GPIO_setPIN(GPIO_D, BIT0);//LED1
	GPIO_setPIN(GPIO_D, BIT2);//LED2
	GPIO_clearPIN(GPIO_D, BIT2);
}
void square_led(){
	GPIO_setPIN(GPIO_D, BIT0);
	GPIO_setPIN(GPIO_D, BIT2);
	GPIO_clearPIN(GPIO_D, BIT0);
}
void triangular_led(){
	GPIO_setPIN(GPIO_D, BIT0);
	GPIO_setPIN(GPIO_D, BIT2);
}
void turn_rgb_off(){
	//apagamos todos los leds
	GPIO_setPIN(GPIO_B, BLUE_LED_PIN);
	GPIO_setPIN(GPIO_B, RED_LED_PIN);
	GPIO_setPIN(GPIO_E, GREEN_LED_PIN);
}

void Wave_gen(WaveType wave, SIZEWAVE){
	switch(wave){
			case TRIANGLE:
				triangular_led();
				DAC_output(TriangleWave[SIZEWAVE]);
				GPIO_clearPIN(GPIO_E,GREEN_LED_PIN);
				break;
			case SQUARE:
				square_led();
				DAC_output(SquareWave[SIZEWAVE]);
				GPIO_clearPIN(GPIO_D,BLUE_LED_PIN);
				break;
			case SQUARE:
				sine_led();
				DAC_output(sineWave[SIZEWAVE]);
				GPIO_clearPIN(GPIO_D,RED_LED_PIN);
				break;
			default:
			break;
		}


}
