#include "RCC.h"

// CLOCK | MCU -> PERIPHERALS 
// 1. System Clock
// 2. AHB Clock 
// 3. APB1 Clcok 
// 3. APB2 Clock 
// 
//							| ---> PERIPHERALS (GPIOA...GPIOI)
//							|
//				| ---> AHB1 < ---> APB1 ---> PERIPHERALS (TIM2...TIM5, UART4, USART3, DAC1, DAC2)
//				|			|
//				|			| ---> ABP2 ---> PERIPHERALS (TIM9...TIM11, USART1, ADC1...ADC3)
// MCU ---> AHB < ---> AHB2 ---> PERIPHERALS (RNG, CAMERA, USB OTG)
//				|
//				|
//				| ---> AHB3 ---> PERIPHERALS (External Memory Controller (FSMC))
//
//
// 										| ---> APB1	---> |	
//  USART CLOCK İÇİN:			USARTx <				 < ---> AHB1 ---> AHB(System Clock) ---> MCU
//										| ---> ABP2 ---> |
//
// Functions ->
// RCC_GetSystemClock()	: Find System Clock (AHB Clock)
// RCC_GetHClock()		: Find AHB1 Clock
// RCC_GetPClock1()		: Find APB1 Clock
// RCC_GetPClock2()		: Find APB2 Clock 


const uint8_t AHB_Prescalers[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};	// Bölünme oranları (DIV2, DIV4 gibi)
const uint8_t APB_Prescalers[] = {0, 0, 0, 0, 1, 2, 3, 4};



/*
 * @brief	RCC_GetSystemClock, find system core clock
 *
 * @retval	uint32_t
 */
										/****************** AHB CLOCK ******************/
uint32_t RCC_GetSystemClock(void)
{
	uint32_t SystemCoreClock = 0;	// Sistem clock değerini tutacak değişken
	uint8_t clkSource = 0;			// Sistemde hangi clock'un kullanıldığını göstermesi için değişken oluşturduk.
 
	clkSource = ((RCC->CFGR >> 2U) & 0X3U);	// Sistem clock'u bulmak için RCC->CFGR->SWS registerine eriştik.	
	// SWS(System clock switch status): Sistemde hangi clock'un kullanıldığını gösterir. 

	switch(clkSource)
	{
		case 0:  SystemCoreClock = 16000000;	break;	// Sistem Clock -> HSI: 16 MHz
		case 1:  SystemCoreClock = 8000000;		break;	// Sistem Clock -> HSE: 8  MHz
		default: SystemCoreClock = 16000000;			// Varsayılan olarak 16 MHz yaptık 
	}
	return SystemCoreClock;
}

// Sistem clock belli bir bölünme oranı(division(DIV)) ile bölünerek çevresel birimlere aktarılır. 
// Bu clock çevresel birimlere dağıtılırken belli bir bölünme oranı(division(DIV)) ile bölünür ki 
// çevresel birimlerin çalışacağı seviyeye gelebilsin. 
//
// ( System Clock / DIV ) = 

/*
 * @brief	RCC_GetHClock is find AHB clock
 *
 * @retval	uint32_t
 */
										/****************** AHB1 CLOCK ******************/
uint32_t RCC_GetHClock(void)
{
	uint32_t AHB_PeriphClock = 0;	// AHB Clock 
	uint32_t SystemCoreClock = 0;	// System Clock 
	uint8_t HPRE_Value = 0;
	uint8_t tempValue = 0;

	SystemCoreClock = RCC_GetSystemClock();				// AHB(System Clock) bulundu 
	HPRE_Value = ((RCC->CFGR >> 4U) & 0xFU);			// Kaça bölünmesi gerektiğini tutan değişken 
	tempValue = AHB_Prescalers[HPRE_Value];				// Bölünme oranı
	AHB_PeriphClock = (SystemCoreClock >> tempValue);	// 

	/*
	Örneğin system clock 16 MHz olsun (16 000 000) ve 16 bölünme oranı(DIV16) ile bölmek isteyelim.
	Peki bir DIV16 yapmak için registere hangi değeri vermemiz gerekir -> Reference Manual -> RCC -> RCC_CFGR -> HPRE -> Bit 7:4 (HPRE)
	DIV16 yapmak için registere 1011 yazmak gerekir yani 11 değeri. 

	SystemCoreClock = 16 000 000
	HPRE_Value = 11 
	AHB_Prescalers[11] = 4 	(tempValue = 4)
	AHB_PeriphClock = 16
	*/
	return AHB_PeriphClock;
}



/*
 * @brief	RCC_GetPClock1 is find APB1 clock
 *
 * @retval	uint32_t
 */
										/****************** APB1 CLOCK ******************/
uint32_t RCC_GetPClock1(void)
{
	uint32_t APB1_PeriphClock = 0;
	uint32_t Hclock = 0;
	uint8_t HPRE1_Value = 0;
	uint8_t tempValue = 0;

	Hclock = RCC_GetHClock();						// AHB1 Clock (APB1, AHB1 veri yoluna bağlıdır.Önce AHB1 bulunur, ardından APB1 bulunur.)
	HPRE1_Value = ((RCC->CFGR >> 10U) & 0x7U);		// Kaça bölünmesi gerektiğini tutan değişken 
	tempValue = APB_Prescalers[HPRE1_Value];		// Bölünme oranı
	APB1_PeriphClock = (Hclock >> tempValue);

	return APB1_PeriphClock;
}



/*
 * @brief	RCC_GetPClock2 is find APB2 clock
 *
 * @retval	uint32_t
 */
										/****************** APB2 CLOCK ******************/
uint32_t RCC_GetPClock2(void)
{
	uint32_t APB2_PeriphClock = 0;
	uint32_t Hclock = 0;
	uint8_t HPRE2_Value = 0;
	uint8_t tempValue = 0;

	Hclock = RCC_GetHClock();					// AHB1 Clock (APB1, AHB1 veri yoluna bağlıdır.Önce AHB1 bulunur, ardından APB1 bulunur.)
	HPRE2_Value = ((RCC->CFGR >> 13U) & 0x7U);	// Kaça bölünmesi gerektiğini tutan değişken 
	tempValue = APB_Prescalers[HPRE2_Value];	// Bölünme oranı
	APB2_PeriphClock = (Hclock >> tempValue);

	return APB2_PeriphClock;
}
