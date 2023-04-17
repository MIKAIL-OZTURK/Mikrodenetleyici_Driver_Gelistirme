#include "USART.h"



/*
 * @brief	closeUSART_ISR_Tx, Disables the interrupt for transmission
 *
 * @param	USART_Handle = User config structure
 *
 * @retval	void
 */
static void closeUSART_ISR_Tx(USART_HandleTypedef_t *USART_Handle)
{
	USART_Handle->TxBufferSize = 0;
	USART_Handle->pTxBuffer = NULL;
	USART_Handle->TxStatus = USART_BUS_FREE;

	USART_Handle->Instance->CR1 &= ~(0X1U << USART_CR1_TxEIE);
}



/*
 * @brief	closeUSART_ISR_Rx, Disables the interrupt for transmission
 *
 * @param	USART_Handle = User config structure
 *
 * @retval	void
 */
static void closeUSART_ISR_Rx(USART_HandleTypedef_t *USART_Handle)
{
	USART_Handle->RxBufferSize = 0;
	USART_Handle->pRxBuffer = NULL;
	USART_Handle->RxStatus = USART_BUS_FREE;

	USART_Handle->Instance->CR1 &= ~(0X1U << USART_CR1_RxNEIE);
}



/*
 * @brief	USART_SendWith_IT, Stores the user data into the DR register for 8 or 16 bits format
 *
 * @param	USART_Handle = User config structure
 *
 * @retval	void
 */
static void USART_SendWith_IT(USART_HandleTypedef_t *USART_Handle)
{
	if((USART_Handle->Init.WorldLenght == USART_WORDLENGHT_9Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE))
	{
		uint16_t *p16BitsData = (uint16_t*)(USART_Handle->pTxBuffer);

		USART_Handle->Instance->DR = (uint16_t)(*p16BitsData & (uint16_t)0x01FF);
		USART_Handle->pTxBuffer += sizeof(uint16_t);
		USART_Handle->TxBufferSize -= 2;
	}
	else
	{
		USART_Handle->Instance->DR = (uint8_t)(*(USART_Handle->pTxBuffer) & (uint8_t)0x00FF);
		USART_Handle->pTxBuffer++;
		USART_Handle->TxBufferSize--;
	}

	if(USART_Handle->TxBufferSize == 0)
	{
		closeUSART_ISR_Tx(USART_Handle);
	}
}



/*
 * @brief	USART_ReceiveWith_IT, Reads the data register and stores into the user variable for 8 or 16 bits format
 *
 * @param	USART_Handle = User config structure
 *
 * @retval	void
 */
static void USART_ReceiveWith_IT(USART_HandleTypedef_t *USART_Handle)
{
	uint16_t *p16BitsBuffer;
	uint8_t *p8BitsBuffer;

	if((USART_Handle->Init.WorldLenght == USART_WORDLENGHT_9Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE))
	{
		p16BitsBuffer = (uint16_t*)USART_Handle->pRxBuffer;
		p8BitsBuffer = NULL;
	}
	else
	{
		p8BitsBuffer = (uint8_t*)USART_Handle->pRxBuffer;
		p16BitsBuffer = NULL;
	}

	if(p8BitsBuffer == NULL)
	{
		*p16BitsBuffer = (uint16_t)(USART_Handle->Instance->DR & 0x01FFU);
		USART_Handle->pRxBuffer += sizeof(uint16_t);
		USART_Handle->RxBufferSize -= 2;
	}
	else
	{
		if((USART_Handle->Init.WorldLenght == USART_WORDLENGHT_9Bits) && (USART_Handle->Init.Parity != USART_PARITY_NONE))
		{
			*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
			USART_Handle->pRxBuffer++;
			USART_Handle->RxBufferSize--;
		}
		else if((USART_Handle->Init.WorldLenght == USART_WORDLENGHT_8Bits) && (USART_Handle->Init.Parity != USART_PARITY_NONE))
		{
			*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x00FFU);
			USART_Handle->pRxBuffer++;
			USART_Handle->RxBufferSize--;
		}
		else
		{
			*p8BitsBuffer = (uint8_t)(USART_Handle->Instance->DR & 0x007FU);
			USART_Handle->pRxBuffer++;
			USART_Handle->RxBufferSize--;
		}
	}

	if(USART_Handle->RxBufferSize == 0)
	{
		closeUSART_ISR_Rx(USART_Handle);
	}
}




/*
 * @brief	USART_Init, configures the USART Peripheral
 *
 * @param	USART_Handle = User config structure
 *
 * @retval	void
 */
// Konfigürasyon için kullanılan fonksiyondur.
void USART_Init(USART_HandleTypedef_t *USART_Handle)
{
	uint32_t tempReg = 0;
	uint32_t periphClock = 0;
	uint32_t mantissaPart = 0;
	uint32_t fractionPart = 0;
	uint32_t USART_DIV_Value = 0;
	uint32_t tempValue = 0;
	
	// CR1-2-3 registerlerinde bulunan konfigürasyon ayarları 
	/***** OverSampling		WordLenght		Mode		Parity *****/
	tempReg = USART_Handle->Instance->CR1;
	tempReg |= (USART_Handle->Init.OverSampling) | (USART_Handle->Init.WorldLenght) | (USART_Handle->Init.Mode) | (USART_Handle->Init.Parity);
	USART_Handle->Instance->CR1 = tempReg;


	/***** StopBits *****/
	tempReg = USART_Handle->Instance->CR2;
	tempReg &= ~(0x3U << USART_Handle->Init.StopBits);
	USART_Handle->Instance->CR2 = tempReg;

	/***** HardWwareFlowControl *****/
	tempReg = USART_Handle->Instance->CR3;
	tempReg |= (USART_Handle->Init.HardWareFlowControl);
	USART_Handle->Instance->CR3 = tempReg;
	// Register üzerinde doğrudan işlem yapmıyoruz. İlgili register için geçici bir değişken oluşturduk.
	// Konfigürasyon ayarlarını geçici değişeknde yaptık ve tüm ayarlamalar tamamlanınca geçici değişkende tutulan
	// kongigürasyon ayarları ilgili registere yüklendi. (USART_Handle->Instance->CR1 = tempReg)
	
	/***** Baud Rate Configuration *****/
	if(USART_Handle->Instance == USART1 || USART_Handle->Instance == USART6)
	// USART1 ve USART6 APB2 clock hattına bağlıdır. O yüzden farklı bir clock ile baud rate değerleri hesaplanır
	{	//APB2 Clock hattı için clock ayarları --> USART1 ve USART6
		periphClock = RCC_GetPClock2();
	}
	else // APB1 Clock hattı için clock ayarları --> Diğer USART birimleri (USART2, UART5..)
	{
		periphClock = RCC_GetPClock1();
	}
//
// Baud Rate Formülü -> (Kaynak: Reference Manual -> Page 978: Fractional baud rate generation)
//
//					Peripheral Clock
//		USARTDIV = ------------------------------------------------
//				8 x (2 - OVERSAMPLINGx) x (Tx/Rx Baud)
//
// OVERSAMPLING8 = 1
// OVERSAMPLING16 = 0
// USARTDIV = Donanımın istenilen baud rate için ayarlanması sağlayan değer 
// Tx/Rx Baud = Baud Rate
// Peripheral Clock = USART donanımının çalışma frekansı yani APB1~APB2 üzererinden gelen clock frekansı
/*
	ÖRNEK - 1 : Baud Rate Hızımızı 9600'e ayarlayalım ve OVERSAMPLING16 olsun(default). Peripheral Clock 8MHz olsun. -->

			  8.000.000
	USARTDIV = ------------------------- = 52.083333
			8 x (2-0) x 9600

	Bulduğumuz bu değeri USART_BRR registerine yazmak için mantissa ve fraction şeklinde ikiye ayırmamız gerekmekte. ->
	Mantissa = 52
	Fractions = 16 x 0.083333 ~= 1

	En son bu değerleri USART_BRR registerine yazıyoruz
*/

	if(USART_Handle->Init.OverSampling == USART_OVERSAMPLE_8)
	{																							// ÖRNEK - 1'e göre yorumlarsak:
		USART_DIV_Value = __USART_DIV_VALUE_8(periphClock, USART_Handle->Init.BaudRate);	// USARTDIV değerini bulduk 	(USARTDIV = 52.083333)
		mantissaPart = (USART_DIV_Value / 100U);						// mantissa = (USARTDIV / 100) => (52.083333 / 100) = 52
		fractionPart = (USART_DIV_Value) - (mantissaPart * 100U);				// fraction = (52.083) - ((52/100) * 100) = (52.083333 - 52) = 0.083333
		fractionPart = (((fractionPart * 8U) + 50U) / 100U) & (0x07U);
	}
	else /* USART_Handle->Init.OverSampling == USART_OVERSAMPLE_16 */
	{
		USART_DIV_Value = __USART_DIV_VALUE_16(periphClock, USART_Handle->Init.BaudRate);
		mantissaPart = (USART_DIV_Value / 100U);
		fractionPart = (USART_DIV_Value) - (mantissaPart * 100U);
		fractionPart = (((fractionPart * 16U) + 50U) / 100U) & (0x0FU);
	}

	/* BRR(Baud Rate Register) - Baud rate hızı için mantissa ve fraction değerlerinin yazılması gerekn registerdir. 		 */
	tempValue |= (mantissaPart << 4U);			// 9600 BaudRate için mantissa değerini registere(USART_BRR) yazıyoruz (mantissa = 52)
	tempValue |= (fractionPart << 0U);			// 9600 BaudRate için fraction değerini registere(USART_BRR) yazıyoruz (fraction = 0.083333)
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
// Veri göndermek için kullanılan fonksiyondur. 
void USART_TransmitData(USART_HandleTypedef_t *USART_Handle, uint8_t *pData, uint16_t dataSize)
{
	// Durumlar: 
	// 1. 9 bit veri no parity
	// 2. 8 bit veri no parity
	// 3. 9 bit, yes parity ise 8 bit veri 1 bit parity
	// 4. 8 bit, yer parity ise 7 bit veri 1 bit parity
	uint16_t *data16Bits = 0; // Veri max 9 bit olacağından min 16 bit bir değişken oluşturuyoruz. 
	// data16Bits 9 bitlik veri için oluşturulmuştur. 9 bitlik veri (uint8_t *pData)<- burada depolanamz.  

	// 1. DURUM: 9 bit veri no parity -> 
	if((USART_Handle->Init.WorldLenght == USART_WORDLENGHT_9Bits) && (USART_Handle->Init.Parity == USART_PARITY_NONE))
	{
		data16Bits = (uint16_t*)pData;	// 9 bitlik veri (uint8_t *pData)<- burada depolanamz.  
		// o yüzden 9 bitlik işlemler içim gelen veri artık data16Bits değişkenininde işlenecek 
	}
	else
	{
		data16Bits = NULL;	// Eğer veri 9 bit değilse NULL göster(rastgele yerleri göstermesin diye) çünkü biz 
		// data16Bits değişkenini  sadece 9 bit veri için oluşturduk. Burada işi bitiyor. Diğer tüm işlemler 
		// uint8_t *pData üzerinden çalışabilir. 
	}

	while( dataSize > 0 )	// Veri boyutu sııfr değilse veri gelmeye devam ediyordur. Mesaj gönderme işlemi -->
	{
		while( !(USART_GetFlagStatus(USART_Handle, USART_TxE_FLAG)) );
		// USART_TxE_FLAG: Transmit data register empty. 1 ise veri yazılabilir, 0 ise veri yazılamaz. 
		// USART_TxE_FLAG işlem sırasında 0'dır, veri yazılamaz.
		// while(! (0)) = while(1) (İşlem bitene kadar bekle)
		// İşlem bitince USART_TxE_FLAG 1 olur. Artık veri yazılabilir.
		// while(! (1)) = while (0) döngü kırıldı. Diğer işlemler ----> 

		if(data16Bits == NULL)	// data16Bits NULL ise artık bu değişken ile işim bitmiştir. Yani 9 bitlik bir 
		// işlem yapmayacağım artık. Artık tüm veriler 8 veya 7 bit'tir. uint8_t *pData ile çalışılabilir. 
		{
			USART_Handle->Instance->DR = (uint8_t)(*pData & 0xFFU);
			// (*pData & 0xFFU): Verim 8 bit olduğundan sağlama olması açısından yeniden 8 biti 1 olan sayı ile 
			// & işelemine tabi tuttuk. Böylece pData max 8 bit olması sağlanmış olur. 
			pData++;	// Verinin adresini arttırdık. 0x04'deki veri işlendiğinde sıra 0x08'deki veriye gelsin...
			dataSize--; // Verinin boyutu azalttık. Veriler 1 bayt 1 bayt gönderilir. Her gönderimde benim verim 1 bayt
			// azalır ve karşıya giden veriler 1 bayt 1 bayt artar. ( 8 bit 1 bayt ve değişkenimiz: uint8_t pData )
		}
		else // NULL değilse demek ki verim 9 bittir ve data16Btis 9 bitlik bir veriyi tutmaktadır. uint8_t *pData ile çalışılamaz.
		{
			USART_Handle->Instance->DR = (uint16_t)(*data16Bits & (0x01FFU));
			// (*data16Bits & 0x01FFU): Verim 9 bit olduğundan 16 bit çok fazla ve gereksiz işlemler ile 
			// program uzayabilir. O yüzden 9 biti 1 olan bir sayı ile data16Bits ile & işlemine tabi tutarsam
			// data16Bits'lik değişkenim sanki 9 bitmiş gibi hareket edecektir. 
			data16Bits++; // Verinin adresini arttırdık. 0x04'deki veri işlendiğinde sıra 0x08'deki veriye gelsin...
			dataSize -= 2;// Verinin boyutu azalttık. Veriler 2 bayt 2 bayt gönderilir. Her gönderimde benim verim 2 bayt
			// azalır ve karşıya giden veriler 2 bayt 2 bayt artar. 
		}
	}

	while( !(USART_GetFlagStatus(USART_Handle, USART_TC_FLAG)) );
	// USART_TC_FLAG: Transmission complete. 0 ise gönderim tamamlanmadı, 1 ise gönderim tamamlandı demek. 
	// Gönderim tamamlanmadı ise USART_TC_FLAG = 0'dır.
	// while(! (0)) = while(1) Bekle !
	// Gönderim tamamlanınca USART_TC_FLAG = 1'dir.
	// while(! (1)) = while(0) Döngüyü Kır, işlem tamam. 
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
void USART_TransmitData_IT(USART_HandleTypedef_t *USART_Handle, uint8_t *pData, uint16_t dataSize)
{
	USART_BusState_t usartBusState = USART_Handle->TxStatus;

	if(usartBusState != USART_BUS_Tx)
	{
		USART_Handle->pTxBuffer = (uint8_t*)pData;
		USART_Handle->TxBufferSize = (uint16_t)dataSize;
		USART_Handle->TxISR_Function = USART_SendWith_IT;

		USART_Handle->Instance->CR1 |= (0x1U << USART_CR1_TxEIE);
	}
}



/*
 * @brief	USART_ReceiveData_IT, Read the data from external world with interrupt method
 *
 * @param	USART_Handle 	= User config structure
 * @param	pBuffer 		= Stores the data in this variable
 * @param	dataSize 		= Bytes numbers that we will read
 *
 * @retval	void
 */
void USART_ReceiveData_IT(USART_HandleTypedef_t *USART_Handle, uint8_t *pBuffer, uint16_t dataSize)
{
	USART_BusState_t usartBusState = USART_Handle->RxStatus;

	if(usartBusState != USART_BUS_Rx)
	{
		USART_Handle->pRxBuffer = (uint8_t*)pBuffer;
		USART_Handle->RxBufferSize = (uint16_t)dataSize;
		USART_Handle->RxISR_Function = USART_ReceiveWith_IT;

		USART_Handle->Instance->CR1 |= (0x1U << USART_CR1_RxNEIE);
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
// USART'ı aktif eden fonksiyondur
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
// Bayrak durum kontrolü yapmak için kullanılan fonksiyondur. 
USART_FlagStatus_t USART_GetFlagStatus(USART_HandleTypedef_t *USART_Handle, uint16_t flagName)
{
	// SR registerinde benim gönderdiğim bayrağı kontrol et, veri var ise SET yok ise RESET'e çek 
	return ((USART_Handle->Instance->SR & flagName) ? USART_FLAG_SET : USART_FLAG_RESET);
}



/*
 * @brief	USART_InterruptHandler, Check the flag and register for interrupt
 *
 * @param	USART_Handle = User config structure
 *
 * @retval	void
 */
void USART_InterruptHandler(USART_HandleTypedef_t *USART_Handle)
{
	uint8_t flagValue = 0;
	uint8_t interruptValue = 0;

	flagValue = (uint8_t)((USART_Handle->Instance->SR >> 7U) & 0x1U);
	interruptValue = (uint8_t)((USART_Handle->Instance->CR1 >> 7U) & 0x1U);

	if(flagValue && interruptValue)
	{
		USART_Handle->TxISR_Function(USART_Handle);
	}

	flagValue = (uint8_t)((USART_Handle->Instance->SR >> 5U) & 0x1U);
	interruptValue = (uint8_t)((USART_Handle->Instance->CR1 >> 5U) & 0x1U);

	if(flagValue && interruptValue)
	{
		USART_Handle->RxISR_Function(USART_Handle);
	}
}
