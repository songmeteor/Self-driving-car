/*
 * pwm.c
 *
 * Created: 2025-03-13 오후 12:49:00
 *  Author: microsoft
 */ 

#include "pwm.h"
#include "extern.h"

void init_timer1(void);
void init_L298N(void);
void stop(void);
void forward(int speed);
void backward(int speed);
void turn_left(int speed);
void turn_right(int speed);

/*
	16bit timer 3번 활용
	PMW 출력 신호
	---------------
	PE5 : OC1A (왼쪽 바퀴)
	PE6 : OC1B (오른쪽 바퀴)
	BTN0 : auto / manual
	  
*/

void init_timer1(void)
{
	// 분주비 : 64 16000000 / 64 --> 2500000 Hz (250kHz)
	// T = 1/f = 1 / 2500000Hz --> 0.000004sec(4us)
	// 2500000Hz 에서 256개 펄스를 count하면 소요시간 1.02ms
	//                127개                         0.5ms
	//P318 표 14-1
	TCCR1B |= 1 << CS11 | 1 << CS10; //분주비 : 64
	
	//16 timer 1번의 모드 14: 16비트 고속 PWM (p348 표15-6)
	TCCR1A |= 1 << WGM11;    //TOP을 ICR1에 지정을 할 수 있다.
	TCCR1B |= 1 << WGM12 | 1 << WGM13;
	
	// 비반전 모드 TOP : ICR1비교일치값(PWM)지정 : OCR1A, OCR1B
	// 사용자가 설정한 PWM값 일치시 OCR1A, OCR1B의 출력핀이 LOW로 바뀌고 BOTTOM에 HIGH로 바뀐다
    TCCR1A |= 1 << COM1A1 | 1 << COM1B1;
	
	ICR1 = 0x3FF; // ICR1(Input Capture Register) : TOP 값 1023 == > 4ms
	
}

/*
	방향 설정
	=========
	1. LEFT MOTOR
	PF0 : IN1 (motor driver)
	PF1 : IN2
	2. RIGHT MOTOR
	PF2 : IN3 (motor driver)
	PF3 : IN4
	
	IN1 / IN3      IN2 / IN4
	0              1      (역회전)
	1              0      (정회전)`
	1              1       stop
*/

void init_L298N(void)
{
	MOTOR_PWM_DDR |= 1 << MOTOR_LEFT_PORT_DDR | 1 << MOTOR_RIGHT_PORT_DDR;
	MOTOR_DRIVER_DIRECTION_DDR |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;   // IN1 IN2 IN3 IN4 출력모드
	
    MOTOR_DRIVER_DIRECTION_PORT &= ~( 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 ); 
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 3 | 1 << 0;                           // 전진모드
}

void stop(void)
{
	MOTOR_DRIVER_DIRECTION_PORT &= ~( 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 );
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;       // 정지
	OCR1A = OCR1B = 0;
}

void forward(int speed)
{
	 MOTOR_DRIVER_DIRECTION_PORT &= ~( 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 ); 
	 MOTOR_DRIVER_DIRECTION_PORT |= 1 << 3 | 1 << 0;                        // 전진모드
	 OCR1A = OCR1B = speed;                                           // PORTB5(OCR1A) 왼쪽, PORTB6(OCR1B)오른쪽
}

void backward(int speed)
{
	MOTOR_DRIVER_DIRECTION_PORT &= ~( 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 );
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 2 | 1 << 1;                        // 후진모드
	OCR1A = OCR1B = speed;                                           // PORTB5(OCR1A) 왼쪽, PORTB6(OCR1B)오른쪽
}

void turn_left(int speed)
{
	 MOTOR_DRIVER_DIRECTION_PORT &= ~( 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 );
	 MOTOR_DRIVER_DIRECTION_PORT |= 1 << 3 | 1 << 0;                         //전진모드
	 OCR1A = 0;                   //왼쪽 바퀴
	 OCR1B = speed;               //오른쪽 바퀴
}

void turn_right(int speed)
{
	MOTOR_DRIVER_DIRECTION_PORT &= ~( 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 );
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << 3 | 1 << 0;                         //전진모드
	OCR1A = speed;             //왼쪽 바퀴
	OCR1B = 0;		          //오른쪽 바퀴
}
