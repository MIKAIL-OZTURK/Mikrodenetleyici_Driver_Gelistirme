#include "USART.h"


/*
 * @brief	USART_Init, configures the USART Peripheral
 *
 * @param	USART_Handle = User config structure
 *
 * @retval	void
 */
void USART_Init(USART_HandleTypedef_t *USART_Handle)
{
	uint32_t tempReg = 0;

	/***** OverSampling		WordLenght		Mode		Parity *****/
	tempReg = USART_Handle->Instance->CR1;
	tempReg |= (USART_Handle->Init.OverSampling) | (USART_Handle->Init.WorldLenght) | (USART_Handle->Init.Mode) | (USART_Handle->Init.Parity);
	USART_Handle->Instance->CR1 = tempReg;

	/***** StopBits *****/
	tempReg = USART_Handle->Instance->CR2;
	tempReg &= ~(USART_Handle->Init.StopBits);
	USART_Handle->Instance->CR2 = tempReg;

	/***** HardWwareFlowControl *****/
	tempReg = USART_Handle->Instance->CR3;
	tempReg |= (USART_Handle->Init.HardWareFlowControl);
	USART_Handle->Instance->CR3 = tempReg;
}
