# SPI (Serial Peripherals Interface)
Çevresel cihazlarla veya diğer mikrodenetleyicilerle veri transferi sağlayan yazılım/donanım tabanlı seri iletişim protokolüdür.         
![350px-SPI_three_slaves svg](https://user-images.githubusercontent.com/75627147/195082417-567dcf47-c0a3-46b9-8ddc-ff9f2544709d.png)


- Slave cihaz sadece "SS(Slave Select) ~ CS(Chip Select)" olarak isimlendirilen sinyal ile aktif hale getirilir ve bu sinyali sadece master cihaz üretir.         
- Veri transferi seri bir şekilde bit bit yapılır ve bu sinyaller iki sinyal ile taşınır:              
  - MOSI(Master Output Slave Input): Master -> Slave                      
  - MISO(Master Input Slave Output): Slave -> Master                             
- SCLK sinyali senkron olarak MOSI ve MISO sinyallerinin taşınmasında kullanılır. Sinyal master cihaz tarafından üretirilir.


## SPI Registers 
































