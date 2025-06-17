/*
 * ultrasonic.h
 *
 * Created: 2025-03-12 오후 2:49:06
 *  Author: microsoft
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>  // sei()등

#define LEFT_TRIG   0
#define MIDDLE_TRIG 1
#define RIGHT_TRIG  2

#define TRIG_PORT PORTA
#define TRIG_DDR  DDRA

#define LEFT_ECHO   4
#define MIDDLE_ECHO 5
#define RIGHT_ECHO  6

#define ECHO_PIN PINE // External INT 4 , INT5 , INT 6
#define ECHO_DDR DDRE

#endif /* ULTRASONIC_H_ */