/*
 * main.h
 *
 * Created: 2019-07-23 오전 10:24:14
 *  Author: kccistc
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "adc.h"
#include "time.h"
#include "button.h"
#include "dotmatrix.h"
#include "uart0.h"

void reset_Game();
uint8_t getRandNum();
void display_Set_Diff(uint16_t _prevMillisec);
void tracking_Head();
void make_Food();

#endif /* MAIN_H_ */