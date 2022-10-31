#ifndef INC_USART_H_
#define INC_USART_H_

#include "STM32F407xx.h"				// Device Specific Header File 

// USART_InitTypedef_t yapısındaki parametrelerin aldığı değerlerdir ->  

// Mode için CR1-> Bit 2 ve Bit 3'e göre değerleri belirledik.
/* @def_group MODE_Types */
#define USART_MODE_Tx					( (uint32_t)(0x00000008) )	// Bit 3: 1 ise 
#define USART_MODE_Rx					( (uint32_t)(0x00000004) )	// Bit 2: 1 ise
#define USART_MODE_Tx_Rx				( (uint32_t)(0x0000000C) )	// Bit 2 ve 3: 1 ise 

// WordLenght için CR1-> Bit 12'ye göre değerleri belirledik.
/* @def_group WordLenght_Types */
#define USART_WORDLENGHT_8Bits				( (uint32_t)(0x00000000) )	// Bit 12: 0 ise
#define USART_WORDLENGHT_9Bits				( (uint32_t)(0x00001000) )	// Bit 12: 1 ise 

// Parity için CR1-> Bit 10 ve Bit 9'a göre değerleri belirledik.
// NOT: 9.bit(Parity Selection) ancak 10.bit (Parity Control Enable) aktif edildiğinde işe yarar. 
/* @def_group Parity_Modes */
#define USART_PARITY_NONE				( (uint32_t)(0x00000000) )	// Bit 10: 0 ,Bit 9: 0
#define USART_PARITY_Even				( (uint32_t)(0x00000400) )	// Bit 10: 1 ,Bit 9: 0  
#define USART_PARITY_Odd				( (uint32_t)(0x00000600) )	// Bit 10: 1 ,Bit 9: 1

// Çok yüksek hızlarda çalışırken veri iletilirken kaçırmamak adına bir gecikme sağlar.
// Stop Bits için CR2-> Bit 13:12'ye göre değerleri belirledik.
/* @def_group Stop_Bits */
#define USART_STOPBITS_1				( (uint32_t)(0x00000000) )	// Bit 13:0 ,Bit 12:0
#define USART_STOPBITS_Half				( (uint32_t)(0x00001000) )	// Bit 13:0 ,Bit 12:1
#define USART_STOPBITS_2				( (uint32_t)(0x00002000) )	// Bit 13:1 ,Bit 12:0
#define USART_STOPBITS_1_Half				( (uint32_t)(0x00003000) )	// Bit 13:1 ,Bit 12:1


// Bir bit boyunca kaç örneklem(sample) alsın. Kontrol ve hata yakalama amaçlı kullanılırlar.
// Over Sampling için CR1->Bit 15'e göre değeleri belirledik. 
/* @def_group OverSampling_Modes */
#define USART_OVERSAMPLE_16				( (uint32_t)(0x00000000) )	// Bit 15: 0 
#define USART_OVERSAMPLE_8				( (uint32_t)(0x00008000) )	// Bit 15: 1

// Hardware Flow Control için CR3-> Bit 9 ve Bit 8'e göre değerleri belirledik.
/* @def_group HardWareFlowControl_Modes */
#define USART_HW_NONE					( (uint32_t)(0x00000000) )	// Bit 9:0 ,Bit 8:0
#define USART_HW_RTS					( (uint32_t)(0x00000100) )	// Bit 9:0 ,Bit 8:1
#define USART_HW_CTS					( (uint32_t)(0x00000200) )	// Bit 9:1 ,Bit 8:0
#define USART_HW_CTS_RTS				( (uint32_t)(0x00000300) )	// Bit 9:1 ,Bit 8:1

typedef enum
{
	USART_FLAG_RESET = 0x0U,
	USART_FLAG_SET = !USART_FLAG_RESET

}USART_FlagStatus_t;


#define __USART_DIV_VALUE_8(__CLOCK__, __BAUDRATE__)		( (25U * (uint32_t)(__CLOCK__)) / (4U * (uint32_t)(__BAUDRATE__)) )
#define __USART_DIV_VALUE_16(__CLOCK__, __BAUDRATE__)		( (25U * (uint32_t)(__CLOCK__)) / (2U * (uint32_t)(__BAUDRATE__)) )

typedef enum
{
	USART_BUS_FREE = 0x0U,
	USART_BUS_Tx = 0x1U,
	USART_BUS_Rx = 0x2U

}USART_BusState_t;

// Register üzerinden kontrol edilecek bölümler, çoğunlukla CR(control register)'den faydalanılır ->
typedef struct
{ 
	uint32_t Mode;						/*!< Transmission and Reception Modes @def_group MODE_Types 			*/
	uint32_t BaudRate;					/*!< User Value for UART Baudrate			 							*/
	uint32_t WorldLenght;				/*!< 8 Bits & 9 Bits  Modes @def_group WordLenght_Types 				*/
	uint32_t Parity;					/*!< Even & Odd Parity Modes @def_group Parity_Modes 					*/
	uint32_t StopBits;					/*!< Stop Bits Modes @def_group Stop_Bits 								*/
	uint32_t OverSampling;				/*!< OverSampling Modes @def_group OverSampling_Modes 					*/
	uint32_t HardWareFlowControl;		/*!< HardWareFlowControl Modes @def_group HardWareFlowControl_Modes 	*/

}USART_InitTypedef_t;

// Hem konfigürasyon ayarlarının bulunduğu yapıya(USART_InitTypedef_t) hemde registerlere(USART_TypeDef_t) erişebilmek 
// için oluştulmuş yapıdır. 
// Konfigürasyon Ayarları: USART.h -> USART_InitTypedef_t yapısı, Registers: STM3244xx.h -> USART_TypeDef_t yapısı
typedef struct __USART_HandleTypedef_t
{
	USART_TypeDef_t *Instance;		// Register'lara erişebikmek için kullanılır. (Instance->CR1 gibi)
	USART_InitTypedef_t Init;		// Konfigürasyon Parametrelerine erişebilmek için kullanılır. (Init.Mode gibi)
	uint8_t *pTxBuffer;			// 
	uint16_t TxBufferSize;
	uint8_t TxStatus;
	void (*TxISR_Function)(struct __USART_HandleTypedef_t *USART_Handle);
	uint8_t *pRxBuffer;
	uint16_t RxBufferSize;
	uint8_t RxStatus;
	void(*RxISR_Function)(struct __USART_HandleTypedef_t *USART_Handle);

}USART_HandleTypedef_t;

// Konfigürasyon için kullanılan fonksiyondur.
void USART_Init(USART_HandleTypedef_t *USART_Handle);

// Veri göndermek için kullanılan fonksiyondur. 
void USART_TransmitData(USART_HandleTypedef_t *USART_Handle, uint8_t *pData, uint16_t dataSize);

// Veri almak için kullanılan fonksiyondur 
void USART_ReceiveData(USART_HandleTypedef_t *USART_Handle, uint8_t *pBuffer, uint16_t dataSize);

void USART_TransmitData_IT(USART_HandleTypedef_t *USART_Handle, uint8_t *pData, uint16_t dataSize);

void USART_ReceiveData_IT(USART_HandleTypedef_t *USART_Handle, uint8_t *pBuffer, uint16_t dataSize);

void USART_PeriphCmd(USART_HandleTypedef_t *USART_Handle, FunctionalState_t stateOfUSART);

USART_FlagStatus_t USART_GetFlagStatus(USART_HandleTypedef_t *USART_Handle, uint16_t flagName);

void USART_InterruptHandler(USART_HandleTypedef_t *USART_Handle);

#endif /* INC_USART_H_ */
