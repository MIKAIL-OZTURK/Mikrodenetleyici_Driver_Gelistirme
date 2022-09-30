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



## Kaynaklar 
- **SYSCFG** : STM32F407 User(Referance) Manual -> 9. System configuration controller (SYSCFG) 
- **EXTI** : STM32F407 User(Referance) Manual -> 12. Interrupts and events 
- **NVIC** : Cortex -M4 Devices Generic User Guide -> Cortex-M4 Peripherals -> Nested Vectored Interrupt Controller

