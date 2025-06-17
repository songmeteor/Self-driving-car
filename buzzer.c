/*
 * buzzer.c
 *
 * Created: 2025-03-21 오전 9:44:18
 *  Author: microsoft
 */ 
#include "buzzer.h"
#include "extern.h"

void init_buzzer(void);
void Beepo(void);
void GrandFa(void);

volatile uint8_t beepo_idx = 0;
volatile uint8_t grandfa_idx = 0;

void init_buzzer(void)
{
	// PE3(OC3A)를 출력으로 설정
	BUZZER_DDR |= 1 << BUZZER_PIN;
	
	TCCR3A = (1<<COM3A0); 
	TCCR3B = (1<<WGM32) | (1<<CS31);
	
	TCCR3C = 0;
	OCR3A = 0;  
}

void Beepo(void)
{
	int Fire_Truck_Tune[] = {FA_01, LA_01_H,'\0'};
	const int Fire_Truck_Beats[] = {BEAT_1_2, BEAT_1_2,'\0'};
	
	if (is_buzzer_playing == 0)  
	{
		if (Fire_Truck_Beats[beepo_idx] == '\0')
		{
			beepo_idx = 0;
			return;
		}
		OCR3A = Fire_Truck_Tune[beepo_idx];
		is_buzzer_playing = 1;
		buzzer_delay = Fire_Truck_Beats[beepo_idx];
		beepo_idx++;
	}
	return;
}

void GrandFa(void)
{
	int GrandFa_Tune[] = {SO_01, DO_02, TI_01, DO_02, RE_02, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, 0,
		SO_01, DO_02, TI_01, DO_02, RE_02, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, 0,
		DO_02, MI_02, SO_02, MI_02, RE_02, DO_02, TI_01, DO_02, RE_02, DO_02, TI_01, LA_01, SO_01, DO_02, MI_02, SO_02, MI_02, RE_02, DO_02, TI_01, DO_02, RE_02,
	SO_01, DO_02, RE_02, MI_02, FA_02, MI_02, LA_01, RE_02, RE_02, DO_02, DO_02, DO_02, TI_01, LA_01, TI_01, DO_02, '/0'};

	const int GrandFa_Beats[] = {BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_4,
		BEAT_1_4, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2, BEAT_1_4,
		BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4,
	BEAT_1_4, BEAT_1_2, BEAT_1_2, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_4, BEAT_1_8, BEAT_1_8, BEAT_1_2, '\0'};
	
	if (is_buzzer_playing == 0)
	{
		if (GrandFa_Beats[grandfa_idx] == '\0')
		{
			grandfa_idx = 0;
			return;
		}
		OCR3A = GrandFa_Tune[grandfa_idx];
		is_buzzer_playing = 1;
		buzzer_delay = GrandFa_Beats[grandfa_idx];
		grandfa_idx++;
	}
	return;
}
