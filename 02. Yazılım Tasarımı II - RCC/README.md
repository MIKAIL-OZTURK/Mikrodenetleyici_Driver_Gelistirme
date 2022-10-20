# RCC (Reset and Clock Control)
Varsayılan olarak tüm çevresel birimlerin veri yolları(data bus ~ clock hattı) güç tasarrufu için pasif konumdadır. Kullanılamak istenen çevresel birim için clock 
hattı aktif edilmek zorundadır, yoksa kullanım mümkün değildir. Clock hatlarını aktif veya pasif eden yapılara RCC(reset and clock control) denir.   

## RCC.h 
1. Çevresel birimlerin clock hattını aktif veya devre dışı bırakmak için kullanılan makro fonksiyonlar

## RCC.c 

## RCC Registers 

RCC Clock Configuration Register (RCC_CFGR):

1. SW(System clock switch): Sistem clock seçimi yapmak için kullanılır. (HSI, HSE, PLL)
2. SWS(System clock switch status): Sistemde hangi clock'un kullanıldığını gösterir. 
3. 
4- 7. HPRE(AHB Prescaler): Clock bölünme oranlarının bulunduğu registerdir. (DIV4, DIV8...)












