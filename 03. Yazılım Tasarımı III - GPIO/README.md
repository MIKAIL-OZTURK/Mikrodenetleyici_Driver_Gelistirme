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
GPIO alternate function low register - Modu alternatif fonksiyon(alternate function) olarak seçilmiş bir pini yapılandırmak 
çin kullanılır. 

**10. GPIOx_AFRH**                         
GPIO alternate function high register - Modu alternatif fonksiyon(alternate function) olarak seçilmiş bir pini yapılandırmak 
için kullanılır.                    

---

# API's                 
## void GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct)          
## void GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct)               
## void GPIO_WritePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber, GPIO_PinState_t pinState)                 
## GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)                
## void GPIO_LockPin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber)                        
## void GPIO_TogglePin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber)                       









