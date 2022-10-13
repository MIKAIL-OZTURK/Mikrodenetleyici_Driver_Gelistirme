# USART INTERRUPRT MOD

## USART_TRANSMIT_IT

### 1. USART2IRQ_Handler();
İnterrupt modu için main.c dosyalarında tanımlanan özel interrupt fonksiyonlarıdır. Kesme uygulanacak birimin fonksiyonlaru `Startup > startup_stm32f407vgtx.c` dosyasında tanımlıdırlar. 

### 2. USART_InterruprtHandler();
İnterrupt'ın gelip gelmediğini uygun register ve bayrağa bakarak kontrol edildlği fonksiyondur. Register ve bayrağa gelen veriler ile interrupt'ın türü belirlenir.  
Örneğin USART_Transmit mi USART_Receive mi burada karar verilir.    
USART2IRQ_Handler() fonksiyonunda çağırılırlar. 

### 3. USART_TransmitData_IT();
İnterrupt işleminin başladığı kısımdır. İşlem sırasında kullanılacak veriler global değişkenlere atanır ki tüm fonksiyonlar erişebilsin. Ardından interrupt'ı aktif hale getiriyoruz.
Böylece interrupt başlatıldı ve veriler işlenmeye hazır. 
**INTERRUPT MODE -> ENABLE**                                        
**INTERRUPT DATAS -> READY**              

### 4. USART_SendWith_IT();
USART_TransmitData_IT() fonksiyonundan gelen verilerin ne iş yapacağına karar verildiği yani verilerin işlendiği fonksiyondur. Örneğin tranmsit, receive işlemleri burada gerçekleşir
**INTERRUPT DATAS -> DONE** 

### 5. closeUSART_ISRC();
 USART_SendWith_IT() fonksiyonunda işlemi biten verilerin sıfırlandığı ve interrupt'ın pasif hale getirildiği kısımdır. 
**INTERRUPT MODE -> DISABLE** 
**INTERRUPT DATAS -> RESET**








