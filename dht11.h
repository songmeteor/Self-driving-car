/*
 * dht11.h
 *
 * Created: 2025-03-18 오전 10:52:27
 *  Author: microsoft
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>		// printf

#define DHT_DDR		DDRG
#define DHT_PORT	PORTG
#define DHT_PIN		PING
#define DHT_PIN_NUM	0

uint8_t us_count = 0;
enum state_define {OK, TIMEOUT, VALUE_ERROR, TRANS_ERROR};
enum state_define dht11_state = OK;	// 앞으로 관리할 state변수 생성 (초기값 OK)

void dht11_main(void);
void init_dht11(void);

#endif /* DHT11_H_ */