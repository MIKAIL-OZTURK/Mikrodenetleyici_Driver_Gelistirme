#include <stdio.h>
#include <stdint.h>

#define GPIO_MAX_PIN_NUMBER    ( (uint16_t)(16U) )

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

int main(void)
{
    uint16_t position;
    uint16_t fakeValue = 0x0U;
    uint16_t lastValue = 0x0U;

    uint16_t GPIO_Pin_Config = GPIO_PIN_11 | GPIO_PIN_7 | GPIO_PIN_2;

    for (uint16_t position = 0U; position < GPIO_MAX_PIN_NUMBER; position++)
    {
        fakeValue = (0x1U << position);
        lastValue = ((GPIO_Pin_Config) & fakeValue);

        printf("position = %d \t fakeValue = %#x \t lastValue = %#x \n", position, fakeValue, lastValue);
        /*
        position = 0     fakeValue = 0x1         lastValue = 0
        position = 1     fakeValue = 0x2         lastValue = 0
        position = 2     fakeValue = 0x4         lastValue = 0x4
        position = 3     fakeValue = 0x8         lastValue = 0
        position = 4     fakeValue = 0x10        lastValue = 0
        position = 5     fakeValue = 0x20        lastValue = 0
        position = 6     fakeValue = 0x40        lastValue = 0
        position = 7     fakeValue = 0x80        lastValue = 0x80
        position = 8     fakeValue = 0x100       lastValue = 0
        position = 9     fakeValue = 0x200       lastValue = 0
        position = 10    fakeValue = 0x400       lastValue = 0
        position = 11    fakeValue = 0x800       lastValue = 0x800
        position = 12    fakeValue = 0x1000      lastValue = 0
        position = 13    fakeValue = 0x2000      lastValue = 0
        position = 14    fakeValue = 0x4000      lastValue = 0
        position = 15    fakeValue = 0x8000      lastValue = 0
        */
        if (lastValue == fakeValue)
        {
            /* PIN NUMBER IS FOUND !    */
            /* position is pin number   */
            /* lastValue is pin value   */
        }
    }

    return 0;
}
