/*
 * ultrasonic.c
 *
 * Created: 2025-03-12 오후 2:49:23
 *  Author: microsoft
 */ 
#include "ultrasonic.h"

extern volatile int ultrasonic_check_timer;

void init_ultrasonic(void);
void trigger_ultrasonic(void);
void distance_ultrasonic(void);

volatile int ultrasonic_dis[3] = {0};
volatile char scm[3][50];
static uint16_t start_time = 0;

ISR(INT4_vect)
{
	if(ECHO_PIN & 1 << LEFT_ECHO)
	{
		TCNT2 = 0;
	}
	else
	{
		ultrasonic_dis[0] = 1000000.0 * TCNT2 * 1024 / F_CPU / 58;
		//sprintf(scm[0],"left: %d cm\n", ultrasonic_dis[0] );
	}
}

ISR(INT5_vect)
{
	if(ECHO_PIN & 1 << MIDDLE_ECHO)
	{
		TCNT2 = 0;
	}
	else
	{
		ultrasonic_dis[1] = 1000000.0 * TCNT2 * 1024 / F_CPU / 58;
		//sprintf(scm[1],"middle: %d cm\n", ultrasonic_dis[1] );
	}
}

ISR(INT6_vect)
{
	if(ECHO_PIN & 1 << RIGHT_ECHO)
	{
		TCNT2 = 0;
	}
	else
	{
		ultrasonic_dis[2] = 1000000.0 * TCNT2 * 1024 / F_CPU / 58;
		//sprintf(scm[2],"right: %d cm\n", ultrasonic_dis[2] );
	}
}

void init_ultrasonic(void)
{
	TRIG_DDR |= 1 << LEFT_TRIG | 1 << MIDDLE_TRIG | 1 << RIGHT_TRIG ;
	ECHO_DDR &= ~(1 << LEFT_ECHO | 1 << MIDDLE_ECHO | 1 << RIGHT_ECHO); 
	
	EICRB |= 0 << ISC41 | 1 << ISC40 | 0 << ISC51 | 1 << ISC50 | 0 << ISC61 | 1 << ISC60; 
	
	TCCR2 |= 1 << CS22 | 1 << CS20; 
	
	EIMSK |= 1 << INT4 | 1 << INT5 | 1 << INT6; 
}

void trigger_ultrasonic(void)
{
	TRIG_PORT &= ~(1 << LEFT_TRIG | 1 << MIDDLE_TRIG | 1 << RIGHT_TRIG); //low
	_delay_us(1);
	TRIG_PORT |= 1 << LEFT_TRIG | 1 << MIDDLE_TRIG | 1 << RIGHT_TRIG; //high
	_delay_us(15); //규격에는 10us인데 여유를 둬서 15us
	TRIG_PORT &= ~(1 << LEFT_TRIG | 1 << MIDDLE_TRIG | 1 << RIGHT_TRIG); //low
}

void distance_ultrasonic(void)
{
	if(ultrasonic_check_timer >= 50) // 0.5초
	{
		ultrasonic_check_timer = 0;
		//printf("%s%s%s",scm[0],scm[1],scm[2]);
		trigger_ultrasonic();
	}
}