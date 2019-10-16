/*
 * time.h
 *
 * Created: 2019-07-02 오전 10:29:38
 *  Author: LYW
 */ 


#ifndef TIME_H_
#define TIME_H_

#include "main.h"

void timer0_Init(void);
void incMilliSec(void);
uint32_t millis(void);
void incTime(void);
uint8_t getSecond();
#endif /* TIME_H_ */