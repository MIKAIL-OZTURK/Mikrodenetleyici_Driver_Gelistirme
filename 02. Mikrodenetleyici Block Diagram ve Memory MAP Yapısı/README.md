# Mikrodenetleyici Hakkında Genel Bilgi      
## Mikroişlemci
Mikroişlemci, genellikle toplama, çıkarma, sayıları karşılaştırma, sayıları bir yerden diğerine aktarma, aritmetik ve mantık işlemlerini 
gerçekleştiren bilgisayar sistemlerinin merkezi birimidir. Basitçe, işlemci, merkezi işlem birimi veya mantık yongası olarak da bilinmektedir.
Mikroişlemcinin beyni CPU(Central Process Unit) ‘dur. Mikroişlemcinin hafızasındaki programları bulma, çağırma ve onları çalıştırma, veri işleme 
ve veri akışı gibi işlemler bu birim sayesinde gerçekleştirilir. CPU'ların içerisinde 8,16,32 ve 64 bitlik **register**leri(kaydediciler) 
bulunmaktadır.
> **Register** - Hafızadaki(RAM) veriler işlenirken kullanılan geçici ve kalıcı saklayıcılardır. Registerler işlemcinin çekirdeğinde 
> olduklarından verilere ulaşmak daha hızlı gerçekleşir.

## Mikrodenetleyici
Tek çiplik bilgisayar olarak tanımlanabilir. İçerisinde bir mikroişlemci, bellek, sayısal giriş-çıkış pinleri ve diğer çevresel birimleri 
barındırır. Popüler mikrodenetliyici türleri PIC, ARM, 8051(İntel), AVR(Atmel), MSP(Texas) 'dir. 

## Veri Yolları (Bus)
Mikroişlemciyle diğer çevresel birim arasındaki veri iletişimini sağlayan yapılara veri yolu(bus) denir. 
 
> Varsayılan olarak tüm çevresel birimlerin veri yolları(data bus ~ clock hattı) güç tasarrufu için pasif 
> konumdadır. Kullanılamak istenen çevresel birim için clock hattı aktif edilmek zorundadır, yoksa kullanım 
> mümkün değildir. 

_Daha ayrıntılı bilgi için: "STM32F407VG Data Sheet.pdf -> Sayfa 19"_

# Block Diagramlar Neden Kullanılır ?     
1. Mikrodenetliyici tasarımı hakkında genel bir bilgi verir. (Mikroişlemci, RAM, Flash vs.)
2. Mikrodenetleyici sahip olduğu çevresel birimler hakkında bilgi verir. (GPIO, ADC, TIM, SPI vs.)
3. Mikroişlemci ile çevresel birimlerin bağlı olduğu veri yolları hakkında bilgi verir. (AHB1, APB1 vs. ) 

