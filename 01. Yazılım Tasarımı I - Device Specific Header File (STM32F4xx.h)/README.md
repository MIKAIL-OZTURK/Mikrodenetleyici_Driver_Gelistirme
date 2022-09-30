# Device Specific Header File (STM32F407xx.h)   
Mikrodenetleyici için temel tanımlamaların yapıldığı dosyadır. 

- Temel adres değerleri arasında boşluk olamaz.
- Temel adres değerleri _unsiged long_ olmalıdır. Bunun için temel adres değerleri **UL** ile bitmelidir. 
- Tüm çevre birimlerinin registerları `volatile` olmak zorundadır.   
- Çevresel birimlerin registerları tanımlanırken doğru konfigürasyon için sırası ile tanımlanmalıdır. 

## :dart: Yol Haritası      
## **1. Diğer Tanımlamalar**
```c
#include <stdint.h>							// uint8_t, uint32_t değişken türleri için kullanılır.

#define __IO volatile							// // volatile keywordu yerine standart olan __IO keyword'un kullanılması
#define SET_BIT(REG, BIT)			((REG) |=  (BIT))	// Bt Set
#define CLEAR_BIT(REG, BIT)			((REG) &= ~(BIT))	// Bit Reset
#define READ_BIT(REG, BIT			((REG) &   (BIT))	// Read Bit 
#define UNUSED(x)				(void)x			// Derleyici "Bu fonksiyon kullanılmıyor" hatasını vermesin diye kullanılan fonksiyon.

typedef enum								// Register ENABLE veya DISABLE yapabilmek için kullanılan yapı
{
	DISABLE = 0x0U,
	ENABLE = !DISABLE,
}FunctionalState_t;
```

## **2. Temel Adres Tanımlamaları**  
#### 1. Hafıza Birimlerinin Temel Adresleri     
- FLASH & SRAM1 & SRAM2          
```c
ÖRNEK | #define SRAM2_BASE_ADDR		(0x2001C000UL)
```
#### 2. Veri Yollarının Temel Adresleri 
- APB1 & APB2 & AHB1 & AHB2       
```c
ÖRNEK | #define AHB1_BASE_ADDR		(PERIPH_BASE_ADDR + 0x00020000UL)
```   
#### 3. Çevresel Birimlerin Temel Adresleri 
```c
ÖRNEK | #define GPIOC_BASE_ADDR		(AHB1_BASE_ADDR + 0x0800UL)	
```
#### 4. Çevresel Birimleri Registerler Yapıları 
```c
ÖRNEK | GPIO Peripherals Structure Definitions
typedef struct
{
	__IO uint32_t MODER;		/*!< GPIO port mode register 			Address Offset = 0x00 */
	__IO uint32_t OTYPER;		/*!< GPIO port output type register 		Address Offset = 0x04 */
	....
}GPIO_TypeDef_t;
```    
#### 5. Çevresel Birimleri Registerler Yapıları erişmek için Port Tanımlaması 
```c
ÖRNEK | #define GPIOC			((GPIO_TypeDef_t *)(GPIOC_BASE_ADDR)))	 // GPIOC->MODER 
```

#### 6. Clock Hatları için Bit Tanımlamaları 
```c
ÖRNEK |                      
#define RCC_AHB1ENR_GPIOCEN_Pos				(2U)					/*!< RCC AHB1ENR register GPIOCEN Bit Position      */
#define RCC_AHB1ENR_GPIOCEN_Msk				(0x1 << RCC_AHB1ENR_GPIOCEN_Pos)	/*!< RCC AHB1ENR register GPIOCEN Bit Mask 	    */
#define RCC_AHB1ENR_GPIOCEN				RCC_AHB1ENR_GPIOCEN_Msk			/*!< RCC AHB1ENR register GPIOCEN Bit Macro	    */
```

#### 7. Çevresel Birim Fonksiyonlarının Bulunuduğu Kütüphane Dosyaları 
```c
#include "RCC.h"
#include "GPIO.h"
#include "EXTI.h"
```       
---  
## :bookmark_tabs: Kaynaklar 
### Temel Adreslerin Tanımlanması 
- Hafıza Birimlerinin Temel Adresleri için: **SRAM**  ``` Reference Manual -> 2. Memory and bus architecture -> Embedded SRAM ```
- Hafıza Birimlerinin Temel Adresleri için: **FLASH** ``` Reference Manual -> 3. Embedded Flash memory interface ```
- Veri Yollarının Temel Adresleri için: **AHB1,ABP1...** ``` Reference Manual -> 2. Memory and bus architecture -> Memory Map ```
- Çevre Birimlerinin Temel Adresleri için: **TIM1, UART4...** ``` Reference Manual -> 2. Memory and bus architecture -> Memory Map ```
### Çevresel Birim Yapılarının Tanımlanması 
- **GPIO** Biriminin Yapıları için: ``` Reference Manual -> General purpose I/Os (GPIO) -> GPIO Registers Map ``` 
- **RCC** Biriminin Yapıları İçin:  ``` Reference Manual -> Reset and Clock Control for STM32F407xx -> RCC Registers -> RCC Register Map ```
