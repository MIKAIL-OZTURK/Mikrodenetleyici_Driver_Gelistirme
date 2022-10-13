#include "STM32F407xx.h"

static void GPIO_Config(void);
static void USART_Config(void);

USART_HandleTypedef_t USART_Handle;
GPIO_InitTypeDef_t GPIO_InitStruct;


int main(void)
{
	GPIO_Config();
	USART_Config();

	char messageToSend[20] = "";

	USART_ReceiveData(&USART_Handle, (uint8_t*)messageToSend, 12);
	USART_TransmitData(&USART_Handle, (uint8_t*)messageToSend, strlen(messageToSend));

	while(1)
	{

	}
}


static void USART_Config(void)
{
	RCC_USART2_CLK_ENABLE();

	USART_Handle.Instance = USART2;
	USART_Handle.Init.BaudRate = 115200;
	USART_Handle.Init.HardWareFlowControl = USART_HW_NONE;
	USART_Handle.Init.Mode = USART_MODE_Tx_Rx;
	USART_Handle.Init.OverSampling = USART_OVERSAMPLE_16;
	USART_Handle.Init.Parity = USART_PARITY_NONE;
	USART_Handle.Init.StopBits = USART_STOPBITS_1;
	USART_Handle.Init.WorldLenght = USART_WORDLENGHT_8Bits;

	USART_Init(&USART_Handle);
	USART_PeriphCmd(&USART_Handle, ENABLE);
}


static void GPIO_Config(void)
{
	RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_AF;
	GPIO_InitStruct.Alternate = GPIO_AF7;
	GPIO_InitStruct.pinNumber = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPd = GPIO_PUPD_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_VERY;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
