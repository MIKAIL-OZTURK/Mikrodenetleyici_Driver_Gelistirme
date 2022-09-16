# Gömülü Sistem Katmanları 
Bir gömülü yazılım uygulama katmanı(application layer) ve sürücü katmanından(driver layer) 
oluşur. Sistem olarak düşünürsek bir de donanım katmanı vardır. 
Üçünün arasındaki ilişkiyi anlamak için bir led yakma örneği:

1. GPIOWritePin(GPIOD, GPIO_PIN_12);            // Uygulama Katmanı (Application Layer)
2. STM32F407xx.h -> GPIO.h -> GPIO.c            // Sürücü Katmanı (Driver Layer)
3. GPIOD_OutputData Register -> 0X40020048      // Donanım (Hardware)


## Device Specific Headers Files (STM32F407xx.h)
1. Mikrodenetleyici için tanımlanmış çevresel birimlerin temel adreslerini içerir.   
2. Flash, SRAM1 ve SRAM2'in temel adreslerini içerir.    
3. Kesme numalarının (Interruprt Number Definition) tanımını içerir. 
4. Makro tanımlarını içerir. ( Bit definitaion'daki makrolar )
