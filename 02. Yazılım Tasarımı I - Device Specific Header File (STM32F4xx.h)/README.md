# Device Specific Header File (STM32F407xx.h)   
```c
#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

//Device specific header file dosyasındaki tüm tanımalamalar bu arada olmalıdıır ! 

#endif /* INC_STM32F407XX_H_ */
```

## :dart: Yol Haritası      
### **1. Temel Adres Tanımlamaları**  
- Temel adres değerleri arasında boşluk olamaz.
- Temel adres değerleri _unsiged long_ olmalıdır. Bunun için temel adres değerleri **UL** ile bitmelidir. 
#### 1. Hafıza Birimlerinin Temel Adresleri       
```c
/* Memory Base Address	*/
#define	FLASH_BASE_ADDR 					(0x08000000UL)	/* Flash Base Address (up to 1 MB) 	 */
#define SRAM1_BASE_ADDR						(0x20000000UL)	/* SRAM1 Base Address (112 KB) 		 */
#define SRAM2_BASE_ADDR						(0x2001C000UL)	/* SRAM2 Base Address (16 KB) 		 */
```    

#### 2. Veri Yollarının Temel Adresleri    
```c
/* Peripheral Base Address */
#define PERIPH_BASE_ADDR					(0x40000000UL)				/* Base Address for All Peripherals   	    */
#define APB1_BASE_ADDR						(PERIPH_BASE_ADDR + 0x00000000UL)	/* APB1 Bus Domain Base Address 	    */
#define APB2_BASE_ADDR						(PERIPH_BASE_ADDR + 0x00010000UL)	/* APB2 Bus Domain Base Address 	    */
#define AHB1_BASE_ADDR						(PERIPH_BASE_ADDR + 0x00020000UL)	/* AHB1 Bus Domain Base Address 	    */
#define AHB2_BASE_ADDR						(PERIPH_BASE_ADDR + 0x10000000UL)	/* AHB2 Bus Domain Base Address 	    */
```

#### 3. Çevre Birimlerinin Temel Adresleri    
```c
/* APB1 Peripherals Base Address */
#define TIM2_BASE_ADDR						(APB1_BASE_ADDR + 0x0000UL) 	/* Timer 2 Base Address 	*/
#define TIM3_BASE_ADDR						(APB1_BASE_ADDR + 0x0400UL)	/* Timer 3 Base Address 	*/
#define TIM4_BASE_ADDR						(APB1_BASE_ADDR + 0x0800UL) 	/* Timer 4 Base Address 	*/
#define TIM5_BASE_ADDR						(APB1_BASE_ADDR + 0x0C00UL) 	/* Timer 5 Base Address 	*/
#define SPI2_BASE_ADDR						(APB1_BASE_ADDR + 0x3800UL) 	/* SPI 2 Base Address   	*/
#define SPI3_BASE_ADDR						(APB1_BASE_ADDR + 0x3C00UL) 	/* SPI 3 Base Address   	*/
#define USART2_BASE_ADDR					(APB1_BASE_ADDR + 0x4400UL) 	/* USART 2 Base Address 	*/
#define USART3_BASE_ADDR					(APB1_BASE_ADDR + 0x4800UL)	/* USART 3 Base Address 	*/
#define UART4_BASE_ADDR						(APB1_BASE_ADDR + 0x4C00UL)	/* UART 4 Base Address		*/
#define UART5_BASE_ADDR						(APB1_BASE_ADDR + 0x5000UL) 	/* UART 5 Base Address  	*/
#define I2C1_BASE_ADDR						(APB1_BASE_ADDR + 0x5400UL) 	/* I2C 1 Base Address   	*/
#define I2C2_BASE_ADDR						(APB1_BASE_ADDR + 0x5800UL) 	/* I2C 2 Base Address   	*/
#define I2C3_BASE_ADDR						(APB1_BASE_ADDR + 0x5C00UL) 	/* I2C 3 Base Address   	*/

/* APB2 Peripherals Base Address */
#define TIM1_BASE_ADDR						(APB2_BASE_ADDR + 0x0000UL)		/* Timer 1 Base Address 	*/
#define TIM8_BASE_ADDR						(APB2_BASE_ADDR + 0x0400UL)		/* Timer 8 Base Address 	*/
#define USART1_BASE_ADDR					(APB2_BASE_ADDR + 0x1000UL)		/* USART 1 Base Address 	*/
#define USART6_BASE_ADDR					(APB2_BASE_ADDR + 0x1400UL)		/* USART 6 Base Address 	*/
#define SPI1_BASE_ADDR						(APB2_BASE_ADDR + 0x3000UL)		/* SPI 1 Base Address		*/
#define SPI4_BASE_ADDR						(APB2_BASE_ADDR + 0x3400UL)		/* SPI 4 Base Address 		*/
#define SYSCFG_BASE_ADDR					(APB2_BASE_ADDR + 0x3800UL)		/* SYSCFG Base Address		*/
#define EXTI_BASE_ADDR						(APB2_BASE_ADDR + 0x3C00UL)		/* EXTI Base Address		*/

/* AHB1 Peripherals Base Address */
#define GPIOA_BASE_ADDR						(AHB1_BASE_ADDR + 0x0000UL)		/* GPIOA Base Address 		*/
#define GPIOB_BASE_ADDR						(AHB1_BASE_ADDR + 0x0400UL)		/* GPIOB Base Address 		*/
#define GPIOC_BASE_ADDR						(AHB1_BASE_ADDR + 0x0800UL)		/* GPIOC Base Address 		*/
#define GPIOD_BASE_ADDR						(AHB1_BASE_ADDR + 0x0C00UL)		/* GPIOD Base Address 		*/
#define GPIOE_BASE_ADDR						(AHB1_BASE_ADDR + 0x1000UL)		/* GPIOE Base Address 		*/
#define RCC_BASE_ADDR						(AHB1_BASE_ADDR + 0x3800UL)		/* RCC Base Address		*/
```

### **2. Çevre Birimleri Yapılarının Tanımlanması**    
- Tüm çevre birimlerinin registerları `volatile` olmak zorundadır.   
- Çevresel birimlerin registerları tanımlanırken doğru konfigürasyon için sırası ile tanımlanmalıdır. 
#### 1. GPIO (General Purpose I/O)    
```c
#include <stdint.h>

#define __IO volatile

/* Peripherals Structure Definitions */
typedef struct
{
	__IO uint32_t MODER;				/*!< GPIO port mode register 	        	Address Offset = 0x00 */
	__IO uint32_t OTYPER;				/*!< GPIO port output type register 		Address Offset = 0x04 */
	__IO uint32_t OSPEEDR;				/*!< GPIO port output speed register 		Address Offset = 0x08 */
	__IO uint32_t PUPDR;				/*!< GPIO port pull-up/pull down register 	Address Offset = 0x0C */
	__IO uint32_t IDR;				/*!< GPIO port input data register 		Address Offset = 0x10 */
	__IO uint32_t ODR;				/*!< GPIO port output data register 		Address Offset = 0x14 */
	__IO uint32_t BSRR;				/*!< GPIO port bit set/reset register 		Address Offset = 0x18 */
	__IO uint32_t LCKR;				/*!< GPIO port configuration lock register 	Address Offset = 0x1C */
	__IO uint32_t AFR[2];				/*!< GPIO alternate function 		        Address Offset = 0x20 */
}GPIO_TypeDef_t;

#define GPIOA								((GPIO_TypeDef_t *)(GPIOA_BASE_ADDR))
#define GPIOB								((GPIO_TypeDef_t *)(GPIOB_BASE_ADDR))
#define GPIOC								((GPIO_TypeDef_t *)(GPIOC_BASE_ADDR))
#define GPIOD								((GPIO_TypeDef_t *)(GPIOD_BASE_ADDR))
#define GPIOE								((GPIO_TypeDef_t *)(GPIOE_BASE_ADDR))
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
