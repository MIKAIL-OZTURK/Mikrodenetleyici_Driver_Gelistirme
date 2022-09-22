# RCC.h
Varsayılan olarak tüm çevresel birimlerin veri yolları(data bus ~ clock hattı) güç tasarrufu için pasif konumdadır. Kullanılamak istenen çevresel birim için clock 
hattı aktif edilmek zorundadır, yoksa kullanım mümkün değildir. Clock hatlarını aktif veya pasif eden yapılara RCC(reset and clock control) denir.   

### STM32F407xx.h Dosyasında Olanlar
- RCC Register Yapısı ( typedef struct { ... }RCC_TypeDef_t; )
- RCC Nesnesi ( #define RCC									((RCC_TypeDef_t  *)(RCC_BASE_ADDR  )) )
- AHB1 veri yolu üzerindeki GPIO portlarını kullanabilmek için bit tanımlamaları (Bit Definition)  

### RCC.h Dosyasında Olanlar 
- RCC AHB1 Peripherals Clock Control Macro Definitions 

## :dart: Yol Haritası  
### 1. AHB1 Veri Yolunu Aktif veya Pasif Eden Registerler 
#### 1. GPIO
GPIO çevre birimlerini kullanabilmek için GPIO'nun bağlı olduğu veri yolu aktif edilmelidir: AHB1.     
AHB1 veri yolunu GPIO portları için aktif eden RCC registeri: ```RCC->AHB1ENR```

```c
/* RCC AHB1 Peripherals Clock Control Macro Definitions */
#define RCC_GPIOA_CLK_ENABLE()		  do{ 	uint32_t tempValue = 0;										                 \
                                          SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);					       \
                                          tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);	 \
                                          UNUSED(tempValue);											                   \
                                        }while(0)

#define RCC_GPIOB_CLK_ENABLE()		  do{	uint32_t tempValue = 0;										                   \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);					         \
                                        tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);	   \
                                        UNUSED(tempValue);											                     \
                                      }while(0)

#define RCC_GPIOC_CLK_ENABLE()		  do{ 	uint32_t tempValue = 0;										                  \
                                          SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);					        \
                                          tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);	  \
                                          UNUSED(tempValue);											                    \
                                        }while(0)

#define RCC_GPIOD_CLK_ENABLE()		  do{	uint32_t tempValue = 0;										                     \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);					           \
                                        tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);	     \
                                        UNUSED(tempValue);											                       \
                                      }while(0)

#define RCC_GPIOA_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN)
#define RCC_GPIOB_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN)
#define RCC_GPIOC_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN)
#define RCC_GPIOD_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN)
```




