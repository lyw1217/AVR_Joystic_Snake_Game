/*
 * button.h
 *
 * Created: 2019-07-02 오후 6:45:09
 *  Author: kccistc
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#define F_CPU 16000000UL
#define DDR_BUTTON	DDRG
#define PIN_BUTTON	PING
#define PIN_JOY		PING0
#define PIN_DIFF	PING1
#define PIN_REST	PING2

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void button_Init();
uint8_t joybutton_State();
uint8_t diff_button_State();
uint8_t restart_button_State();

#endif /* BUTTON_H_ */