# GPIO (General Purpose Input Output)

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

### void GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct)                                            
GPIO port ve pinlerini kondigüre eder.                                   
***GPIO_TypeDef_t *GPIOx*** - Port bilgisi alır. (GPIOA...GPIOK gibi)                                   
**GPIO_InitTypeDef_t *GPIO_ConfigStruct*** - Konfigürasyon sağlayan yapının adresini alır. Örneğin:                               

```c
static void GPIO_LedConfig()
{
	GPIO_InitTypeDef_t GPIO_LedStruct = { 0 };  	// Konfigürasyon sağlayan yapı 
	RCC_GPIOD_CLK_ENABLE();		                  // Clock Enable for LED's 

	// LED Configuration
	GPIO_LedStruct.pinNumber = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_LedStruct.Mode = GPIO_MODE_OUTPUT;
	GPIO_LedStruct.Speed = GPIO_SPEED_LOW;
	GPIO_LedStruct.Otype = GPIO_OTYPE_PP;
	GPIO_LedStruct.PuPd = GPIO_PUPD_NOPULL;
  
	GPIO_Init(GPIOD, &GPIO_LedStruct);
 }
```
#### API İncelenmesi 

```c
void GPIO_Init(GPIO_TypeDef_t *GPIOx, GPIO_InitTypeDef_t *GPIO_ConfigStruct)
{
	uint32_t position;
	uint32_t fakePosition = 0x00U;
	uint32_t lastPosition = 0x00U;
	uint32_t tempValue = 0x00U;
	
	for(position = 0U; position < GPIO_MAX_PIN_NUMBER; position++)
	{
		fakePosition = (0x1U << position);
		lastPosition = ((uint32_t)(GPIO_ConfigStruct->pinNumber) & fakePosition);	
		if(fakePosition == lastPosition)
		{
	// 1. for döngüsü ile 0 - 16 bitlerini geziyoruz. (position = 1,2,3,4...15)
	// 2. fakeValue değeri ile her bite 1 değerini yazıyoruz. (fakeValue = 0x1,0x2,0x4...0x8000)
	// 3. Kullanıcının girdiği pin numarası ile fakeValue değerlerini ve işlemine tabi tutuyoruz. (Kontrol)
	// 4. Eşleştikleri(fakeValue == lastValue) position değeri pin numarası olur.  
	// Pin numarası bulunduktan sonra ilgili konfigürasyon ayarlarına geçilir -->

			/* MODE CONFIG */
			tempValue = GPIOx->MODER;
			tempValue &= ~(0x3U << (position * 2));
			tempValue |= (GPIO_ConfigStruct->Mode << (position * 2));
			GPIOx->MODER = tempValue;
	// GPIO port mode register(GPIOx_MODER) - Bir pinin modunu belirlemek için kullanılır.   
	// 1. Register üzerinde işlem yapmıyoruz o yüzden ilgili registeri işlem yapacağımız geçici bir değişkene atadık. 
	// 2. Güvenlik amaçlı önce ilgili bitleri temizliyoruz. 
		// 0x3U yani iki biti 1 olan sayı(0x00000011 = 0x3U) ile tersliyoruz.  
		// MODER reigsteri iki bit değer(0x10,0x11 gibi) alacağı için (position * 2) yapıyoruz. 
		// Örneğin MODER5'e erişmek için (5x2) 10 ve 11. bitlere değer yazmak gerekir. 
	// 3. Kullanıcının girdiği mod değerini işlem yaptığımız değişkene atadık. (tempValue = GPIO_MODE_INPUT gibi)
	// 4. Mod için tüm konfigürasyonlar hazır ve MODER registeri bu konfigürasyon ayarlarına göre çalışabilir.
	
			if(GPIO_ConfigStruct->Mode == GPIO_MODE_OUTPUT || GPIO_ConfigStruct->Mode == GPIO_MODE_AF)
			{
				/* Output Type CONFIG */
				tempValue = GPIOx->OTYPER;
				tempValue &= ~(0x1U << position);
				tempValue |= (GPIO_ConfigStruct->Otype << position);
				GPIOx->OTYPER = tempValue;

				/* Output Speed CONFIG */
				tempValue = GPIOx->OSPEEDR;
				tempValue &= ~(0x3U << (position * 2));
				tempValue |= (GPIO_ConfigStruct->Speed << (position * 2));
				GPIOx->OSPEEDR = tempValue;
		// OTYPER ve OSPEEDR sadece modun OUTPUT veya ANALOG olduğu durumlarda çalışır.
		// Adı üstünde OTYPER = Output Type... OSPEEDR = Output Speed...
		// Konfigürasyon ayaları MODER ile aynı mnatıkla çalışır. 
		// Geçici bir değişken oluşturuduk, güvenlik amaçlı temizledik ve kullanıcının girdiği değeri en son
		// ilgili register'lara ilettik. 
			}
			/* Push Pull CONFIG */
			tempValue = GPIOx->PUPDR;
			tempValue &= ~(0x3U << (position * 2));
			tempValue |= (GPIO_ConfigStruct->PuPd << (position * 2));
			GPIOx->PUPDR = tempValue;
		// Konfigürasyon ayaları MODER ile aynı mnatıkla çalışır. 
		// Geçici bir değişken oluşturuduk, güvenlik amaçlı temizledik ve kullanıcının girdiği değeri en son
		// ilgili register'lara ilettik. 

			/* Alternate Mode CONFIG */
			if(GPIO_ConfigStruct->Mode == GPIO_MODE_AF)
			{
				tempValue = GPIOx->AFR[position >> 3U];
				tempValue &= ~(0xFU << ((position & 0x7U) * 4));
				tempValue |= (GPIO_ConfigStruct->Alternate << ((position & 0x7U) * 4));
				GPIOx->AFR[position >> 3U] = tempValue;
			}
		// Alternatif Funciton modu seçilmiş ise konfigürasyon ayarları yap -->
		// AFR[0] = AFRL(alternate function low register), AFR[1] = AFRH (alternate function high register)
		// AFRL = 0 ile 7 arasındaki bitler için işlme yapar.
		// AFRH = 8 ile 15 arasındaki bitler için işlem yapar. Örndeğin AFR9 için işlem yapmak istersek AFRH
		// registerini kullanmak gerekir. AFRH ise AFR[1]'de idi. Yani AFR9 için AFR[1] kullanmak gerekir.
		// 1. Register üzerinde işlem yapmıyoruz, geçici bir değişken oluşturduk. 
			// AFR[position >> 3U] = AFR[pinNumber / 8] 
			// Neden 8'e böldük ? Çünkü AFRL(0-7) ve AFRH(8-15) 8 adet register tutuyor. Diyelim ki kullanıcı AFR6'da işlem yapmak istesin.
			// AFR[ 6 / 8 ] = AFR[ 0 ], peki AFR[0]'da ne vardı -> AFRL. 
			// Demekki AFR6 için AFRL'de işlem yapamk gerekir, peki ARFL nerede -> AFR[0] 
			// ** Bir sayıyı 8'e bölmek (2^3) bitsel olarak sayısı 3 bit sağa kaydırmak demektir. 16'ya bölmek 4 bit, 64'e bölmek 6 bit sağa...
		// 2. Güvenlik amaçlı önce ilgili bitleri temizliyoruz. 
			// 0xFU : AFR registeri 4 bit değer alacağından dolayı 4 biti 1 olan sayı ile tersliyoruz.(0x00001111U = 0xFU)
			// Eğer ilgili register 3 bit değer alsaydı 3 biti 1 olan sayı ile terslerdik. Yani 0x7U (0x0000111 = 0x7U)
			// position & 0x7U : 8'e göre mod aldık. (2^3 = 8).Buradan 3'ü çekiyoruz ve 3 biti 1 olan sayi ile & işlemi yapıyoruz. 
			// ((position & 0x7U) * 4)) ise mod sonucunu 4 ile çarpıp ilgili registeri buluyoruz. 
			// Neden 4 ile çarpıypruz ? Çünkü bir bir register 4 bit değer alıyor. (0101, 1111 gibi...)
				// Örnekle izah edelim. AFR15'e ulaşmak için -->
				// 1. AFR[15 / 8] = AFR[1] yani ARF15 için AFRH(AFR[1])'a ulaşmak gerekir.
				// 2. Peki AFR15 AFR[1]'in hangi pininden itiabren başlıyor ? 
				// (15 % 8) = 7
				// 7 x 4 = 28 (Başlangıç Biti 28)
				// Demek ki AFR15, AFR[1]'de ve 28. pinde başlıyor. (AFRH15 = 28 29 30 31)
				// Örnek 2: AFR3 için -->
				// 1. AFR[3 / 8] = AFR[0] (AFRL)
				// 2. (3 % 8) = 3, 3 x 4 = 12 (Başlangıç Biti 12)
				// AFRL3 = 12 13 14 15 
		// 3. Kullanıcının girdiği değeri, ilgili bit kadar kaydırarak işlem yaptığımız değişkene atadık. (tempValue = GPIO_AF7 gibi)
		// 4. Altenate Function için tüm konfigürasyonlar hazır ve AFR registeri bu konfigürasyon ayarlarına göre çalışabilir.
		// ÖZETLERSEK 
		// AFR[0~1] için 8'e böldük, ilgili bitleri bulmak için 8'e göre mod alıp 4 ile çarptık. Bitleri temizledik ve kullanıcıdan 
		// verileri aldık. Konfigürasyon ayarları bitince bu işlemleri geçici değişkenden ilgili registere ilettik.
		}
	}
}
```
            
### void GPIO_WritePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber, GPIO_PinState_t pinState)                                  
Bir pinin değerini SET veya RESET yapar. 
***GPIO_TypeDef_t *GPIOx*** - Port bilgisi alır. (GPIOA...GPIOK gibi)                            
***uint16_t pinNumber*** - Pin bilgisi alır. (GPIO_PIN_7 gibi)                              
***GPIO_PinState_t pinState*** - Pin durum bilgisi alır. (GPIO_PIN_SET veya GPIO_PIN_RESET)                                       

```c
void GPIO_WritePin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber, GPIO_PinState_t pinState)
{
	//  BSRR: Bir biti set veya reset yapar. Kullanımı:     
	//  0 - 15 Bitler için 		1 -> Set		0 -> İşlem Yok 
	// 16 - 31 Bitler için 		1 -> Reset 		0 -> İşlem Yok
	
	if(pinState == GPIO_Pin_Set)
	{
		GPIOx->BSRR = pinNumber;
	}
	else
	{
		GPIOx->BSRR = (pinNumber << 16U);
	}
	// 0-15 bitlerini SET yapmak için ilgili bite 1 yazmak yeterli. (GPIOx->BSRR = pinNumber)
	// Ancak RESET yapmak için 16 bit ötelemek gerekir. (GPIOx->BSRR = (pinNumber << 16U))
}
```




### GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber)                
### void GPIO_LockPin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber)                        
### void GPIO_TogglePin(GPIO_TypeDef_t* GPIOx, uint16_t pinNumber)                       
