/*
 * extern.h
 *
 * Created: 2025-03-20 오전 11:29:16
 *  Author: microsoft
 */ 
#ifndef EXTERN_H_
#define EXTERN_H_

extern void init_timer1(void);
extern void init_L298N(void);
extern void init_button(void);
extern void init_uart0(void);
extern void init_ultrasonic(void);
extern void init_led(void);
extern void init_buzzer(void);
extern void init_fnd(void);

extern void stop(void);
extern void forward(int speed);
extern void backward(int speed);
extern void turn_left(int speed);
extern void turn_right(int speed);

extern int get_button(int button_num, int button_pin);
extern void UART0_transmit(uint8_t data);
extern void distance_ultrasonic(void);
extern void fnd_display(int speed, int func_index);
extern void Beepo(void);
extern void GrandFa(void);

extern void I2C_LCD_init(void);
extern void LCD_display(int total_distance, int func_index);

extern volatile uint8_t rx_msg_received;
extern volatile uint8_t bt_data;
extern volatile uint8_t rx_buff[80];
extern volatile uint8_t beepo_idx;
extern volatile uint8_t grandfa_idx;

extern volatile int ultrasonic_dis[3];
extern volatile int msec_count ;
extern volatile int sec_count;
extern volatile int fnd_clk;
extern uint8_t is_buzzer_playing;
extern int func_index;
extern int buzzer_delay;
#endif /* EXTERN_H_ */