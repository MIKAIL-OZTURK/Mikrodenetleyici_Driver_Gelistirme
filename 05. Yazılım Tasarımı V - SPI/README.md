# SPI (Serial Peripherals Interface)
Çevresel cihazlarla veya diğer mikrodenetleyicilerle veri transferi sağlayan yazılım/donanım tabanlı seri iletişim protokolüdür. Hız ile alakalı işlemlerde SPI tercih edilir.                                   
![350px-SPI_three_slaves svg](https://user-images.githubusercontent.com/75627147/195082417-567dcf47-c0a3-46b9-8ddc-ff9f2544709d.png)


- Slave cihaz sadece "SS(Slave Select) ~ CS(Chip Select)" olarak isimlendirilen sinyal ile aktif hale getirilir ve bu sinyali sadece master cihaz üretir.         
- Veri transferi seri bir şekilde bit bit yapılır ve bu sinyaller iki sinyal ile taşınır:              
  - MOSI(Master Output Slave Input): Master -> Slave                      
  - MISO(Master Input Slave Output): Slave -> Master                             
- SCLK sinyali senkron olarak MOSI ve MISO sinyallerinin taşınmasında kullanılır. Sinyal master cihaz tarafından üretirilir.


## SPI Registers 
1. **SPI control register 1 (SPI_CR1)**
- BIDIMODE: İletişimin çift yönlü(Full Duplex) veya tek yönlü(Half Duplex) olacağını belirler.         
- BIDIOE: İletişim tek taraflı(Half Duplex) ise veri alıcanak mı ,okunacak mı olduğunu belirler.  
- CRCEN: (Hardware CRC calculation enable)          
- CRCNEXT: (CRC transfer next)             
- DFF: Gönderilecek/alınacak verinin 8 veya 16 bit olmasını belirler. (Data Frame Format)                
- RXONLY: İletişim çift yönlü(Full Duplex) ise veri veri hem aınıp hem okunacak mı yoksa sadece okunaak mı olduğunu belirler. (Receive Only)                 
- SSM: SSM biti ayarlandığında, NSS pin girişi SSI bitinden gelen değer ile değiştirilir. (Software Slave Management)                     
- SSI: Bu bit, yalnızca SSM biti ayarlandığında bir etkiye sahiptir. Bu bitin değeri NSS pinine zorlanır ve NSS pininin IO değeri yoksayılır. (Internal slave select)   
- LSBFIRST: İletişim yüksek değerlikli(MSB) veya düşük değerlikli(LSB) bitten başlayacağını belirler. (8 bit ise 7. bit MSB, 0.bit LSB) (Frame Format)
- SPE: SPI'yı aktif veya pasif duruma getirir. (SPI Enable)
- BR: İletişim frekansının bölme oranını belirler. (Baud Rade)                    
- MSTR: Cihazın Master veya Slave olmasını belirler. (Master Selection)                   
- CPOL: Clock hattının başlangıç ve bitiş değerlerini ayarlar. CPOL 0 ise Clock lojik 0'dan başlar ve biter. CPOL 1 ise Clock lojik 1'den başlar ve biter. (Clock 
Polarity )                                              
- CPHA: Ne zaman örnekleme alınacağını/okunacağını belirler. CPHA 0 ise yükseelen kenarda, CPHA 1 ise düşen kenarda veri alınır/okunur. (Clock Phase)                

2. **SPI control register 2 (SPI_CR2)**                                
3. **SPI status register (SPI_SR)**  - Bayrakların(Flag) olduğu registerdir.              
- RXNE: Veri okunuyor mu okunmuyor mu bayrağıdıır. (Receive buffer not empty)           
- TXE: Veri alınıyor mu alnımıyor mu bayrağıdır. (Transmit buffer empty)              
- CHSIDE: (Channel Side)...                    
- UDR: (Underrun Flag)...                
- CRERR: (CRC Error Flag)...
- MODF: (Mode Fault)...            
- OVR: (Overrun Flag)...            
- BSY: SPI'ın meşgul mu değil mi bayrağıdır. (Busy Flag)                   
- FRE: (Frame Format Error)                  

4. **SPI data register (SPI_DR)** - Seri iletişim sonunda elde edilen veriy barındıran registerdir.                                              
5. **SPI CRC polynomial register (SPI_CRCPR)**                                
6. **SPI RX CRC register (SPI_RXCRCR)**                                  
7. **SPI TX CRC register (SPI_TXCRCR)**                            
8. **SPI_I2S configuration register (SPI_I2SCFGR)**                   
9. **SPI_I2S prescaler register (SPI_I2SPR)**                                
                  






























