/*
 * button.c
 *
 * Created: 2019-07-02 오후 6:45:01
 *  Author: LYW
 */ 

#include "button.h"

void button_Init(){
	  DDR_BUTTON &= ~((1<<PIN_JOY) | (1<<PIN_DIFF) | (1<<PIN_REST));
}

// 버튼이 눌리면 1을 리턴, 눌리지 않으면 0을 리턴

uint8_t joybutton_State(){
	static uint8_t prevState = 1; // static을 선언하면 전역변수처럼, 처음만 설정되고 다음부터는 기존 값을 그대로 사용한다.
	
	_delay_ms(10); // 채터링 예방
	
	if(((PIN_BUTTON & (0x01 << PIN_JOY)) == 0) && (prevState == 1)){ // 버튼 close 이면서 prevState == 1 --> 버튼이 처음 눌린 상태
		prevState = 0;
		return 0;
	}
	else if (((PIN_BUTTON & (0x01 << PIN_JOY)) == (0x01 << PIN_JOY)) && (prevState == 0)){ // 버튼이 open 이면서 prevState == 0 --> 버튼이 처음 눌렸다가 떼졌을 때
		prevState = 1;
		return 1;
	}
	return 0;
}

uint8_t diff_button_State(){
	static uint8_t prevState_2 = 1; // static을 선언하면 전역변수처럼, 처음만 설정되고 다음부터는 기존 값을 그대로 사용한다.
	
	_delay_ms(10); // 채터링 예방
	
	if(((PIN_BUTTON & (0x01 << PIN_DIFF)) == 0) && (prevState_2 == 1)){ // 버튼 close 이면서 prevState == 1 --> 버튼이 처음 눌린 상태
		prevState_2 = 0;
		return 0;
	}
	else if (((PIN_BUTTON & (0x01 << PIN_DIFF)) == (0x01 << PIN_DIFF)) && (prevState_2 == 0)){ // 버튼이 open 이면서 prevState == 0 --> 버튼이 처음 눌렸다가 떼졌을 때
		prevState_2 = 1;
		return 1;
	}
	return 0;
}

uint8_t restart_button_State(){
	static uint8_t prevState_3 = 1; // static을 선언하면 전역변수처럼, 처음만 설정되고 다음부터는 기존 값을 그대로 사용한다.
	
	_delay_ms(10); // 채터링 예방
	
	if(((PIN_BUTTON & (0x01 << PIN_REST)) == 0) && (prevState_3 == 1)){ // 버튼 close 이면서 prevState == 1 --> 버튼이 처음 눌린 상태
		prevState_3 = 0;
		return 0;
	}
	else if (((PIN_BUTTON & (0x01 << PIN_REST)) == (0x01 << PIN_REST)) && (prevState_3 == 0)){ // 버튼이 open 이면서 prevState == 0 --> 버튼이 처음 눌렸다가 떼졌을 때
		prevState_3 = 1;
		return 1;
	}
	return 0;
}