# SPI INTERRUPRT MOD

## USART_TRANSMIT_IT

### 1. SPI1_IRQHandler();
İnterrupt modu için main.c dosyalarında tanımlanan özel interrupt fonksiyonlarıdır. Kesme uygulanacak birimin fonksiyonlaru `Startup > startup_stm32f407vgtx.c` dosyasında tanımlıdırlar. 

### 2. SPI_InterruptHandler();
İnterrupt'ın gelip gelmediğini uygun register ve bayrağa bakarak kontrol edildlği fonksiyondur. Register ve bayrağa gelen veriler ile interrupt'ın türü belirlenir.  
Örneğin SPI_Transmit mi SPI_Receive mi burada karar verilir.    
SPI1_IRQHandler() fonksiyonunda çağırılırlar. 

### 3. USART_TransmitData_IT();
İnterrupt işleminin başladığı kısımdır. İşlem sırasında kullanılacak veriler global değişkenlere atanır ki tüm fonksiyonlar erişebilsin. Ardından interrupt'ı aktif hale getiriyoruz.
Böylece interrupt başlatıldı ve veriler işlenmeye hazır.                  
**INTERRUPT MODE -> ENABLE**                                                        
**INTERRUPT DATAS -> READY**              

### 4. SPI_TransmitHelper_16Bits(); ~ SPI_TransmitHelper_8Bits();
USART_TransmitData_IT() fonksiyonundan gelen verilerin ne iş yapacağına karar verildiği yani verilerin işlendiği fonksiyondur. Örneğin tranmsit, receive işlemleri burada gerçekleşir.                           
**INTERRUPT DATAS -> DONE** 

### 5. SPI_CloseISR_TX();
 USART_SendWith_IT() fonksiyonunda işlemi biten verilerin sıfırlandığı ve interrupt'ın pasif hale getirildiği kısımdır.                        
**INTERRUPT MODE -> DISABLE** 
**INTERRUPT DATAS -> RESET**



## ÖZET
### 1. USART2IRQ_Handler(); -> PROGRAMA INTERRUPT BİLDİRİMİ YAPILDI                     
### 2. USART_InterruprtHandler(); -> INTERRUPT_MOD BELİRLENDİ             
### 3. USART_TransmitData_IT(); -> INTERRUPT AKTİF, VERİLER HAZIR            
### 4. SPI_TransmitHelper_8Bits(); -> VERİLER İŞLENİYOR...                 
### 5. SPI_CloseISR_TX(); -> VERİLER SIFIRLANDI, INTERRUPT PASİF                  


## SPI.c
```c

/**********************************************************************************************/

void SPI_InterruptHandler(SPI_HandleTypeDef_t *SPI_Handle)
{
	uint8_t interruptSource = 0;
	uint8_t interruptFlag = 0;

	interruptSource = SPI_Handle->Instance->CR2 & (0x1U << SPI_CR2_TXEIE);
	interruptFlag = SPI_Handle->Instance->SR & (0x1U << SPI_SR_TxE);

	if((interruptSource != 0 ) && (interruptFlag != 0))
	{
		SPI_Handle->TxISRFunction(SPI_Handle);
	}

	interruptSource = SPI_Handle->Instance->CR2 & (0x1U << SPI_CR2_RXNEIE);
	interruptFlag = SPI_Handle->Instance->SR & (0x1U << SPI_SR_RxNE);

	if((interruptSource != 0 ) && (interruptFlag != 0))
	{
		SPI_Handle->RxISRFunction(SPI_Handle);
	}
}

/**********************************************************************************************/

void SPI_TransmitData_IT(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pData, uint16_t sizeOfData)
{
	SPI_BusStatus_t busState = SPI_Handle->busStateTx;

	if(busState != SPI_BUS_BUSY_TX)
	{
		SPI_Handle->pTxDataAddr = (uint8_t*)pData;
		SPI_Handle->TxDataSize = (uint16_t)sizeOfData;
		SPI_Handle->busStateTx = SPI_BUS_BUSY_TX;

		if(SPI_Handle->Instance->CR1 & (0x1U << SPI_CR1_DFF))
		{
			SPI_Handle->TxISRFunction = SPI_TransmitHelper_16Bits;
		}
		else
		{
			SPI_Handle->TxISRFunction = SPI_TransmitHelper_8Bits;
		}
		SPI_Handle->Instance->CR2 |= (0x1U << SPI_CR2_TXEIE);
	}
}

/**********************************************************************************************/

static void SPI_TransmitHelper_16Bits(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle->Instance->DR = *((uint16_t*)(SPI_Handle->pTxDataAddr));
	SPI_Handle->pTxDataAddr += sizeof(uint16_t);
	SPI_Handle->TxDataSize -= 2;
	if(SPI_Handle->TxDataSize == 0)
	{
		SPI_CloseISR_TX(SPI_Handle);
	}
}

static void SPI_TransmitHelper_8Bits(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle->Instance->DR = *((uint8_t*)(SPI_Handle->pTxDataAddr));
	SPI_Handle->pTxDataAddr += sizeof(uint8_t);
	SPI_Handle->TxDataSize--;

	if(SPI_Handle->TxDataSize == 0)
	{
		SPI_CloseISR_TX(SPI_Handle);
	}
}

/**********************************************************************************************/

static void SPI_CloseISR_TX(SPI_HandleTypeDef_t *SPI_Handle)
{
	SPI_Handle->Instance->CR2 &= ~(0x1U << SPI_CR2_TXEIE);
	SPI_Handle->TxDataSize = 0;
	SPI_Handle->pTxDataAddr = NULL;
	SPI_Handle->busStateTx = SPI_BUS_FREE;
}

/**********************************************************************************************/
```
