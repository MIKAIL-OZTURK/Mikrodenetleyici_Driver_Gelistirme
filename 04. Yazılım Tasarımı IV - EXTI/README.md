# EXTI (External Interrupt Controller)
Önceliği yüksek işlerin mikrodenetleyici tarafından ana program akışını keserek yapılmasına interrupt denir. Kesme birimini kullanabilmek için üç birimi konfigüre etmek gerekir. 
1. SYSCFG : Kesme işleminin hangi port ve pinde gerçekleşeceğini bildirir. Mikrodenetleyicide bulunur.             
2. EXTI : Maskelemeyi kaldırır ve registerların alacağı değeleri(düşen kenar, yükselen kenar, interrupt mode vs.) ayarlar. Mikrodenetleyicide bulunur.          
3. NVIC : Konfigüre edilen kesme işlemini aktfi hale getirir. Mikroişlemcide bulunur.                    


**Interrupt Service Routine (ISR)**                     
İşlemci interrupt'a girdiğinde yapması gerekenleri söyleyen fonksiyondur. Kaynak: STM32CubeİDE -> "Startup" Klasörü -> "startup_stm32f407vgtx.s" dosyası     
> ISR fonksiyonları kısa tutulmalıdır ve içerisinde döngü kullanılmamalıdır.                  
```c
void EXTI0_IRQHandler()
{
	if(EXTI->PR & 0x1)
	{
		EXTI->PR |= (0x1U << 0U);
		GPIO_WritePin(GPIOD, GPIO_PIN_ALL, GPIO_Pin_Set);
	}
}
```

### EXTI.h
1. Registerlerin Aldığı Değerlerin Tanımlanması
2. Yapılar
3. Fonksiyon Prototipleri ( API's ) 

### EXTI.c
1. Fonksiyon Tanımlamaları ( API's )

## SYSCFG Registers
**1. SYSCFG external interrupt configuration register 1 (SYSCFG_EXTICR1)**                              
**2. SYSCFG external interrupt configuration register 2 (SYSCFG_EXTICR2)**                                           
**3. SYSCFG external interrupt configuration register 3 (SYSCFG_EXTICR3)**                                            
**4. SYSCFG external interrupt configuration register 4 (SYSCFG_EXTICR4)**                                             
                                 
![Ekran görüntüsü 2022-09-30 160823](https://user-images.githubusercontent.com/75627147/193276552-cd1161ad-3eb8-4af6-85ef-50b5f221c554.png)         
Pin ve port seçilir. Örneğin C portu 2. pin için -> EXTI2'ye 0010 yazılır.                  

## EXTI Registers 
**1. Interrupt mask register (EXTI_IMR)**                     
İnterrupt maskelemelerini kaldırır.                   
**2. Event mask register (EXTI_EMR)**                     
Event maskelemelerini kaldırır.              
**3. Rising trigger selection register (EXTI_RTSR)**        
Geçerli pini yükselen kenar olarak ayarlar. ( BUTON ON -> SET )                       
**4. Falling trigger selection register (EXTI_FTSR)**                         
Geçerli pini düşen kenar olarak ayarlar. ( BUTON ON -> RESET )                                        
**5. Software interrupt event register (EXTI_SWIER)**                                                                   
Yazılımsal olarak kesme üretir.                      
**6. Pending register (EXTI_PR)**                                                                
İnterruprt gerçekleşip gerçekleşmediğini kontrol eden registerdir. 
```c
void EXTI0_IRQHandler()
{
	if(EXTI->PR & 0x1)                    // "PR register" ve "0. bit" ve(&) işlemi sonucu 1 ise interrupt başarılıdır. 
	{
		EXTI->PR |= (0x1U << 0U);           
		GPIO_WritePin(GPIOD, GPIO_PIN_ALL, GPIO_Pin_Set);
	}
}
```

## NVIC Registers


### ÖRNEK
```c
static void GPIO_ButtonInterruptConfig()
{
	EXTI_InitTypeDef_t EXTI_Struct = { 0 };				// Lokal değişken olduğu için tüm değerler sıfırlandı. 
	
	/* SYSCFG Config --> */
	RCC_SYSCFG_CLK_ENABLE();					// Clock hattı aktfi edildi.
	EXTI_LineConfig(EXTI_PortSource_GPIOA, EXTI_LineSource_0);	// Port ve pin numaraları belirlendi. A0 

	/* EXTI Config --> */ 
	EXTI_Struct.EXTI_LineCmd = ENABLE;				// İnterrupt veya event mod aktif edildi
	EXTI_Struct.EXTI_LineNumber = EXTI_LineSource_0;		// Port seçimi yapıldı.
	EXTI_Struct.EXTI_Mode = EXTI_MODE_Interrupt;			// İnterrupt modu ayarlandı.
	EXTI_Struct.TriggerSelection = EXTI_Trigger_Rising;		// Yükselen kenar ayarlandı.
	EXTI_Init(&EXTI_Struct);					// Konfigüre ettiğimiz ayarları gerekli pine yükledik. 

	/* NVIC Config --> */
	NVIC_EnableInterrupt(EXTI0_IRQNumber);				// Interruprt'a izim verdik. (EXTI0 --> A portu 0. pin)
}
```
---

# API's            
### void EXTI_Init(EXTI_InitTypeDef_t *EXTI_InitStruct)
Geçerli yapıya göre konfigürasyon ayarı yapar.                            
***EXTI_InitTypeDef_t *EXTI_InitStruct***: Konfigürasyon ayarlarının yapıldığı yapıdır.           
                         
```c
void EXTI_Init(EXTI_InitTypeDef_t *EXTI_InitStruct)
{
	uint32_t tempValue = 0;
	tempValue = (uint32_t)EXTI_BASE_ADDR;
	EXTI->IMR &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);
	EXTI->EMR &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);

	if(EXTI_InitStruct->EXTI_LineCmd != DISABLE)
	{
		tempValue += EXTI_InitStruct->EXTI_Mode;
		*((__IO uint32_t *)tempValue ) |= (0x1U << EXTI_InitStruct->EXTI_LineNumber);
		tempValue = (uint32_t)EXTI_BASE_ADDR;
		EXTI->RTSR &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);
		EXTI->FTSR &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);

		if(EXTI_InitStruct->TriggerSelection == EXTI_Trigger_RF)
		{
			EXTI->RTSR |= (0x1U << EXTI_InitStruct->EXTI_LineNumber);
			EXTI->FTSR |= (0x1U << EXTI_InitStruct->EXTI_LineNumber);
		}
		else
		{
			tempValue += EXTI_InitStruct->TriggerSelection;
			*((__IO uint32_t *)tempValue ) |= (0x1U << EXTI_InitStruct->EXTI_LineNumber);
		}
	}
	else
	{
		tempValue = (uint32_t)EXTI_BASE_ADDR;
		tempValue += EXTI_InitStruct->EXTI_Mode;
		*((__IO uint32_t *)tempValue ) &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);
	}
}
```

### void EXTI_LineConfig(uint8_t PortSource, uint8_t EXTI_LineSource)                           


```c
void EXTI_LineConfig(uint8_t PortSource, uint8_t EXTI_LineSource)
{
	uint32_t tempValue = 0;

	tempValue = SYSCFG->EXTI_CR[EXTI_LineSource >> 2U];
	tempValue &= ~(0xFU << (EXTI_LineSource & 0x3U) * 4);
	tempValue = (PortSource << (EXTI_LineSource & 0x3U) * 4);
	SYSCFG->EXTI_CR[EXTI_LineSource >> 2U] = tempValue;
}
```

### void NVIC_EnableInterrupt(IRQNumber_TypeDef_t IRQNumber)                 

```c
void NVIC_EnableInterrupt(IRQNumber_TypeDef_t IRQNumber)
{
	uint32_t tempValue = 0;
	tempValue = *((IRQNumber >> 5U) + NVIC_ISER0);
	tempValue &= ~(0x1U << (IRQNumber & 0x1FU));
	tempValue |=  (0x1U << (IRQNumber & 0x1FU));
	*((IRQNumber >> 5U) + NVIC_ISER0) = tempValue;
}
```

## Kaynaklar 
- **SYSCFG** : STM32F407 User(Referance) Manual -> 9. System configuration controller (SYSCFG) 
- **EXTI** : STM32F407 User(Referance) Manual -> 12. Interrupts and events 
- **NVIC** : Cortex -M4 Devices Generic User Guide -> Cortex-M4 Peripherals -> Nested Vectored Interrupt Controller

