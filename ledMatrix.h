/*
 * ledMatrix.h
 *
 * Created: 2019-07-23 오후 5:33:43
 *  Author: kccistc
 */ 


#ifndef LEDMATRIX_H_
#define LEDMATRIX_H_

#define F_CPU 16000000UL

#define DDR_ROW	DDRE
#define DDR_COL	DDRD
#define PORT_ROW	PORTE
#define PORT_COL	PORTD

#define COL_ON	0
#define COL_OFF	1
#define ROW_ON	1
#define ROW_OFF	0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void write_column_data(uint8_t data);
void write_row_data(uint8_t data);
void ledMatrix_Init();
void dot_print_Play();
void dot_print_Dead();
void dot_print_Count();
void dot_print_Number(uint8_t num);

#endif /* LEDMATRIX_H_ */