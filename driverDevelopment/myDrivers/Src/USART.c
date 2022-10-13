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
	uint32_t periphClock = 0;
	uint32_t mantissaPart = 0;
	uint32_t fractionPart = 0;
	uint32_t USART_DIV_Value = 0;
	uint32_t tempValue = 0;

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

	/***** Baud Rate Configuration *****/
	if(USART_Handle->Instance == USART1 || USART_Handle->Instance == USART6)
	{
		periphClock = RCC_GetPClock2();
	}
	else
	{
		periphClock = RCC_GetPClock1();
	}

	if(USART_Handle->Init.OverSampling == USART_OVERSAMPLE_8)
	{
		USART_DIV_Value = __USART_DIV_VALUE_8(periphClock, USART_Handle->Init.BaudRate);
		mantissaPart = (USART_DIV_Value / 100U);
		fractionPart = (USART_DIV_Value) - (mantissaPart * 100U);
		fractionPart = (((fractionPart * 8U) + 50U) / 100U) & (0x07U);
	}
	else
	{
		USART_DIV_Value = __USART_DIV_VALUE_16(periphClock, USART_Handle->Init.BaudRate);
		mantissaPart = (USART_DIV_Value / 100U);
		fractionPart = (USART_DIV_Value) - (mantissaPart * 100U);
		fractionPart = (((fractionPart * 16U) + 50U) / 100U) & (0x0FU);
	}

	tempValue |= (mantissaPart << 4U);
	tempValue |= (fractionPart << 0U);
	USART_Handle->Instance->BRR = tempValue;
}



/*
 * @brief	USART_TransmitData, Transmits data
 *
 * @param	USART_Handle = User config structure
 * @param	pData = Address of data to send
 * @param	dataSize = Length of your data in bytes
 *
 * @retval	void
 */
void USART_TransmitData(USART_HandleTypedef_t *USART_Handle, uint8_t *pData, uint16_t dataSize)
{
	uint16_t *data16Bits = 0;

	if((USART_Handle->Init.WorldLenght == USART_WORDLENGHT_9Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE))
	{
		data16Bits = (uint16_t*)pData;

	}
	else
	{
		data16Bits = NULL;
	}

	while( dataSize > 0 )
	{
		while( !(USART_GetFlagStatus(USART_Handle, USART_TxE_FLAG)) );

		/* 9Bits Data no Parity is for ELSE Condition, for others you will be in IF Case */
		if(data16Bits == NULL)
		{
			USART_Handle->Instance->DR = (uint8_t)(*pData & 0xFFU);
			pData++;
			dataSize--;
		}
		else
		{
			USART_Handle->Instance->DR = (uint16_t)(*data16Bits & (0x01FFU));
			data16Bits++;
			dataSize -= 2;
		}
	}

	while( !(USART_GetFlagStatus(USART_Handle, USART_TC_FLAG)) );
}



/*
 * @brief	USART_ReceiveData, Receives data
 *
 * @param	USART_Handle = User config structure
 * @param	pBuffer = Address of data to store
 * @param	dataSize = Length of your data in bytes
 *
 * @retval	void
 */
void USART_ReceiveData(USART_HandleTypedef_t *USART_Handle, uint8_t *pBuffer, uint16_t dataSize)
{
	uint16_t *p16BitsBuffer;
	uint8_t *p8BitsBuffer;

	if((USART_Handle->Init.WorldLenght == USART_WORDLENGHT_9Bits) && ( USART_Handle->Init.Parity == USART_PARITY_NONE))
	{
		p16BitsBuffer = (uint16_t*)pBuffer;
		p8BitsBuffer = NULL;
	}
	else
	{
		p8BitsBuffer = (uint8_t*)pBuffer;
		p8BitsBuffer = NULL;
	}

	while(dataSize > 0)
	{
		while(!(USART_GetFlagStatus(USART_Handle, USART_RxNE_FLAG)));

		if(p8BitsBuffer == NULL)
		{
			*p16BitsBuffer = (uint16_t)(USART_Handle->Instance->DR & 0x01FFU);
			p16BitsBuffer++;
			dataSize -= 2;
		}
		else
		{
			if((USART_Handle->Init.WorldLenght == USART_WORDLENGHT_9Bits) && (USART_Handle->Init.Parity != USART_PARITY_NONE) )
			{
				*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
				p8BitsBuffer++;
				dataSize--;
			}
			else if((USART_Handle->Init.WorldLenght == USART_WORDLENGHT_8Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE) )
			{
				*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
				p8BitsBuffer++;
				dataSize--;
			}
			else
			{
				*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x007FU);
				p8BitsBuffer++;
				dataSize--;
			}
		}
	}
}



/*
 * @brief	USART_PeriphCmd, Enable or disable USART Peripheral
 *
 * @param	USART_Handle = User config structure
 * @param	stateOfUSART = ENABLE of DISABLE
 *
 * @retval	void
 */
void USART_PeriphCmd(USART_HandleTypedef_t *USART_Handle, FunctionalState_t stateOfUSART)
{
	if(stateOfUSART == ENABLE)
	{
		USART_Handle->Instance->CR1 |= (0x1U << USART_CR1_UR);
	}
	else
	{
		USART_Handle->Instance->CR1 &= ~(0x1u << USART_CR1_UR);
	}
}



/*
 * @brief	USART_GetFlagStatus, Return the flag of SR Register
 *
 * @param	USART_Handle = User config structure
 * @param	flagName = Flag name of SR Register
 *
 * @retval	USART_FlagStatus_t
 */
USART_FlagStatus_t USART_GetFlagStatus(USART_HandleTypedef_t *USART_Handle, uint16_t flagName)
{
	return ((USART_Handle->Instance->SR & flagName) ? USART_FLAG_SET : USART_FLAG_RESET);
}
