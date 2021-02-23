//Snake Game 1.2
#include <stdio.h>
#include <windows.h>		//system("cls");
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>		//strcpy()

//<!>아래 2개 작동 안됨 
#include <mmsystem.h>	//Multimedia Sytem
#pragma comment(lib, "winmm.lib")	//프로그램 사용, 윈도우미디어프로그램(.wav only) 

//https://freesoung.org
//전처리지시자 : 상수 or 매크로 정의 
#define LEFT 75					
#define RIGHT 77
#define UP 72
#define DOWN 80
#define PAUSE 112
#define ESC 27
 
#define MAP_X 3
#define MAP_Y 2
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

#define _C4 261.6256
#define _D4 293.6648
#define _E4 329.6276
#define _F4 349.2282
#define _G4 391.9954
#define _A4 440.0000
#define _B4 493.8833
#define _C5 523.2544

#define BGM_PATH "bgm.wav"		//BGM 파일 경로 
 
int x[100], y[100]; //x,y 좌표값을 저장 총 100개 
int food_x, food_y; //food의 좌표값을 저장 
int length; 		//몸길이를 기억 
int speed; 			//게임 속도 
int score; 			//점수 저장  --reset함수에 의해 초기화됨
int best_score=0; 	//최고 점수 저장 --reset함수에 의해 초기화 되지 않음 
char best_user[100];
int last_score=0; 	//마지막 점수 저장  --reset함수에 의해 초기화 되지 않음
int dir; 			//이동방향 저장 
int key; 			//입력받은 키 저장 
int status_on=0; 	// 개발자용 status 표시활성화 변수.. 게임중에 S키를 누르면 활성 
char user_name[100];
 
//x값을 2x로 변경, 좌표값에 바로 문자열을 출력하는 함수 
void gotoxy(int x,int y,char* s) { 
	HANDLE hConsole;		//HANDLE 구조체 변수 선언 
    COORD pos={2*x,y}; 		//COORD 구조체 변수 선언 
    
    //GetStdHandle(STD_OUTPUT_HANDLE)
	//표준 장치(표준 입력, 표준 출력, 표준 오류)의 핸들값을 받아오는 함수
	//보안상의 이유로 프로그램 장치의  주소값을 직접 주지 않고, 핸들값 전달함 
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
    
	SetConsoleCursorPosition(hConsole,pos);   //지정된 콘솔 화면 버퍼에서 커서 위치를 설정하는 함수
    //hConsole(hConsoleOutput) : 콘솔 화면 버퍼에 대한 핸들. 핸들에는 GENERIC_READ 권한이 있어야 함
	//pos(dwCursorPosition) : 새 커서 위치를 지정하는 COORD 구조체. 좌표는 화면 버퍼 문자 셀의 열과 행

    printf("%s",s);
}
// 함수 선언 
void title(void); 		//게임 시작화면 
void reset(void); 		//게임을 초기화 
void draw_map(void); 	//게임판 테두리를 그림 
void move(int dir); 	//뱀머리를 이동 
void pause(void); 		//일시정지 
void game_over(void); 	//게임 오버를 확인 
void food(void); 		//음식 생성 
void status(void); 		//개발자용 status표시
void readwrite_record(char mode);  	//파일에서 최고점 불러오기 
void game_effect(int mode);			//Game Effect 
 
////////////////////////////MAIN START//////////////////////////////
int main(void){
	readwrite_record('r');	//레코드 파일 불러오기 
    title();				//게임 시작 화면 보이기 
    
    while(1){
        if(kbhit()){		//kbhit() : 키보드 눌림 유무 반환 
			do{
				key=getch();	//입력받은 키를 key 에 저장(방향키는 키 값 2개 읽어들임) 
			} while(key==224); 	//첫번째 입력받은 값이 224이면 두번째 입력받은 값을 key에 저장하고 반복문 종료 
    	}
    	//방향키를 비롯한 몇몇 키들은 확장 아스키 코드 : 일반 아스키 코드와는 다르게 2byte를 사용하여 표현.
		//따라서 getch() 함수를 사용하여 방향키 값을 읽어들이면 두 개의 값 반환(첫번째 값 224).
		//방향키의 아스키 값
			// ↑: 224, 72
			// ↓: 224, 80
			// ←: 224, 75
			// →: 224, 77
		
		Sleep(speed);		//대기, ms단위, 초기값:100(0.1s) 
        
        switch(key){ //입력받은 키를 파악하고 실행  
            case LEFT:
            case RIGHT:
            case UP:
            case DOWN:
                if((dir==LEFT&&key!=RIGHT)||	//Left방향이고 Right키가 눌린게 아니거나(반대 방향 키를 누른게 아니라면)	
					(dir==RIGHT&&key!=LEFT)||
					(dir==UP&&key!=DOWN)||
					(dir==DOWN&&key!=UP))		//180회전 이동이 아니라면 
                    dir = key;					//누른키의 방향을 진행방향에 저장 
                
				key=0; 	//키값 reset 
            	break;
            case PAUSE: 	// P키를 누르면 일시정지 
                pause();	// 일시정지 사용자 함수 
            	break;
            case 115: 		//S키를 누르면 개발자용 status를 활성화 시킴  
                if(status_on==0) status_on=1;
                else status_on=0;
                key=0;
                break;
            case ESC: //ESC키를 누르면 프로그램 종료 
                exit(0);
        }
        move(dir); 		//뱀 이동, 방향 전환 포함 
        
        if(status_on==1) status(); // status표시 
    }    
    return 0;
}
    
///////////////////////////MAIN END////////////////////////////////

//=== 게임 시작 화면 
void title(void){
    int i,j;
    
    system("mode con cols=73 lines=25"); 	//콘솔 창 크기 변경(콘솔 명령어) 
	//mode con : Console Mode, 콘솔 제어
	//cols : 가로 크기(Width), lines : 세로 크기(Height)
    
    while (kbhit()) getch(); //버퍼에 있는 키값을 버림 
     
    draw_map();    //맵 테두리를 그림 
    
    //<!> 이거 작동 안됨 
	//PlaySound(TEXT(BGM_PATH), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//undefined reference to __imp_ playsound'
	//Dev-C++ uses MingW.
	//import #pragma comment <File> <= DevC++ 에서#pragma가 동작 안되는 것 같음 
	//On Dev-C++ go to Project Options and then on the Parameters tab
	//그러나, 사운드 작동 안됨 
    
	// 맵 테두리 안쪽을 빈칸으로 채움 
    for(i=MAP_Y+1;i<MAP_Y+MAP_HEIGHT-1;i++){ 						//MAP_Y=2, MAP_HEIGHT = 20
        for(j=MAP_X+1;j<MAP_X+MAP_WIDTH-1;j++) gotoxy(j,i,"  ");	//MAP_X=3, MAP_WEIGHT = 30
    }
 
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+4,"+--------------------------+");
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+5,"|        S N A K E         |");
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+6,"+--------------------------+");
 
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+8,"     < INPUT YOUR NAME > ");
 
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+10,"   ◇ ←,→,↑,↓ : Move    ");
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+11,"   ◇ P : Pause             ");    
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+12,"   ◇ ESC : Quit              ");    
 
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+16," BEST SCORE : ");
	printf("%s의 %d점", best_user, best_score);    
    
	gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+17,"      INPUT NAME : ");
	
    game_effect(1);		//게임 시작 사운드 출력 
    
    scanf("%s", user_name);    
	
	gotoxy(MAP_X,MAP_Y+MAP_HEIGHT," YOUR SCORE: "); 	//점수표시 
    printf("%3d, LAST SCORE: %3d, BEST SCORE: %s의 %3d", score, last_score, best_user, best_score);
		
	while(1){
        if(kbhit()){ 				//키입력받음 
            key=getch();			//아무키나 눌려지면 게임 시작 
            if(key==ESC) exit(0); 	// ESC키면 종료 
            else break; 			//아니면 그냥 계속 진행 
        } 
        gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+8," < PRESS ANY KEY TO START > ");
        Sleep(400);		//깜빡 거림 
        gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+8,"                            ");
        Sleep(400);
    }
    reset(); // 게임을 초기화하고 시작  
}
 
//===게임 초기화 
void reset(void){
    int i;
    system("cls"); 				//화면을 지움
    draw_map(); 				//맵 테두리를 그림 
    while (kbhit()) getch(); 	//버퍼에 있는 키값을 버림 
    
    dir=LEFT; 	// 방향 초기화  
    speed=100; 	// 속도 초기화 
    length=5; 	//뱀 길이 초기화 
    score=0; 	//점수 초기화 
    
    for(i=0;i<length;i++){ 		//뱀 몸통값 입력 
        x[i]=MAP_WIDTH/2+i;		//뱀 X좌표, X축 중앙부터 i값만큼 이동하면서 "O "그리기 
        y[i]=MAP_HEIGHT/2;		//뱀 Y좌표, Y축 중앙 
        gotoxy(MAP_X+x[i],MAP_Y+y[i],"ㅇ");
    }
    gotoxy(MAP_X+x[0],MAP_Y+y[0],"ㅎ"); //뱀 머리 그림 
    food(); 							// food 생성  
}

//=== 맵 테두리 그리기 
void draw_map(void){ 
    int i,j;
    for(i=0;i<MAP_WIDTH;i++){
        gotoxy(MAP_X+i,MAP_Y,"■");
    }
    for(i=MAP_Y+1;i<MAP_Y+MAP_HEIGHT-1;i++){
        gotoxy(MAP_X,i,"■");
        gotoxy(MAP_X+MAP_WIDTH-1,i,"■");
    }
    for(i=0;i<MAP_WIDTH;i++){
        gotoxy(MAP_X+i,MAP_Y+MAP_HEIGHT-1,"■");
    }
}

//=== 뱀 움직이기(입력:방향) 
void move(int dir){
    int i;
    
 	//===== <  food와 충돌했을 경우 > =====
    if(x[0]==food_x && y[0]==food_y){ 	
        Beep(260, 100);					//도,Beep(음높이, 음지속시간(ms)) 
		score+=10; 						//점수 증가 
        food(); 						//새로운 food 추가 
        length++; 						//길이증가 
        x[length-1] = x[length-2]; 		//새로 만든 몸통에 값 입력 
        y[length-1] = y[length-2];
    }
    //===== < 벽과 충돌했을 경우 > ===== 
    if(x[0]==0 || x[0]==MAP_WIDTH-1 || y[0]==0 || y[0]==MAP_HEIGHT-1){ 
        game_over();
        return; //game_over에서 게임을 다시 시작하게 되면 여기서부터 반복되므로 
                //return을 사용하여 move의 나머지 부분이 실행되지 않도록 합니다. 
    }
    //===== < 자기몸과 충돌했는지 검사 > =====
    for(i=1 ; i<length ; i++){ 
        if(x[0]==x[i] && y[0]==y[i]){
            game_over();
            return;
        }
    }    
    // 몸통 마지막을 지움
    gotoxy(MAP_X+x[length-1], MAP_Y+y[length-1], "  ");  
    for(i=length-1 ; i>0 ; i--){ //몸통좌표를 한칸씩 옮김 
        x[i] = x[i-1];
        y[i] = y[i-1];
    }    
    gotoxy(MAP_X+x[0], MAP_Y+y[0], "ㅇ"); //머리가 있던곳을 몸통으로 고침 
    if(dir==LEFT) --x[0]; //방향에 따라 새로운 머리좌표(x[0],y[0])값을 변경 
    if(dir==RIGHT) ++x[0];
    if(dir==UP) --y[0]; 
    if(dir==DOWN) ++y[0];     
    gotoxy(MAP_X+x[i],MAP_Y+y[i],"ㅎ"); //새로운 머리좌표값에 머리를 그림 
}

//=== 게임 일시 정지  : p키를 눌렀을 경우 
void pause(void){ 	
    while(1){
        if(key==PAUSE){	//화면 일시 정지 
            gotoxy(MAP_X+(MAP_WIDTH/2)-8,MAP_Y,"< PAUSE : PRESS ANY KEY TO RESUME > ");
            Sleep(400);
            gotoxy(MAP_X+(MAP_WIDTH/2)-8,MAP_Y,"                                    ");
            Sleep(400);
        }else{
            draw_map();		//맵 테두리 그리기 
            return;
        }
		 
        if(kbhit()){	//키가 눌렸을 경우 
            do{
                key=getch();
            }while(key==224);
        }            
    }
}

//=== 최고점수 파일에 저장하기 또는 읽어오기 
void readwrite_record(char mode){
	FILE *fp = NULL;
	switch(mode){
		case 'r':
			fp = fopen("record.bin","rb");		//notepad++에서 bin파일 읽기 
			if(fp == NULL){						//파일 열기 실패(파일이 없을 경우) 
				fclose(fp);						//열었던 파일스트임 종료 후, 
			
				fp = fopen("record.bin","wb");	//쓰시 파일스트림을 새로 생성(파일 자동 생성) 
				fprintf(fp,"%s %d","머구리씨", best_score);	//Writes the C string pointed by format to the stream
				fclose(fp);
				//exit(0);	//Exit Success, exit(1);-Exit Failure
				break;
			}
			fscanf(fp,"%s %d", best_user, &best_score);	//파일에서 읽어서 변수에 저장 
			
			fclose(fp);
			break;
		case 'w':
			fp = fopen("record.bin","wb");
			if(fp == NULL){
				printf("파일 열기 실패");
				exit(0);	//Exit Success, exit(1);-Exit Failure
			}
			fprintf(fp,"%s %d", best_user, best_score);	//파일에 저장 
			
			fclose(fp);
			break;
	}	
}

void game_effect(int mode){
	switch(mode){
		case 1:
			Beep(_C4, 200);	//도, Beep(음높이, 음지속시간(ms)) 
			Beep(_D4, 200);	//레 
			Beep(_E4, 200);	//미 
			Beep(_F4, 200);	//파 
			Beep(_G4, 200);	//솔 
			Beep(_A4, 200);	//라 
			Beep(_B4, 200);	//시 
			Beep(_C5, 200);	//도	 
			break;
		case 2:
			Beep(_C5, 200);	//도	 
			Beep(_B4, 200);	//시 
			Beep(_A4, 200);	//라 
			Beep(_G4, 200);	//솔
			Beep(_F4, 200);	//파
			Beep(_E4, 200);	//미 
			Beep(_D4, 200);	//레
			Beep(_C4, 200);	//도
	}
} 
//########################################################
//##############  [함수] 게임 종료 ####################### 
//########################################################

void game_over(void){ 
	game_effect(2);
    gotoxy(MAP_X+(MAP_WIDTH/2)-6,MAP_Y+5,"+----------------------+");
    gotoxy(MAP_X+(MAP_WIDTH/2)-6,MAP_Y+6,"|      GAME OVER..     |");
    gotoxy(MAP_X+(MAP_WIDTH/2)-6,MAP_Y+7,"+----------------------+");
    gotoxy(MAP_X+(MAP_WIDTH/2)-6,MAP_Y+8," YOUR SCORE : ");
    printf("%d", last_score=score);
    
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+12," Press any keys to restart.. ");
    
    if(score > best_score) {	//내 점수가 최고점수보다 높으면 
        best_score = score;		//내 점수를 최고점수에 저장 
        strcpy(best_user, user_name);		//user_name을 best_user로 복사 
    	gotoxy(MAP_X+(MAP_WIDTH/2)-4,MAP_Y+10,"☆ BEST SCORE ☆");
    }
    readwrite_record('w');			//파일에 저장하기 
    Sleep(500);
    
	while (kbhit()) getch();		//키가 눌려지면 
    key=getch();
    title();						//게임 시작 
}
 
void food(void){
    int i;
    
    int food_crush_on=0;	//food가 뱀 몸통좌표에 생길 경우 on(1) 
    int r=0; 				//난수 생성에 사동되는 변수 
    gotoxy(MAP_X,MAP_Y+MAP_HEIGHT," YOUR SCORE: "); 	//점수표시 
    printf("%3d, LAST SCORE: %3d, BEST SCORE: %s의 %3d", score, last_score, best_user, best_score);
    
    while(1){            
        food_crush_on=0;    

        srand((unsigned)time(NULL)+r); 		//난수표생성 
        
        food_x=(rand()%(MAP_WIDTH-2))+1;    //난수를 좌표값에 넣음 
        food_y=(rand()%(MAP_HEIGHT-2))+1;
		        
        for(i=0;i<length;i++){ 				//food가 뱀 몸통과 겹치는지 확인  
            if(food_x==x[i]&&food_y==y[i]){
                food_crush_on=1; 			//겹치면 food_crush_on 를 on 
                r++;
				break;
            }
        }
        
        if(food_crush_on==1) continue; 			//겹쳤을 경우 while문을 다시 시작 
            
        gotoxy(MAP_X+food_x,MAP_Y+food_y,"♪"); //안겹쳤을 경우 좌표값에 food를 찍고 
        speed-=3; 								//속도 증가 
        break;        
    }
}
 
void status(void){ 		//각종 스텟을 볼수 있는 함수 
    gotoxy(MAP_X+MAP_WIDTH+1,MAP_Y,"head= ");
    printf("%2d,%2d",x[0],y[0]);
    gotoxy(MAP_X+MAP_WIDTH+1,MAP_Y+1,"food= ");
    printf("%2d,%2d",food_x,food_y);
    gotoxy(MAP_X+MAP_WIDTH+1,MAP_Y+2,"leng= ");
    printf("%2d",length);
    gotoxy(MAP_X+MAP_WIDTH+1,MAP_Y+3,"key= ");
    printf("%3d",key);
    gotoxy(MAP_X+MAP_WIDTH+1,MAP_Y+4,"spd= ");
    printf("%3d",speed);
    gotoxy(MAP_X+MAP_WIDTH+1,MAP_Y+6,"score= ");
    printf("%3d",score);  
}
