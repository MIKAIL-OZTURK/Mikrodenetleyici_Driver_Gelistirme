# GPIO.h  
## :dart: Yol Haritası
### 1. Registerlerin Aldığı Değerlerin Tanımlanması 
```c
/* @def_gruop GPIO_Pins */
#define GPIO_PIN_0			(uint16_t)(0x0001)	/*!< GPIO Pin 0 Selected */
#define GPIO_PIN_1			(uint16_t)(0x0002)	/*!< GPIO Pin 1 Selected */
...

/* @def_group GPIO_Pin_Modes */
#define GPIO_MODE_INPUT		(0x0U)
#define GPIO_MODE_OUTPUT	(0x1U)
...

/* @def_group GPIO_OTYPE_Moders */
#define GPIO_OTYPE_PP		(0x0U)
#define GPIO_OTYPE_OD		(0x1U)

/* @def_group GPIO_PuPd_Modes */
#define GPIO_PUPD_NOPULL	(0x0U)
#define GPIO_PUPD_PULLUP	(0x1U)
...

/* @def_group GPIO_Speed_Modes */
#define GPIO_SPEED_MEDIUM	(0x1U)
... 
```
### 2. Yapılar
```c
typedef struct
{
	uint32_t pinNumber;		/*!< GPIO Pin Numbers @def_gruop GPIO_Pins 			*/
	uint32_t Mode;			/*!< GPIO Pin Numbers @def_gruop GPIO_Pin_Modes 	*/
	uint32_t Otype;			/*!< GPIO Pin Numbers @def_gruop GPIO_OTYPE_Modes 	*/
	uint32_t PuPd;			/*!< GPIO Pin Numbers @def_group GPIO_PUPD_Modes 	*/
	uint32_t Speed;			/*!< GPIO Pin Numbers @def_gruop GPIO_Speed_Modes	*/
	uint32_t Alternate;		/*!< GPIO Pin Numbers @def_gruop GPIO_Pins 			*/

}GPIO_InitTypeDef_t;
```

### 3. Fonksiyon Prototipleri ( API's )
```c
void GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct);
void GPIO_WritePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber, GPIO_PinState_t pinState);
GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber);
void GPIO_LockPin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber);
void GPIO_TogglePin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber);
```