#include "STM32F407xx.h"

static void GPIO_LedConfig();
static void GPIO_ButtonInterruptConfig();
static void SPI_Config();
static void SPI_GPIO_Config();

SPI_HandleTypeDef_t SPI_Handle;
USART_HandleTypedef_t USART_Handle;

void EXTI0_IRQHandler()
{
	char msgToSend[] = "Hello World\n";
	if(EXTI->PR & 0x1)
	{
		EXTI->PR |= (0x1U << 0U);

		SPI_TransmitData_IT(&SPI_Handle, (uint8_t*)msgToSend, strlen(msgToSend));
	}
}

void SPI1_IRQHandler()
{
	SPI_InterruptHandler(&SPI_Handle);
}

int main(void)
{
	GPIO_LedConfig();
	GPIO_ButtonInterruptConfig();
	SPI_GPIO_Config();
	SPI_Config();

	for(;;)
	{
		GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_Pin_Set);

	}
}

static void GPIO_LedConfig()
{
	GPIO_InitTypeDef_t GPIO_LedStruct = { 0 };

	RCC_GPIOD_CLK_ENABLE();		// Clock Enable for LED's
	RCC_GPIOA_CLK_ENABLE();		// Clock Enable for Button

	// LED Configuration
	GPIO_LedStruct.pinNumber = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_LedStruct.Mode = GPIO_MODE_OUTPUT;
	GPIO_LedStruct.Speed = GPIO_SPEED_LOW;
	GPIO_LedStruct.Otype = GPIO_OTYPE_PP;
	GPIO_LedStruct.PuPd = GPIO_PUPD_NOPULL;
	GPIO_Init(GPIOD, &GPIO_LedStruct);

	memset(&GPIO_LedStruct, 0, sizeof(GPIO_LedStruct));

	// Button Configuration
	GPIO_LedStruct.pinNumber = GPIO_PIN_0;
	GPIO_LedStruct.Mode = GPIO_MODE_INPUT;
	GPIO_LedStruct.PuPd = GPIO_PUPD_PULLDOWN;
	GPIO_Init(GPIOA, &GPIO_LedStruct);
}

static void GPIO_ButtonInterruptConfig()
{
	EXTI_InitTypeDef_t EXTI_Struct = { 0 };

	RCC_SYSCFG_CLK_ENABLE();
	RCC_GPIOA_CLK_ENABLE();

	EXTI_LineConfig(EXTI_PortSource_GPIOA, EXTI_LineSource_0);

	EXTI_Struct.EXTI_LineCmd = ENABLE;
	EXTI_Struct.EXTI_LineNumber = EXTI_LineSource_0;
	EXTI_Struct.EXTI_Mode = EXTI_MODE_Interrupt;
	EXTI_Struct.TriggerSelection = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_Struct);

	NVIC_EnableInterrupt(EXTI0_IRQNumber);
}

static void SPI_Config()
{
	RCC_SPI1_CLK_ENABLE();

	SPI_Handle.Instance = SPI1;
	SPI_Handle.Init.BaudRate = SPI_BAUDRATE_DIV8;	// 2 MHz
	SPI_Handle.Init.BusConfig = SPI_BUS_FullDuplex;
	SPI_Handle.Init.CPHA = SPI_CPHA_FIRST;
	SPI_Handle.Init.CPOL = SPI_CPOL_LOW;
	SPI_Handle.Init.DFF_Format = SPI_DFF_8BITS;
	SPI_Handle.Init.FrameFormat = SPI_FRAMEFORMAT_MSB;
	SPI_Handle.Init.Mode = SPI_MODE_MASTER;
	SPI_Handle.Init.SSM_Cmd = SPI_SSM_ENABLE;
	SPI_Init(&SPI_Handle);

	NVIC_EnableInterrupt(SPI1_IRQNumber);
	SPI_PeriphCmd(&SPI_Handle, ENABLE);
}

static void SPI_GPIO_Config()
{
	GPIO_InitTypeDef_t GPIO_InitStruct = { 0 };

	GPIO_InitStruct.pinNumber = GPIO_PIN_5 | GPIO_PIN_7;		// PA5 = SCK, PA7 = MOSI
	GPIO_InitStruct.Mode = GPIO_MODE_AF;
	GPIO_InitStruct.Otype = GPIO_OTYPE_PP;
	GPIO_InitStruct.PuPd = GPIO_PUPD_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_VERY;
	GPIO_InitStruct.Alternate = GPIO_AF5;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
