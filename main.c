/*
 * 01.LED_CONTROL.c
 *
 * Created: 2025-03-04 오후 4:25:34
 * Author : microsoft
 */ 

#include "button.h"
#include <avr/interrupt.h>  // sei()등 
#include <stdio.h>  // printf scanf fgets puts gets 등이 들어 있다. 
#include "extern.h"
#include "def.h"
#include "led.h"

void init_timer0(void);
void manual_mode(void);
void auto_mode_fire_engine(void); 
void auto_mode_daycare_car(void);
void auto_algo(void);
void update_sec(void);
void init_buzzer_count(void);
void initialize(void);

volatile int msec_count = 0;
volatile int fnd_clk = 0;
volatile int ultrasonic_check_timer = 0;
volatile int led_counter = 0;
volatile int buzzer_msec = 0;

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

uint8_t is_buzzer_playing = 0;
int func_index = MANUAL_MODE;
int buzzer_delay = 0 ;
int total_distance = 0;
short led_toggle = 0;

void (*pfunc[]) () = 
{
	manual_mode, 
	auto_mode_fire_engine,   
	auto_mode_daycare_car
};

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	fnd_clk = 1;
	msec_count++;
	ultrasonic_check_timer++;
	led_counter++;

	if (is_buzzer_playing)
	{
		buzzer_msec++;
		if (buzzer_msec == buzzer_delay)
		{
			is_buzzer_playing = 0;
			buzzer_msec = 0;
			buzzer_delay = 0;
			OCR3A = 0;
		}
	}
}

int main(void)
{
	initialize();
	stdout = &OUTPUT;
	sei();
	
	while(1)
	{
		update_sec();
		pfunc[func_index] ();
	}
}

void init_timer0(void)
{
	TCNT0 = 6;  
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; 
	TIMSK |= 1 << TOIE0;  
}

void manual_mode(void)      
{
	if(get_button(BUTTON0,BUTTON0PIN))
	{
		init_L298N();
		init_fnd();
		init_led();
		func_index = AUTO_MODE_FIRE_ENGINE; 
	}
	switch(bt_data)
	{
		case 'F' :
		case 'f' :
			forward(500); //4us x 500 = 0.002sec (2ms)
			break;
		case 'B' :
		case 'b' :
			backward(500); //4us x 500 = 0.002sec (2ms)
			break;
		case 'L' :
		case 'l' :
			turn_left(700);
			break;
		case 'R' :
		case 'r' :
			turn_right(700);
			break;
		case 'S' :
		case 's' :
			stop();
			break;
		default:
			break;	
	}
	if(fnd_clk)
	{
		fnd_clk = 0;
		fnd_display(OCR1A > 0 ? OCR1A : OCR1B, func_index);
	}
}

void auto_mode_fire_engine(void)          //소방차 자율 주행
{
    Beepo();
	distance_ultrasonic();
	auto_algo();
	
	if(fnd_clk)
	{
		fnd_clk = 0;
		fnd_display(OCR1A > 0 ? OCR1A : OCR1B, func_index);
	}
	
	if(led_toggle)
	{
		AUTO_RUN_LED_PORT |= 1 << AUTO_RUN_LED_PIN;
	}
	else 
	{
		AUTO_RUN_LED_PORT &= ~(1 << AUTO_RUN_LED_PIN);
	}
	
	if(get_button(BUTTON0, BUTTON0PIN))
	{
		init_led();
		init_buzzer_count();
		func_index = AUTO_MODE_DAYCARE_CAR;
	}
}

void auto_mode_daycare_car(void)          //어린이차 자율 주행
{
	GrandFa();
	distance_ultrasonic();
	auto_algo();
	
	if(fnd_clk)
	{
		fnd_clk = 0;
		fnd_display(OCR1A > 0 ? OCR1A : OCR1B, func_index);
	}
	
	if(led_toggle)
	{
		AUTO_RUN_LED_PORT |= 1 << AUTO_RUN_LED_PIN;
	}
	else
	{
		AUTO_RUN_LED_PORT &= ~(1 << AUTO_RUN_LED_PIN);
	}
	
	if(get_button(BUTTON0, BUTTON0PIN))
	{
		initialize();
		OCR1A = OCR1B = 0;
		func_index = MANUAL_MODE;
	}
}


void auto_algo(void)
{
	if(ultrasonic_dis[1] > 27)
	{
		if (ultrasonic_dis[0] > 20 && ultrasonic_dis[2] > 20)
		{
			forward(700);
		}
		else if (ultrasonic_dis[0] <= 20 && ultrasonic_dis[2] > 20)
		{
			turn_right(900);
		}
		else if (ultrasonic_dis[0] > 20 && ultrasonic_dis[2] <= 20)
		{
			turn_left(900);
		}
		else
		{
			backward(600);
		}
	}
	else if (ultrasonic_dis[1] <= 27 && ultrasonic_dis[1] >= 10)
	{
		if (ultrasonic_dis[0] <= 20 && ultrasonic_dis[2] > 20)
		{
			turn_right(900);
		}
		else if (ultrasonic_dis[0] > 20 && ultrasonic_dis[2] <= 20)
		{
			turn_left(900);
		}
		else if(ultrasonic_dis[0] > ultrasonic_dis[2])      // 중앙은 10~20인데 좌우가 20이하일떄
		{
			turn_left(900);
		}
		else
		{
			turn_right(900); 
		}
	}
	else
	{
		backward(600);
	}
}

void update_sec(void)
{
	if(msec_count >= 200)
	{
		msec_count = 0;
		if(OCR1A!=0 || OCR1B!=0)
		{
			total_distance = total_distance + ((OCR1A > 0 ? OCR1A : OCR1B) / 70 / 5); // 1초에 40cm 이동
		}
		LCD_display(total_distance, func_index);
	}
	if(led_counter >= (OCR1A > 0 ? 0x3FF - OCR1A : 0x3FF - OCR1B))
	{
		led_counter = 0;
		led_toggle = !led_toggle;
	}
	else if (led_counter >= 0x3FF)
	{
		led_counter = 0;
	}
}

void init_buzzer_count(void)
{
	is_buzzer_playing = 0;
	OCR3A = 0;
	beepo_idx = 0;
	grandfa_idx = 0;
	buzzer_msec = 0;
	buzzer_delay = 0;
}

void initialize(void)
{
	init_timer0();
	init_timer1();
	init_L298N();
	init_uart0();
	init_uart1();
	init_ultrasonic();
	init_button();
	init_led();
	init_buzzer();
	init_fnd();
	I2C_LCD_init();
}