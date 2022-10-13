# USART INTERRUPRT MOD

## USART_TRANSMIT_IT

### 1. USART2IRQ_Handler();
İnterrupt modu için main.c dosyalarında tanımlanan özel interrupt fonksiyonlarıdır. Kesme uygulanacak birimin fonksiyonlaru `Startup > startup_stm32f407vgtx.c` dosyasında tanımlıdırlar. 

### 2. USART_InterruprtHandler();
İnterrupt'ın gelip gelmediğini uygun register ve bayrağa bakarak kontrol edildlği fonksiyondur. Register ve bayrağa gelen veriler ile interrupt'ın türü belirlenir.  
Örneğin USART_Transmit mi USART_Receive mi burada karar verilir.    
USART2IRQ_Handler() fonksiyonunda çağırılırlar. 

### 3. USART_TransmitData_IT();
İnterrupt işleminin başladığı kısımdır. İşlem sırasında kullanılacak veriler global değişkenlere atanır ki tüm fonksiyonlar erişebilsin. Ardından interrupt'ı aktif hale getiriyoruz.
Böylece interrupt başlatıldı ve veriler işlenmeye hazır.                  
**INTERRUPT MODE -> ENABLE**                                                        
**INTERRUPT DATAS -> READY**              

### 4. USART_SendWith_IT();
USART_TransmitData_IT() fonksiyonundan gelen verilerin ne iş yapacağına karar verildiği yani verilerin işlendiği fonksiyondur. Örneğin tranmsit, receive işlemleri burada gerçekleşir.                           
**INTERRUPT DATAS -> DONE** 

### 5. closeUSART_ISRC();
 USART_SendWith_IT() fonksiyonunda işlemi biten verilerin sıfırlandığı ve interrupt'ın pasif hale getirildiği kısımdır.                        
**INTERRUPT MODE -> DISABLE** 
**INTERRUPT DATAS -> RESET**



## ÖZET
### 1. USART2IRQ_Handler(); -> PROGRAMA INTERRUPT BİLDİRİMİ YAPILDI                     
### 2. USART_InterruprtHandler(); -> INTERRUPT_MOD BELİRLENDİ             
### 3. USART_TransmitData_IT(); -> INTERRUPT AKTİF, VERİLER HAZIR            
### 4. USART_SendWith_IT(); -> VERİLER İŞLENİYOR...                 
### 5. closeUSART_ISRC(); -> VERİLER SIFIRLANDI, INTERRUPT PASİF                  


## KOD                    
### main.c               
```c
void USART2_IRQHandler()
{
 USART_InterruptHandler(&USART_Handle);
}

```

### USART.c
```c

/**********************************************************************************************/

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
}

/**********************************************************************************************/

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

/**********************************************************************************************/

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
		closeUSART_ISR(USART_Handle);
	}
}

/**********************************************************************************************/

static void closeUSART_ISR(USART_HandleTypedef_t *USART_Handle)
{
	USART_Handle->TxBufferSize = 0;
	USART_Handle->pTxBuffer = NULL;
	USART_Handle->TxStatus = USART_BUS_FREE;

	USART_Handle->Instance->CR1 &= ~(0X1U << USART_CR1_TxEIE);
}

/**********************************************************************************************/
```






















