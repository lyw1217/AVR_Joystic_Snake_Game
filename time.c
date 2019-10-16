/*
* time.c
*
* Created: 2019-07-02 오전 9:51:35
*  Author: LYW
*/
#include "time.h"

static uint32_t milliSec = 0; // 전역변수로 milliSec 선언
static uint8_t ss = 0; // 전역변수로 초 선언
// static => time.c에서만 사용하게끔 하겠다. 안정성을 위해 외부에서 건드리지 못하게 하겠다.
// 함수 내에서의 static의 역할과 혼동하면 안된다.

void timer0_Init(void){
	// CS0n 분주비 0/1/0(8), 0/1/1(32), 1/0/0(64), 1/0/1(128), 1/1/0(256), 1/1/1(1024)
	TCCR0 |= (1<<CS02) | (0<<CS01) | (0<<CS00);
	// CTC Mode 사용
	TCCR0 |= (1<<WGM01) | (0<WGM00);
	// Timer/Counter0 Compare Interrupt Enable
	// 비교일치 인터럽트 활성화
	TIMSK |= (1<<OCIE0);
	// CTC Mode 이므로  TCNT가 OCR값과 같아지면 인터럽트 발생
	OCR0 = 250 - 1;
	// 클럭이 250번 발생할 때마다 OCR == TCNT이므로 0.001초마다 인터럽트 발생
	// 오버플로우 인터럽트에서 TCNT0 = 256 - 250;과 같은 의미
}

// milliSec를 증가시키는 함수
void incMilliSec(void){
	milliSec++;
}

// milliSec를 반환하는 함수
uint32_t millis(void){
	return milliSec;
}

// 1초 단위로 증가하는 시간 함수
void incTime(void){
	if(milliSec % 1000 == 0){
		ss++;
		if(ss > 59) ss = 0;
	}
}

uint8_t getSecond(){ return ss;}