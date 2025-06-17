/*
 * button.h
 *
 * Created: 2025-03-05 오후 2:30:04
 *  Author: microsoft
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>  // PORTA PORTB등의 I/O 관련 register등이 있다. 
#include <util/delay.h>  // _delay_ms _delay_us

#define BUTTON_DDR DDRB 
#define BUTTON_PIN PINB    // PINB는 PORTD를 읽는 register 5v:1  0v:0 
#define BUTTON0PIN  7    // PORTB.7

#define BUTTON0   0   // PORTD.3의 가상의 index(S/W 번호)
#define BUTTON_NUMER 1
#define BUTTON_PRESS  1   // 버튼을 누르면 high(active-high)
#define BUTTON_RELEASE 0  // 버튼을 떼면 low
#endif /* BUTTON_H_ */