//J5 usart2
#include "sys.h"
#include "usart2.h"	
#include "string.h"
#include "wit_c_sdk.h"


void UART2_SendOneByte(u8 dat)
{
    while((USART2->SR & 0X40) == 0) {};  //Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï
    USART2->DR = (u8) dat;
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

void Uart2_Send(uint8_t* data, uint32_t len) {
  

    for(uint16_t i = 0; i < len; i++) {
        UART2_SendOneByte( * data++);
    }

}

void USART2_IRQHandler(void)
{
		unsigned char ucTemp;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
            ucTemp = USART_ReceiveData(USART2);
						WitSerialDataIn(ucTemp);//½ÓÊÕµ½µÄÊý¾Ý´«ÈëWitSerialDataInº¯ÊýÖÐ
						USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
		
}



