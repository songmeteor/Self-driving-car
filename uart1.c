/*
 * uart0.c
 *
 * Created: 2025-03-10 오전 10:32:31
 *  Author: microsoft
 */ 
#include "uart1.h"
#include "extern.h"

void init_uart1(void);

volatile uint8_t bt_data = 0;
ISR(USART1_RX_vect)
{
	bt_data = UDR1;  // bt로 부터 들어온 HW reg(UDR1)을 1byte 읽어 들인다.
	
    UART0_transmit(bt_data); //bt로 부터 들어온 문자가 어떤것인지 확인하기위해서 comportmaster 로 출력

}                    // bt_data = UDR1을 실행하면 UDR1의 내용이 빈다.
/*
  1. 전송속도: 9600bps : 총글자수 :9600/10bit ==> 960자 
     ( 1글자를 송.수신 하는데 소요 시간 : 약 1ms)
  2. 비동기 방식(start/stop부호 비트로 데이터를 구분)
  3. RX(수신) : 인터럽트 방식으로 구현 
*/

void init_uart1(void)
{
	// 1. 9600bps로 설정
	UBRR1H = 0x00;
	UBRR1L = 207;  // 9600bps P219 표9-9
	// 2. 2배속 통신  표9-1
	UCSR1A |= 1 << U2X1;  // 2배속 통신 
	UCSR1C |= 0x06;   // 비동기/data8bits/none parity
	// P215 표9-1
	// RXEN1 : UART1로 부터 수신이 가능 하도록 
	// TXEN1 : UART1로 부터 송신이 가능 하도록 
	// RXCIE1 : UART1로 부터 1byte가 들어오면(stop bit가 들어 오면)) rx interrupt를 발생 시켜라
	UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
	
}


