/*
* Joystick_snake_game.c
*
* Created: 2019-07-23 오전 10:20:55
* Author : LYW
*/

#include "main.h"

#define JOY_VRX		PORTF0
#define JOY_VRY		PORTF1

#define SIZE 64

enum { UPSIDE, DOWNSIDE, RIGHTSIDE, LEFTSIDE } DIRECTION;
enum { ALIVE, DEAD } LIFE;
enum { PLAY, STOP, START, SUCCESS } STATE;

ISR(TIMER0_COMP_vect){
	incMilliSec();
	incTime();
}

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

int8_t col = 0, row = 7;		// 뱀머리 위치 초기화
uint8_t f_col, f_row;			// 먹이 좌표
uint8_t head_Coord[SIZE][2];	// 뱀 머리 좌표 저장
uint8_t eat_count = 0;			// 먹이 먹은 횟수
uint16_t diff[] = {300, 200, 100};	// 난이도 배열
uint8_t diff_num = 0;			// 초기 난이도 200ms

// slide 구현을 위한 index
uint8_t dead_Index = 0;
uint8_t play_Index = 0;
uint8_t num_Index = 0;
uint8_t success_Index = 0;
uint8_t welcome_Index = 0;
uint8_t score_Index = 0;

uint8_t reset_count = 0;
uint8_t pcount = 0;

int main(void)
{
	timer0_Init();
	ADC_init_single();
	button_Init();
	dotMatrix_Init();
	UART0_Init();
	
	stdout = &OUTPUT;
	sei();
	
	uint16_t read_x, read_y;
	uint32_t prevMillisec = millis();	// 시간
	
	srand(prevMillisec);
	
	// 먹이 첫 위치 설정
	while(1){
		f_col = getRandNum();
		_delay_ms(1);
		f_row = getRandNum();
		if((f_col != col) && (f_row != row)) break;
	}
	
	// 머리 좌표 배열 초기화
	for(int i = 0; i < SIZE; i++){
		for(int j = 0; j < 2; j++){
			head_Coord[i][j] = 0;
		}
	}
	
	DIRECTION = RIGHTSIDE; // 처음은 아래로 움직이기 시작
	LIFE = ALIVE;
	STATE = STOP;
	for(;;){
		if((millis() - prevMillisec) > 50){
			welcome_Index = ((welcome_Index + 1)  % ( txt_welcomesize ));
			prevMillisec = millis();
		}
		slide_Data_Welcome(welcome_Index);
		if(welcome_Index == txt_welcomesize-1){
			STATE = STOP;
			break;
		}
	}
	
	while (1)
	{
		switch(STATE){
			// 처음 시작 화면 //////////////////////////////////////
			case STOP:
			// 초기 난이도 선택 화면 및 시작 대기
			display_Set_Diff(prevMillisec);
			break;
			
			// 게임 시작 전 화면 출력 //////////////////////////////
			case START:
			// 시작 화면 출력 후 플레이 시작
			if((millis() - prevMillisec) > 50){
				play_Index = ((play_Index + 1)  % ( txt_playsize ));
				prevMillisec = millis();
			}
			slide_Data_Play(play_Index);
			if(play_Index == txt_playsize-1){
				STATE = PLAY;
			}
			break;
			
			// 게임 클리어 /////////////////////////////////////////
			case SUCCESS:
			if((millis() - prevMillisec) > 50){
				success_Index = ((success_Index + 1)  % ( txt_successsize ));
				prevMillisec = millis();
			}
			slide_Data_Success(success_Index);
			if(success_Index == txt_successsize-1){
				STATE = STOP;
			}
			break;
			
			// 게임 플레이  ////////////////////////////////////////
			case PLAY:
			switch(LIFE){
				//살아있을 때 //////////////////////////////////////
				case ALIVE:
				// 조이스틱 x, y 상태 읽기
				read_x = read_ADC_single(JOY_VRX);
				read_y = read_ADC_single(JOY_VRY);
				if(read_y < 25){
					DIRECTION = DOWNSIDE;
				}
				// 조이스틱 하
				else if(read_y > 975){
					DIRECTION = UPSIDE;
				}
				// 조이스틱 좌
				else if(read_x < 25){
					DIRECTION = RIGHTSIDE;
				}
				// 조이스틱 우
				else if(read_x > 975){
					DIRECTION = LEFTSIDE;
				}
				
				
				if((millis() - prevMillisec) > (diff[diff_num])){
					// 조이스틱 방향에 따라 움직이는 부분
					switch(DIRECTION){
						case UPSIDE:
						row--;
						break;
						
						case DOWNSIDE:
						row++;
						break;
						
						case LEFTSIDE:
						col--;
						break;
						
						case RIGHTSIDE:
						col++;
						break;
					}
					prevMillisec = millis();
					
					tracking_Head();
				}
				
				// 충돌 확인부
				// 8*8 범위를 벗어나면 DEAD
				if(row >= 8 || col >= 8 || row < 0 || col < 0){
					LIFE = DEAD;
					break;
				}
				
				// 몸체에 머리가 충돌하면 DEAD
				for(;;){
					uint8_t check = 0;
					for(int i = 0; i < eat_count; i++){
						if((head_Coord[i+1][0] == col) && (head_Coord[i+1][1] == row)){
							check = 1;
							break;
						}
					}
					if(check == 1){
						LIFE = DEAD;
						break;
					}else{
						break;
					}
				}
				if(LIFE == DEAD) break;
				
				// 먹이 생성부
				make_Food();
				if(eat_count == 63){
					STATE = SUCCESS;
					break;
				}
				// 도트 출력부
				display_Dot(col, row, f_col, f_row, head_Coord, eat_count);
				break;
				
				
				// 죽었을 때 ////////////////////////////////////
				case DEAD:
				switch(pcount){
					case 0:
					if((millis() - prevMillisec) > 35){
						dead_Index = ((dead_Index + 1)  % ( txt_deadsize ));
						prevMillisec = millis();
					}
					slide_Data_Dead(dead_Index);
					
					// 1회 DEAD 문자 표시 후 점수 표시
					if(dead_Index == (txt_deadsize-1)){
						pcount = 1;
					}
					break;
					
					
					case 1:
					if((millis() - prevMillisec) > 35){
						score_Index = ((score_Index + 1)  + 8);
						prevMillisec = millis();
					}
					slide_Data_Score(score_Index, eat_count);
					reset_count++;
					
					// 일정 시간 점수 표시 후 초기상태로
					if(reset_count == 255){
						STATE = STOP;
						pcount = 0;
						break;
					}
					break;
				}
				break;
			}
			
			// play중 스탑버튼 누르면 처음으로
			if (restart_button_State() == 1){
				STATE = STOP;
			}
			break;
		}
	}
}

void reset_Game(){
	// 초기화
	col = 0;
	row = 7;
	for(int i = 0; i < SIZE; i++){
		for(int j = 0; j < 2; j++){
			head_Coord[i][j] = 0;
		}
	}
	eat_count = 0;
	dead_Index = 0;
	play_Index = 0;
	num_Index = 0;
	success_Index = 0;
	welcome_Index = 0;
	score_Index = 0;
	
	reset_count = 0;
	pcount = 0;
	// 초기화 끝
	
	STATE = START;
	LIFE = ALIVE;
	DIRECTION = RIGHTSIDE; // 처음은 아래로 움직이기 시작
}

uint8_t getRandNum(){
	uint8_t num;
	num = rand() % 8; // 0부터 8까지 랜덤한 수 생성
	return num;
}

void display_Set_Diff(uint16_t _prevMillisec){
	dead_Index = 0;
	play_Index = 0;
	
	if((millis() - _prevMillisec) > 100){
		num_Index = ((num_Index + 1)  % ( txt_numsize ));
		_prevMillisec = millis();
	}
	print_Data_Num(num_Index, diff_num);
	
	if (restart_button_State() == 1){
		reset_Game();
	}
	// 난이도 조절부
	if(diff_button_State() == 1){
		diff_num++;
		if(diff_num > ((sizeof(diff) / sizeof(diff[0]))-1)) diff_num = 0;
	}
}

void tracking_Head(){
	for(int i = SIZE-1; i > 0; i--){
		head_Coord[i][0] = head_Coord[i-1][0];
		head_Coord[i][1] = head_Coord[i-1][1];
	}
	head_Coord[0][0] = col;
	head_Coord[0][1] = row;
}

void make_Food(){
	// 현재 col, row와 같은 값이면 안됨, 뒷 꼬리들과 같은 값이면 안됨
	// 머리가 먹이에 닿으면
	if(f_col == col && f_row == row){
		// 먹었다고 카운트하고
		eat_count++;
				
		for(;;){
			uint8_t check = 0;
			
			// 먹이의 새로운 좌표를 생성
			f_col = getRandNum();
			f_row = getRandNum();
			
			for(int i = 0; i < eat_count; i++){
				if((head_Coord[i][0] == f_col) && (head_Coord[i][1] == f_row)){
					check = 1;
					break;
				}
			}
			if(check == 1){
				continue;
			}else{
				break;
			}
		}
	}
}