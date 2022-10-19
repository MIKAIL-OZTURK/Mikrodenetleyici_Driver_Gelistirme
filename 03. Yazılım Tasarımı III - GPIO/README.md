# GPIO (General Purpose Input Output)

### GPIO.h
1. Registerlerin Aldığı Değerlerin Tanımlanması
2. Yapılar
3. Fonksiyon Prototipleri ( API's )       
### GPIO.c
1. Fonksiyon Tanımlamaları ( API's )

---
## GPIO Registers 
**1. GPIOx_MODER**
GPIO port mode register - Bir pinin modunu belirlemek için kullanılır.         
Modlar: Giriş modu(Input) - Genel amaçlı çıkış modu(General purpose output mode) - Anternatif Fonksiyon modu(Anternate 
function mode) - Analog mod(Analog mode)    

**2. GPIOx_OTYPER**
GPIO port output type register - Bir pinin çıkış türünü yapılandırmak için kullanılır.              
Çıkış Türleri: Output push-pull - Output open-drain           

**3. GPIOx_OSPPEDR**               
GPIO port output speed register - Lojik 1'den lojik 0'a çıkma veya lojik 0'dan lojik 1'e geçme hızını(~süresini) ayarlar. 
Çıkış Hızları: Low - Medium - High -  Very high speeds

**4. GPIOx_PUPDR**              
GPIO port pull-up/pull-down register - Çıkış türü _Output push-pull_ olarak yapılandırılan bir pini pull-up veya pull-down 
olarak ayarlar.
Modlar: No pull-up, pull-down - Pull up - Pull Down 

**5. GPIOx_IDR**            
GPIO port input data register - Bu bitler salt okunurdur(read only). Karşılık gelen G/Ç bağlantı noktasının giriş değerini 
içerirler.

**6. GPIOx_ODR**              
GPIO port output data register - Bu bitler yazılım tarafından okunabilir ve yazılabilir(read or write).

**7. GPIOx_BSRR**                   
GPIO port bit set/reset register - Bir biti set veya reset yapar.                          
Kullanımı: 0 - 15 Bitler için 1 -> Set, 0 -> İşlem Yok | 16 - 31 Bitler için 1 -> Reset 0 -> İşlem Yok            
![1](https://user-images.githubusercontent.com/75627147/192131390-41c7c855-d730-48c9-9334-cccdb1071c2f.png)                   

Örneğin 7. bit için -> Set: (0x1 << 7), Reset: (0x1 << 23) veya 2. bit için -> Set: (0x1 << 2), Reset : (0x1 << 18)           

**8. GPIOx_LCKR**                         
GPIO port configuration lock register - Bir pini konfigüre ettikten sonra ayarlarının değiştirmemesini istediğiniz durumlarda 
pine kilitleme işlemi uygular. 
Kilitlenmiş bir pinin ayarları mikrodenetleyicinin ya da çevresel birimin resetlenmediği sürece konfigüre edilemez. 
Kullanımı:               
```
Pinler: 0 - 15 -> Konfigüre Edilecek Pinler, 16 -> Anahtarın Bulunduğu Pin , 17 - 31 -> Rezerve Edilmiş Pinler
Adım 1: 16. bite 1 yaz ve konfigüre edeceğin pine veri yaz. ( LCKR[16] = '1'  LCKR[15:0] = DATA )
Adım 2: 16. bite 0 yaz ve konfigüre edeceğin pine veri yaz  ( LCKR[16] = '0'  LCKR[15:0] = DATA )            
Adım 3: 16. bite 1 yaz ve konfigüre edeceğin pine veri yaz  ( LCKR[16] = '1'  LCKR[15:0] = DATA )              
Adım 4: Okuma işlemi yap.                                   ( tempValue = GPIOx->LCKR           )
```

**9. GPIOx_AFRL**           
GPIO alternate function low register - Modu alternatif fonksiyon(alternate function) olarak seçilmiş bir pini yapılandormak 
çin kullanılır. 

**10. GPIOx_AFRH**                         
GPIO alternate function high register - Modu alternatif fonksiyon(alternate function) olarak seçilmiş bir pini yapılandormak 
çin kullanılır. 

# GPIO.h
```c
#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "STM32F407xx.h"

/* @def_gruop GPIO_Pins */
#define GPIO_PIN_0			( (uint16_t)(0x0001) )	/*!< GPIO Pin 0 Selected */
#define GPIO_PIN_1			( (uint16_t)(0x0002) )	/*!< GPIO Pin 1 Selected */
#define GPIO_PIN_2			( (uint16_t)(0x0004) )	/*!< GPIO Pin 2 Selected */
#define GPIO_PIN_3			( (uint16_t)(0x0008) )	/*!< GPIO Pin 3 Selected */
#define GPIO_PIN_4			( (uint16_t)(0x0010) )	/*!< GPIO Pin 4 Selected */
#define GPIO_PIN_5			( (uint16_t)(0x0020) )	/*!< GPIO Pin 5 Selected */
#define GPIO_PIN_6			( (uint16_t)(0x0040) )	/*!< GPIO Pin 6 Selected */
#define GPIO_PIN_7			( (uint16_t)(0x0080) )	/*!< GPIO Pin 7 Selected */
#define GPIO_PIN_8			( (uint16_t)(0x0100) )	/*!< GPIO Pin 8 Selected */
#define GPIO_PIN_9			( (uint16_t)(0x0200) )	/*!< GPIO Pin 9 Selected */
#define GPIO_PIN_10			( (uint16_t)(0x0400) )	/*!< GPIO Pin 10 Selected */
#define GPIO_PIN_11			( (uint16_t)(0x0800) )	/*!< GPIO Pin 11 Selected */
#define GPIO_PIN_12			( (uint16_t)(0x1000) )	/*!< GPIO Pin 12 Selected */
#define GPIO_PIN_13			( (uint16_t)(0x2000) )	/*!< GPIO Pin 13 Selected */
#define GPIO_PIN_14			( (uint16_t)(0x4000) )	/*!< GPIO Pin 14 Selected */
#define GPIO_PIN_15			( (uint16_t)(0x8000) )	/*!< GPIO Pin 15 Selected */
#define GPIO_PIN_ALL		( (uint16_t)(0xFFFF) )	/*!< GPIO Pin All Selected */

/* @def_group GPIO_Pin_Modes */
#define GPIO_MODE_INPUT		(0x0U)
#define GPIO_MODE_OUTPUT	(0x1U)
#define GPIO_MODE_AF		(0x2U)
#define GPIO_MODE_ANALOG	(0x3U)

/* @def_group GPIO_OTYPE_Moders */
#define GPIO_OTYPE_PP		(0x0U)
#define GPIO_OTYPE_OD		(0x1U)

/* @def_group GPIO_PuPd_Modes */
#define GPIO_PUPD_NOPULL	(0x0U)
#define GPIO_PUPD_PULLUP	(0x1U)
#define GPIO_PUPD_PULLDOWN	(0x2U)

/* @def_group GPIO_Speed_Modes */
#define GPIO_SPEED_LOW		(0x0U)
#define GPIO_SPEED_MEDIUM	(0x1U)
#define GPIO_SPEED_HIGH		(0x2U)
#define GPIO_SPEED_VERY		(0X3U)

/* @def_gruop AF_Modes */
#define GPIO_AF0			(0x0U)
#define GPIO_AF1			(0x1U)
#define GPIO_AF2			(0x2U)
#define GPIO_AF3			(0x3U)
#define GPIO_AF4			(0x4U)
#define GPIO_AF5			(0x5U)
#define GPIO_AF6			(0x6U)
#define GPIO_AF7			(0x7U)
#define GPIO_AF8			(0x8U)
#define GPIO_AF9			(0x9U)
#define GPIO_AF10			(0xAU)
#define GPIO_AF11			(0xBU)
#define GPIO_AF12			(0xCU)
#define GPIO_AF13			(0xDU)
#define GPIO_AF14			(0xEU)
#define GPIO_AF15			(0xFU)


typedef enum
{
	GPIO_Pin_Reset = 0x0U,
	GPIO_Pin_Set = !(GPIO_Pin_Reset),

}GPIO_PinState_t;


typedef struct
{
	uint32_t pinNumber;		/*!< GPIO Pin Numbers @def_gruop GPIO_Pins 			*/
	uint32_t Mode;			/*!< GPIO Pin Numbers @def_gruop GPIO_Pin_Modes 	*/
	uint32_t Otype;			/*!< GPIO Pin Numbers @def_gruop GPIO_OTYPE_Modes 	*/
	uint32_t PuPd;			/*!< GPIO Pin Numbers @def_group GPIO_PUPD_Modes 	*/
	uint32_t Speed;			/*!< GPIO Pin Numbers @def_gruop GPIO_Speed_Modes	*/
	uint32_t Alternate;		/*!< GPIO Pin Numbers @def_gruop AF_Modes 			*/

}GPIO_InitTypeDef_t;

void GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct);
void GPIO_WritePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber, GPIO_PinState_t pinState);
GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber);
void GPIO_LockPin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber);
void GPIO_TogglePin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber);

#endif /* INC_GPIO_H_ */
```
