# USART (Universal Synchronous and Asynchronous Receiver-Transmitter)
USART protokolü bir seri iletişim protokolüdür. USART protokolünde veriler senkron veya asenkron olarak alınabilir.                
**Senkron** veri alışverişinde bir data hattı ve bir clock hattı bulunmalıdır. Data hattından gidecek veriler, clock hattından gönderilen sinyalin her düşen veya yükselen kenarında alıcıya iletilir.                             
**Asenkron** veri alışverişinde clock hattında ihtiyaç duyulmaz. Verilerin gönderilmeye başlayacağını bildiren bir start biti, verinin gönderilmesinin bittiğini belirten bir stop biti ve bir de verilerin doğru olarak gönderilip gönderilmediğini anlamak için kullanılan parity(eşitlik) biti kullanılmaktadır.            
           
![4PemT](https://user-images.githubusercontent.com/75627147/196900048-93d08a81-d820-423e-92e4-7e8d73174df9.png)

> Asenkron iletişimde kullanılan diğer bir kavram veri gönderim hızıdır(Baud Rate). Bu kavram "bps (bits per second - saniyede gönderilen bit sayısı)" birimi ile 
> anılır. 

## USART Registers 
1. **Status register (USART_SR)** - Bayrakların(Flags) bulunduğu registerdir.     
- PE: (Parity Error)   
- FE: (Framning Error)   
- NF: (Noise Detected Flag)    
- OR: (Overrun Error)   
- IDLE: (IDLE Line Detected)   
- RXNE: (Read Data Register not Empty)    
- TC: (Transmission complete)    
- TXE: (Transmit Data Register Empty)   
- LBD: (LIN Break Detection Flag)   
- CTS: (CTS Flag)     

2. **Data register (USART_DR)** - Seri iletişim sonunda elde edilen veriyi barındıran registerdir.                   
3. **Baud rate register (USART_BRR)**  
4. **Control register 1 (USART_CR1)**            
5. **Control register 2 (USART_CR2)**          
6. **Control register 3 (USART_CR3)**              
7. **Guard time and prescaler register (USART_GTPR)**


## API's 

### void USART_Init(USART_HandleTypedef_t *USART_Handle);


### void USART_TransmitData(USART_HandleTypedef_t *USART_Handle, uint8_t *pData, uint16_t dataSize);


### void USART_ReceiveData(USART_HandleTypedef_t *USART_Handle, uint8_t *pBuffer, uint16_t dataSize);


### void USART_TransmitData_IT(USART_HandleTypedef_t *USART_Handle, uint8_t *pData, uint16_t dataSize);


### void USART_ReceiveData_IT(USART_HandleTypedef_t *USART_Handle, uint8_t *pBuffer, uint16_t dataSize);


### void USART_PeriphCmd(USART_HandleTypedef_t *USART_Handle, FunctionalState_t stateOfUSART);


### USART_FlagStatus_t USART_GetFlagStatus(USART_HandleTypedef_t *USART_Handle, uint16_t flagName);


### void USART_InterruptHandler(USART_HandleTypedef_t *USART_Handle);


















































