/*
 * fnd.c
 *
 * Created: 2025-03-06 오후 12:23:41
 *  Author: microsoft
 */ 
#include "fnd.h"
#include "def.h"
#include "extern.h"

void init_fnd(void);
void fnd_display(int speed, int func_index);

void fnd_display(int speed, int func_index)
{		
    uint8_t fnd_font[] = {~0xc0,~0xf9,~0xa4,~0xb0,~0x99,~0x92,~0x82,~0xd8,~0x80,~0x90,~0x7f};
    
	FND_DIGIT_PORT |= (1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4);
	
	static int digit_select=0; 
	
	switch(digit_select)
	{
		case 0:
			FND_DIGIT_PORT &= ~(1 << FND_DIGIT_D4);  
			FND_DATA_PORT = fnd_font[speed % 10]; 
			break;
		case 1:
			FND_DIGIT_PORT &= ~(1 << FND_DIGIT_D3);
			FND_DATA_PORT = fnd_font[speed / 10  %10];  
			break;
		case 2:
			FND_DIGIT_PORT &= ~(1 << FND_DIGIT_D2); 
			FND_DATA_PORT = fnd_font[speed / 100 ];  
			break;
		case 3:
			FND_DIGIT_PORT &= ~(1 << FND_DIGIT_D1);
			FND_DATA_PORT = func_index == MANUAL_MODE ? 0x6D : ~0x88;   
			break;
	}
	digit_select++;
	digit_select %= 4;   // 다음 표시할 자리수 선택
}

void init_fnd(void)
{
	FND_DATA_DDR = 0xff;  // 출력 모드로 설정
	// FND_DIGIT_DDR |= 0xf0;   // 자릿수 선택 7654 
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 
					 | 1 << FND_DIGIT_D4; 
	FND_DATA_PORT = 0x00;   // fnd를 all off  
}