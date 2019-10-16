/*
* ledMatrix.c
*
* Created: 2019-07-23 오후 5:33:26
*  Author: LYW
*/

#include "ledMatrix.h"

uint8_t alpha_play[4][8] = {
	{0x7C,0x7E,0x66,0x66,0x7E,0x7C,0x60,0x60},	// P
	{0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x7E},	// L
	{0x18,0x3C,0x66,0x66,0x7E,0x7E,0x66,0x66},	// A
	{0x66,0x66,0x66,0x66,0x3C,0x18,0x18,0x18}	// Y
};

uint8_t alpha_dead[4][8] = {
	{0x78,0x7C,0x66,0x66,0x66,0x66,0x7C,0x78},	// D
	{0x7E,0x7E,0x60,0x7C,0x7C,0x60,0x7E,0x7E},	// E
	{0x18,0x3C,0x66,0x66,0x7E,0x7E,0x66,0x66},	// A
	{0x78,0x7C,0x66,0x66,0x66,0x66,0x7C,0x78}	// D
};

uint8_t number[3][8] = {
	{0x08,0x18,0x28,0x08,0x08,0x08,0x08,0x3C},	// 1
	{0x18,0x24,0x24,0x08,0x10,0x20,0x20,0x3C},	// 2
	{0x3C,0x04,0x04,0x18,0x04,0x04,0x24,0x18}	// 3
};

void write_column_data(uint8_t data){
	PORT_COL = data;
}

void write_row_data(uint8_t data){
	PORT_ROW = data;
}

void ledMatrix_Init(){
	DDR_ROW = 0xFF;					// 출력으로 설정
	write_row_data(0xFF * ROW_OFF); // OFF 값 출력
	
	DDR_COL = 0xFF;
	write_column_data(0xFF * COL_OFF);
}


void dot_print_Play(){
	
	uint8_t col_data;
	static uint8_t col = 0;
	static uint16_t count = 0;
	
	if(count > 1000){
		count = 0;
		col++;
		if(col > 3) col = 0;
	}
	
	// col 단위로 한줄씩 출력
	for(int i = 0; i < 8; i++){
		col_data = ~(1 << i);
		
		write_column_data(col_data);
		write_row_data(alpha_play[col][i]);
		_delay_ms(1);
		
		PORTE = 0x00;
		count++;
	}
}

void dot_print_Dead(){
	
	uint8_t col_data;
	static uint8_t col = 0;
	static uint16_t count = 0;
	
	if((count/8) > 100){
		count = 0;
		col++;
		if(col > 3) col = 0;
	}
	
	// col 단위로 한줄씩 출력
	for(int i = 0; i < 8; i++){
		col_data = ~(1 << i);
		
		write_column_data(col_data);
		write_row_data(alpha_dead[col][i]);
		_delay_ms(1);
		
		PORTE = 0x00;
		count++;
	}
}

void dot_print_Count(){
	
	uint8_t smile[8][8] = {
	{0,0,1,1,1,1,0,0},
	{0,1,0,0,0,0,1,0},
	{1,0,1,0,0,1,0,1},
	{1,0,0,0,0,0,0,1},
	{1,0,1,0,0,1,0,1},
	{1,0,0,1,1,0,0,1},
	{0,1,0,0,0,0,1,0},
	{0,0,1,1,1,1,0,0}
	};
	
	// col 변경해가며 row 하나씩 출력	
	for(int row = 7; row >= 0; row--){
		for(int col = 0; col < 8; col++){
			write_column_data(~(1 << col));
			write_row_data(smile[col][row] << row);
			_delay_ms(50);
		}
	}
}

void dot_print_Number(uint8_t num){
	
	uint8_t col_data;
	for(int i = 0; i < 8; i++){
		col_data = ~(1 << i);
		
		write_column_data(col_data);
		write_row_data(number[num][i]);
		_delay_us(500);
		
		PORTE = 0x00;
	}
}

void dot_scrl_Dead(){
	
	static uint8_t buff[8][8] = {0,};
		
	uint8_t col_data;
	// col 단위로 한줄씩 출력
	for(int col = 0; col < 3; col++){
		for(int i = 0; i < 8; i++){
			col_data = ~(1 << i);
			write_column_data(col_data);
			write_row_data(alpha_dead[col][i]);
			_delay_ms(1);
			
			PORTE = 0x00;
		}
	}
}