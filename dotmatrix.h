/*
 * dotmatrix.h
 *
 * Created: 2019-07-25 오후 2:38:18
 *  Author: kccistc
 */ 


#ifndef DOTMATRIX_H_
#define DOTMATRIX_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define REGISTER_DDR DDRE
#define REGISTER_PORT PORTE

// 'bit' 위치의 비트를 1 또는 0으로 설정하기 위한 매크로 함수
#define set_bit(bit) ( REGISTER_PORT |= _BV(bit) ) // _BV = (1 << bit)
#define clear_bit(bit) ( REGISTER_PORT &= ~_BV(bit) )

// 74595 핀이 ATmega에 연결된 위치
#define SHIFT_CLOCK 7
#define LATCH_CLOCK 6
#define DATA 5

#define COL_ON 0
#define COL_OFF 1
#define ROW_ON 1
#define ROW_OFF 0

extern uint8_t txt_dead[];
extern uint8_t txt_deadsize;
extern uint8_t txt_play[];
extern uint8_t txt_playsize;
extern uint8_t txt_num[];
extern uint8_t txt_numsize;
extern uint8_t txt_success[];
extern uint8_t txt_successsize;
extern uint8_t txt_welcome[];
extern uint8_t txt_welcomesize;
extern uint8_t txt_score[];
extern uint8_t txt_scoresize;

//uint8_t get_arr_size();

void ShiftClock(void);
void LatchClock(void);
void ByteDataWrite(uint8_t data);
void dotMatrix_Init();
void slide_Data_Welcome(int pattern_index);
void slide_Data_Dead(int pattern_index);
void slide_Data_Play(int pattern_index);
void slide_Data_Success(int pattern_index);
void slide_Data_Score(int pattern_index, uint8_t _eat_count);
void print_Data_Num(uint8_t num_index, uint8_t diff_num);
void display_Dot(uint8_t _col, uint8_t _row, uint8_t _f_col, uint8_t _f_row, uint8_t _head_Coord[][2], uint8_t _eat_count);
#endif /* DOTMATRIX_H_ */