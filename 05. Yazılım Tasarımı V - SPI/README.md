# SPI (Serial Peripherals Interface)
Çevresel cihazlarla veya diğer mikrodenetleyicilerle veri transferi sağlayan yazılım/donanım tabanlı seri iletişim protokolüdür. Hız ile alakalı işlemlerde SPI tercih edilir.                                   
![350px-SPI_three_slaves svg](https://user-images.githubusercontent.com/75627147/195082417-567dcf47-c0a3-46b9-8ddc-ff9f2544709d.png)


- Slave cihaz sadece "SS(Slave Select) ~ CS(Chip Select)" olarak isimlendirilen sinyal ile aktif hale getirilir ve bu sinyali sadece master cihaz üretir.         
- Veri transferi seri bir şekilde bit bit yapılır ve bu sinyaller iki sinyal ile taşınır:              
  - MOSI(Master Output Slave Input): Master -> Slave                      
  - MISO(Master Input Slave Output): Slave -> Master                             
- SCLK sinyali senkron olarak MOSI ve MISO sinyallerinin taşınmasında kullanılır. Sinyal master cihaz tarafından üretirilir.


## SPI Registers 
1. **SPI control register 1 (SPI_CR1)**
- BIDIMODE: İletişimin çift yönlü(Full Duplex) veya tek yönlü(Half Duplex) olacağını belirler.         
- BIDIOE: İletişim tek taraflı(Half Duplex) ise veri alıcanak mı ,okunacak mı olduğunu belirler.  
- CRCEN: (Hardware CRC calculation enable)          
- CRCNEXT: (CRC transfer next)             
- DFF: Gönderilecek/alınacak verinin 8 veya 16 bit olmasını belirler. (Data Frame Format)                
- RXONLY: İletişim çift yönlü(Full Duplex) ise veri veri hem aınıp hem okunacak mı yoksa sadece okunaak mı olduğunu belirler. (Receive Only)                 
- SSM: SSM biti ayarlandığında, NSS pin girişi SSI bitinden gelen değer ile değiştirilir. (Software Slave Management)                     
- SSI: Bu bit, yalnızca SSM biti ayarlandığında bir etkiye sahiptir. Bu bitin değeri NSS pinine zorlanır ve NSS pininin IO değeri yoksayılır. (Internal slave select)   
- LSBFIRST: İletişim yüksek değerlikli(MSB) veya düşük değerlikli(LSB) bitten başlayacağını belirler. (8 bit ise 7. bit MSB, 0.bit LSB) (Frame Format)
- SPE: SPI'yı aktif veya pasif duruma getirir. (SPI Enable)
- BR: İletişim frekansının bölme oranını belirler. (Baud Rade)                    
- MSTR: Cihazın Master veya Slave olmasını belirler. (Master Selection)                   
- CPOL: Clock hattının başlangıç ve bitiş değerlerini ayarlar. CPOL 0 ise Clock lojik 0'dan başlar ve biter. CPOL 1 ise Clock lojik 1'den başlar ve biter. (Clock 
Polarity )                                              
- CPHA: Ne zaman örnekleme alınacağını/okunacağını belirler. CPHA 0 ise yükseelen kenarda, CPHA 1 ise düşen kenarda veri alınır/okunur. (Clock Phase)                   

2. **SPI control register 2 (SPI_CR2)**                                
3. **SPI status register (SPI_SR)**  -  Bayrakların(Flags) bulunduğu registerdir.                
- RXNE: Veri okunuyor mu okunmuyor mu bayrağıdıır. (Receive buffer not empty)           
- TXE: Veri alınıyor mu alnımıyor mu bayrağıdır. (Transmit buffer empty)              
- CHSIDE: (Channel Side)...                    
- UDR: (Underrun Flag)...                
- CRERR: (CRC Error Flag)...
- MODF: (Mode Fault)...            
- OVR: (Overrun Flag)...            
- BSY: SPI'ın meşgul mu değil mi bayrağıdır. (Busy Flag)                   
- FRE: (Frame Format Error)                  

4. **SPI data register (SPI_DR)** - Seri iletişim sonunda elde edilen veriyi barındıran registerdir.                                              
5. **SPI CRC polynomial register (SPI_CRCPR)**                                
6. **SPI RX CRC register (SPI_RXCRCR)**                                  
7. **SPI TX CRC register (SPI_TXCRCR)**                            
8. **SPI_I2S configuration register (SPI_I2SCFGR)**                   
9. **SPI_I2S prescaler register (SPI_I2SPR)**                                
                  

# API's
## void SPI_Init(SPI_HandleTypeDef_t *SPI_Handle);
SPI çevresel birimini konfigüre etmek için kullanılır.  
***SPI_HandleTypeDef_t *SPI_Handle***: Kullanıcının ilgili yapı ve registere erişmek için kullandığı yapıdır.            
```c
void SPI_Init(SPI_HandleTypeDef_t *SPI_Handle)
{
	uint32_t tempValue = 0;

	tempValue = SPI_Handle->Instance->CR1;

	tempValue |= (SPI_Handle->Init.BaudRate) | (SPI_Handle->Init.CPHA) 			    | (SPI_Handle->Init.CPOL) 		  | (SPI_Handle->Init.DFF_Format) |	\
				       (SPI_Handle->Init.Mode) 	   | (SPI_Handle->Init.FrameFormat) 	| (SPI_Handle->Init.BusConfig)  | (SPI_Handle->Init.SSM_Cmd);

	SPI_Handle->Instance->CR1 = tempValue;
  // 1. Doğrudan registerler üzerinde asla işlem yapmıyoruz. Konfigurasyon ayarları için, kullanılan registeri bu geçici bir değişkene atıyoruz. 
  // 2. Hangi konfigürasyon ayarlarını kullanıcı kullanacak, bunun için istere göre(opsiyonel) registerleri seçiyoruz.
  // 3. Tüm konfigürasyon ayarları kullanıcıya sunuldu, artık SPI bu konfigürasyon ayarlarına göre çalışabilir. (SPI_Handle->Instance->CR1 = tempValue;)
}
```

## void SPI_PeriphCmd(SPI_HandleTypeDef_t *SPI_Handle, FunctionalState_t stateofSPı);
SPI çevresel birimi aktif ya da pasif eden fonksiyondur.              
***SPI_HandleTypeDef_t *SPI_Handle***: Kullanıcının ilgili yapı ve registere erişmek için kullandığı yapıdır.             
```c
void SPI_PeriphCmd(SPI_HandleTypeDef_t *SPI_Handle, FunctionalState_t stateofSPı)
{
	if(stateofSPı == ENABLE)
	{
		SPI_Handle->Instance->CR1 |= (0x1U << SPI_CR1_SPE);
	}
	else
	{
		SPI_Handle->Instance->CR1 &= ~(0x1U << SPI_CR1_SPE);
	}
  // SPI->CR1->SPE registeri 1 olunca SPI aktif, 0 olunca SPI pasif konumdadır.
  // SPE: SPI Enable
}
```

## SPI_FlagStatus_t SPI_GetFlagStatus(SPI_HandleTypeDef_t *SPI_Handle, uint16_t SPI_Flag);
SPI biriminin bayraklarını SET ya da RESET eden fonksiyondur. 
```c
SPI_FlagStatus_t SPI_GetFlagStatus(SPI_HandleTypeDef_t *SPI_Handle, uint16_t SPI_Flag)
{
// SR: Status Register - SPI bayraklarının bulunduğu registerdir. 
	return (SPI_Handle->Instance->SR & SPI_Flag) ? SPI_FLAG_SET : SPI_FLAG_RESET;
}
//  
```

## void SPI_TransmitData(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pData, uint16_t sizeOfData);
Veri gönderme işleminin yapıldığı fonksiyondur.          
***SPI_HandleTypeDef_t *SPI_Handle***: Kullanıcının ilgili yapı ve registere erişmek için kullandığı yapıdır.    
***uint8_t *pData***: Gönderilecek veridir            
***uint16_t sizeOfData***: Gönderilecek verinin boyutudur              
```c
void SPI_TransmitData(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pData, uint16_t sizeOfData)
{
// SPI için gönderilecek veri 16 bit ya da 8 bit veri formatıyla gönderilebilir. İlk önce buna karar verilir. 
// Gönderilecek veri 16 bit veri formatıyla gönderilecek ise -->
	if(SPI_Handle->Init.DFF_Format == SPI_DFF_16BITS)
	{
		while(sizeOfData > 0) // Veri boyutu 0'dan büyük ise veri gönderilmeye devam ediyordur...
		{
			if(SPI_GetFlagStatus(SPI_Handle, SPI_TxE_FLAG)) // TXE: Transmit Buffer Empty 0 ise bayrak boş, 1 ise  bayrak dolu. Eğer bayrak 1 ise...
			{
				SPI_Handle->Instance->DR = *((uint16_t*)pData);
				pData += sizeof(uint16_t);
				sizeOfData -= 2;
			}
      // DR(Data Register)'na gelen veri yazılır. 
      // Verinin adresi artırılır. (Örndeğin 0x00 adresindeki veri gönderilir ardında 0x04, ardından 0x08...)
      // 16 bit gönderme yaptığımız için veriler iki bayt iki bayt gönderilir. Örndeğin 24 baytlık bir veri gönderdiğimizde her gönderim için 2 bayt
      // dosya gönderilir. Kalan 22 bayt hala işlemi beklemektedir. O yüzden veri boyutu 2 bayt 2 bayt azaltılır.
		}
	}
// Gönderilecek veri 8 bit veri formatıyla gönderilecek ise -->
	else /* (SPI_Handle->Init.DFF_Format == SPI_DFF_8BITS) */
	{
		while(sizeOfData > 0)
		{
			if(SPI_GetFlagStatus(SPI_Handle, SPI_TxE_FLAG))
			{
				SPI_Handle->Instance->DR = *pData;
				pData += sizeof(uint8_t);
				sizeOfData--;
			}
		}
	}
	while(SPI_GetFlagStatus(SPI_Handle, SPI_Busy_FLAG));		// Wait for busy flag
}
```

## void SPI_ReceiveData(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pBuffer, uint16_t sizeOfData);
Veri alma işleminin yapıldığı fonksiyondur.          
***SPI_HandleTypeDef_t *SPI_Handle***: Kullanıcının ilgili yapı ve registere erişmek için kullandığı yapıdır.    
***uint8_t *pData***: Alınacak veridir            
***uint16_t sizeOfData***: Alınacak verinin boyutudur  
```c
void SPI_ReceiveData(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pBuffer, uint16_t sizeOfData)
{
	if(SPI_Handle->Init.DFF_Format == SPI_DFF_16BITS)
	{
		while(sizeOfData > 0)
		{
			if(SPI_GetFlagStatus(SPI_Handle, SPI_RxNE_FLAG))
			{
				*((uint16_t*)pBuffer) = (uint16_t)SPI_Handle->Instance->DR;
				pBuffer += sizeof(uint16_t);
				sizeOfData -= 2;
			}
		}
	}
	else	/* (SPI_Handle->Init.DFF_Format == SPI_DFF_8BITS) */
	{
		while(sizeOfData > 0)
		{
			if(SPI_GetFlagStatus(SPI_Handle, SPI_RxNE_FLAG))
			{
				*(pBuffer) = *((__IO uint8_t*)&SPI_Handle->Instance->DR);
				pBuffer += sizeof(uint8_t);
				sizeOfData--;
			}
		}
	}
}
```




## void SPI_TransmitData_IT(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pData, uint16_t sizeOfData);
Veri gönderme işleminin interrupt ile yapıldığı fonksiyondur.          
***SPI_HandleTypeDef_t *SPI_Handle***: Kullanıcının ilgili yapı ve registere erişmek için kullandığı yapıdır.         
***uint8_t *pData***: Gönderilecek veridir               
***uint16_t sizeOfData***: Gönderilecek verinin boyutudur              
```c
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
```


## void SPI_ReceiveData_IT(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pBuffer, uint16_t sizeOfData);
Veri alma işleminin interrupt ile yapıldığı fonksiyondur.                              
***SPI_HandleTypeDef_t *SPI_Handle***: Kullanıcının ilgili yapı ve registere erişmek için kullandığı yapıdır.    
***uint8_t *pData***: Alınacak veridir            
***uint16_t sizeOfData***: Alınacak verinin boyutudur  
```c
void SPI_ReceiveData_IT(SPI_HandleTypeDef_t *SPI_Handle, uint8_t *pBuffer, uint16_t sizeOfData)
{
	SPI_BusStatus_t busState = SPI_Handle->busStateRx;

	if(busState != SPI_BUS_BUSY_RX)
	{
		SPI_Handle->pRxDataAddr = (uint8_t*)pBuffer;
		SPI_Handle->RxDataSize = (uint16_t)sizeOfData;
		SPI_Handle->busStateRx = SPI_BUS_BUSY_RX;

		if(SPI_Handle->Instance->CR1 & (0x1U << SPI_CR1_DFF))
		{
			SPI_Handle->RxISRFunction = SPI_ReceiverHelper_16Bits;
		}
		else
		{
			SPI_Handle->RxISRFunction = SPI_ReceiverHelper_8Bits;
		}
		SPI_Handle->Instance->CR2 |= (0x1U << SPI_CR2_RXNEIE);
	}
}
```


## void SPI_InterruptHandler(SPI_HandleTypeDef_t *SPI_Handle);
Interrupt için bayrak ve registerleri kontrol eden fonksiyondur.     
***SPI_HandleTypeDef_t *SPI_Handle***: Kullanıcının ilgili yapı ve registere erişmek için kullandığı yapıdır.        
```c
void SPI_InterruptHandler(SPI_HandleTypeDef_t *SPI_Handle)
{
	uint8_t interruptSource = 0;
	uint8_t interruptFlag = 0;

	interruptSource = SPI_Handle->Instance->CR2 & (0x1U << SPI_CR2_TXEIE);
	interruptFlag = SPI_Handle->Instance->SR & (0x1U << SPI_SR_TxE);
 
	if((interruptSource != 0 ) && (interruptFlag != 0))
	{
		SPI_Handle->TxISRFunction(SPI_Handle);
  // Registere ve bayrağa bakarak gelen interrupt'ın türünü belirledi. 
  // SPI_CR2_TXEIE registerine ve SPI_SR_TxE bayarğına bak, eğer ikisi de 1 ise interrupt'ın türü TX'dir
	}

	interruptSource = SPI_Handle->Instance->CR2 & (0x1U << SPI_CR2_RXNEIE);
	interruptFlag = SPI_Handle->Instance->SR & (0x1U << SPI_SR_RxNE);

	if((interruptSource != 0 ) && (interruptFlag != 0))
	{
		SPI_Handle->RxISRFunction(SPI_Handle);
  // Registere ve bayrağa bakarak gelen interrupt'ın türünü belirledi. 
  // SPI_CR2_RXNEIE registerine ve SPI_SR_RxNE bayarğına bak, eğer ikisi de 1 ise interrupt'ın türü RX'dir
	}
}
```
