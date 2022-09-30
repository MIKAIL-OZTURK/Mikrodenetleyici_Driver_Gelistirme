## :dart: Yol Haritası  
### Çevresel birimlerin clock hattını aktif etmek veya devre dışı bırakmak için kullanılan makro fonksiyonlar

```c
/* RCC AHB1 Peripherals Clock Control Macro Definitions */
#define RCC_GPIOA_CLK_ENABLE()		  do{ 	uint32_t tempValue = 0;										                 \
                                          SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);					       \
                                          tempValue = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);	 \
                                          UNUSED(tempValue);											                   \
                                        }while(0)
     
#define RCC_GPIOA_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN)
```

