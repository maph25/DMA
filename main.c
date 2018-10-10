#include "MK64F12.h"
#include "NVIC.h"
#include "GPIO.h"
#include "Bits.h"
#include <stdio.h>

#define NUM_STEPS (1u)
#define SYSTEM_CLOCK (21000000u)
#define DELAY (0.01F)

#define ARRAY_SIZE (101u)

#define DMA_CH0 (0x01u)
#define DMA_SOURCE_GPIO (51u)

uint16_t g_sineWave[ARRAY_SIZE] = {2047, 2176, 2304, 2431, 2556, 2680, 2801, 2919,
		3033, 3144, 3250, 3352, 3448, 3539, 3624, 3703, 3775, 3841, 3899, 3950,
		3994, 4030, 4058, 4078, 4090, 4094, 4090, 4078, 4058, 4030, 3994, 3950,
		3899, 3841, 3775, 3703, 3624, 3539, 3448, 3352, 3250, 3144, 3033, 2919,
		2801, 2680, 2556, 2431, 2304, 2176, 2047, 1918, 1790, 1663, 1538, 1414,
		1293, 1175, 1061, 950, 844, 742, 646, 555, 470, 391, 319, 253, 195, 144,
		100, 64, 36, 16, 4, 0, 4, 16, 36, 64, 100, 144, 195, 253, 319, 391, 470,
		555, 646, 742, 844, 950, 1061, 1175, 1293, 1414, 1538, 1663, 1790, 1918,
		2047};//defines source data space
uint16_t g_data_desti[ARRAY_SIZE]; //defines destination data space


void DMA0_IRQHandler(void)
{

	uint8_t i;

	DMA0->INT = DMA_CH0;

	for ( i = 0; i < ARRAY_SIZE; ++i)
	{
		DAC0->DAT[i];
	}

}


void DMA_clock_gating(void)
{
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
}


void DMA_init(void)
{

	DMAMUX->CHCFG[0] = 0;
	DMAMUX->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK | /*enables DMA MUX channel*/
					   DMAMUX_CHCFG_SOURCE(DMA_SOURCE_GPIO);/*source is FTM0 channel 0*/

	DMA0->ERQ = 0x01;//enables DMA0 request

	DMA0->TCD[0].SADDR = (uint32_t)(&g_sineWave[0]);/*defines source data address*/
	DMA0->TCD[0].SOFF = 1;/*Source address signed offset;it is expressed in number of bytes*/
	DMA0->TCD[0].DADDR = (uint32_t)(&DAC0->DAT[0]);/*defines destination data address*/
	DMA0->TCD[0].DOFF = 1;/*destination address signed offset;it is expressed in number of bytes*/

	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(NUM_STEPS);// NUM_STEPS;/*CITER = 1*/
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(NUM_STEPS);/*BITER = 1*/
	DMA0->TCD[0].NBYTES_MLNO = 101;/*byte number*/

	DMA0->TCD[0].ATTR = 0;/*8 bit transfer size, in order to transfer see Kinetis user manual*/
	DMA0->TCD[0].SLAST = 0;//restores the source address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].DLAST_SGA = 0;/*restores the destination address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;/*The end-of-major loop interrupt is enabled*/


}

int main(void)
{
	gpio_pin_control_register_t sw2 = GPIO_MUX1 | GPIO_PE | GPIO_PS | DMA_FALLING_EDGE; /* GPIO configured to trigger the DMA*/

	GPIO_clock_gating(GPIO_C);
	GPIO_pin_control_register(GPIO_C, bit_6, &sw2);
	DMA_clock_gating();
	DMA_init(); /* Configure the T*/
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_5);
	NVIC_global_enable_interrupts;
    for (;;) {

    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
