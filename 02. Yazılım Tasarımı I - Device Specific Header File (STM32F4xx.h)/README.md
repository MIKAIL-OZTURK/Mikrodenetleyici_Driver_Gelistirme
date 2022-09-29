# Device Specific Header File (STM32F407xx.h)   
Mikrodenetleyici için temel tanımlamaların yapıldığı dosyadır. 

- Hafıza Birimlerinin Adresleri (Memory Base Address)
	- FLASH & SRAM1 & SRAM2
- Veri Yollarının Adresleri (Peripheral Base Address)
	- AHB1 & APB1 & APB2...
- Her bir veriyoluna bağlı çevre birimlerinin adresleri (APB2 Peripherals Base Address...)
- Çevre birimlerinin registerlarının bir yapı içerisinde tanımlamaları 
	- GPIO & RCC & SYSCFG...
- Registerlara erişebilmek için kullanılan  portların temel adresleri
```c
#define GPIOA						((GPIO_TypeDef_t *)(GPIOA_BASE_ADDR))

Örnek Kullanımı -->    
GPIOA->MODER gibi
```

- Bit tanımlamaları 


## :dart: Yol Haritası      
### **1. Temel Adres Tanımlamaları**  
- Temel adres değerleri arasında boşluk olamaz.
- Temel adres değerleri _unsiged long_ olmalıdır. Bunun için temel adres değerleri **UL** ile bitmelidir. 
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
#### 3. Veri Yollarına Bağlı Çevresel Birimlerin Temel Adresleri 
```c
ÖRNEK | #define GPIOC_BASE_ADDR		(AHB1_BASE_ADDR + 0x0800UL)	
```


### **2. Çevresel Birim Yapılarının Tanımlanması**    
- Tüm çevre birimlerinin registerları `volatile` olmak zorundadır.   
- Çevresel birimlerin registerları tanımlanırken doğru konfigürasyon için sırası ile tanımlanmalıdır. 
#### 1. GPIO (General Purpose I/O)              
#### 2. RCC (Reset and Clock Control)                          
Varsayılan olarak tüm çevresel birimlerin veri yolları(data bus ~ clock hattı) güç tasarrufu için pasif konumdadır. Kullanılamak istenen çevresel birim için clock hattı aktif edilmek zorundadır, yoksa kullanım mümkün değildir. Clock hatlarını aktif veya pasif eden yapılara RCC(reset and clock control) denir.      


### 3. Özel Tanımlamalar
#### 1. Kütüphaneler
- <stdint.h> | uint32_t ,uint16_t ,uint8_t gibi veri türlerini kullanabilmek için tanımladığımız kütüphanedir.              
- "RCC.h" | Çevresel birimler için veri yollarını aktif eden fonksiyonları içerir. (GPIO için AHB1 registerini aktif eden fonksiyonlar gibi..)  

#### 2. typedef Volatile               
```c
#define __IO volatile		// volatile keywordu yerine standart olan __IO keyword'un kullanılması
```

#### 2. Register Set - Reset - Read Fonksiyon Tanımlamaları
```c
#define SET_BIT(REG, BIT)			((REG) |=  (BIT))	// Bt Set
#define CLEAR_BIT(REG, BIT)			((REG) &= ~(BIT))	// Bit Reset
#define READ_BIT(REG, BIT			((REG) &   (BIT))	// Read Bit 
#define UNUSED(x)				(void)x			// Derleyici "Bu fonksiyon kullanılmıyor" hatasını vermesin diye kullanılan fonksiyon. 
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
