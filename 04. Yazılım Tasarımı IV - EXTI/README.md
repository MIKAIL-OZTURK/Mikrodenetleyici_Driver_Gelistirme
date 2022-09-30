# EXTI (External Interrupt Controller)
Önceliği yüksek işlerin mikrodenetleyici tarafından ana program akışını keserek yapılmasına interrupt denir. Kesme birimini kullanabilmek için üç birimi konfigüre etmek gerekir. 
1. SYSCFG : Kesme işleminin hangi port ve pinde gerçekleşeceğini bildirir. Mikrodenetleyicide bulunur.             
2. EXTI : Maskelemeyi kaldırır ve registerların alacağı değeleri(düşen kenar, yükselen kenar, interrupt mode vs.) ayarlar. Mikrodenetleyicide bulunur.          
3. NVIC : Konfigüre edilen kesme işlemini aktfi hale getirir. Mikroişlemcide bulunur. 

**Interrupt Service Routine (ISR)**
İşlemci interrupt'a girdiğinde yapması gerekenleri söyleyen fonksiyondur. Kaynak: STM32CubeİDE -> "Startup" Klasörü -> "startup_stm32f407vgtx.s" dosyası     
> ISR fonksiyonları kısa tutulmalıdır ve içerisinde döngü kullanılmamalıdır. 

### EXTI.h
1. Registerlerin Aldığı Değerlerin Tanımlanması
2. Yapılar
3. Fonksiyon Prototipleri ( API's ) 

### EXTI.c
1. Fonksiyon Tanımlamaları ( API's )

## SYSCFG Registers
## EXTI Registers 
## NVIC Registers
