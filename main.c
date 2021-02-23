//Snake Game 1.2
#include <stdio.h>
#include <windows.h>		//system("cls");
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>		//strcpy()

//<!>�Ʒ� 2�� �۵� �ȵ� 
#include <mmsystem.h>	//Multimedia Sytem
#pragma comment(lib, "winmm.lib")	//���α׷� ���, ������̵�����α׷�(.wav only) 

//https://freesoung.org
//��ó�������� : ��� or ��ũ�� ���� 
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

#define BGM_PATH "bgm.wav"		//BGM ���� ��� 
 
int x[100], y[100]; //x,y ��ǥ���� ���� �� 100�� 
int food_x, food_y; //food�� ��ǥ���� ���� 
int length; 		//�����̸� ��� 
int speed; 			//���� �ӵ� 
int score; 			//���� ����  --reset�Լ��� ���� �ʱ�ȭ��
int best_score=0; 	//�ְ� ���� ���� --reset�Լ��� ���� �ʱ�ȭ ���� ���� 
char best_user[100];
int last_score=0; 	//������ ���� ����  --reset�Լ��� ���� �ʱ�ȭ ���� ����
int dir; 			//�̵����� ���� 
int key; 			//�Է¹��� Ű ���� 
int status_on=0; 	// �����ڿ� status ǥ��Ȱ��ȭ ����.. �����߿� SŰ�� ������ Ȱ�� 
char user_name[100];
 
//x���� 2x�� ����, ��ǥ���� �ٷ� ���ڿ��� ����ϴ� �Լ� 
void gotoxy(int x,int y,char* s) { 
	HANDLE hConsole;		//HANDLE ����ü ���� ���� 
    COORD pos={2*x,y}; 		//COORD ����ü ���� ���� 
    
    //GetStdHandle(STD_OUTPUT_HANDLE)
	//ǥ�� ��ġ(ǥ�� �Է�, ǥ�� ���, ǥ�� ����)�� �ڵ鰪�� �޾ƿ��� �Լ�
	//���Ȼ��� ������ ���α׷� ��ġ��  �ּҰ��� ���� ���� �ʰ�, �ڵ鰪 ������ 
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
    
	SetConsoleCursorPosition(hConsole,pos);   //������ �ܼ� ȭ�� ���ۿ��� Ŀ�� ��ġ�� �����ϴ� �Լ�
    //hConsole(hConsoleOutput) : �ܼ� ȭ�� ���ۿ� ���� �ڵ�. �ڵ鿡�� GENERIC_READ ������ �־�� ��
	//pos(dwCursorPosition) : �� Ŀ�� ��ġ�� �����ϴ� COORD ����ü. ��ǥ�� ȭ�� ���� ���� ���� ���� ��

    printf("%s",s);
}
// �Լ� ���� 
void title(void); 		//���� ����ȭ�� 
void reset(void); 		//������ �ʱ�ȭ 
void draw_map(void); 	//������ �׵θ��� �׸� 
void move(int dir); 	//��Ӹ��� �̵� 
void pause(void); 		//�Ͻ����� 
void game_over(void); 	//���� ������ Ȯ�� 
void food(void); 		//���� ���� 
void status(void); 		//�����ڿ� statusǥ��
void readwrite_record(char mode);  	//���Ͽ��� �ְ��� �ҷ����� 
void game_effect(int mode);			//Game Effect 
 
////////////////////////////MAIN START//////////////////////////////
int main(void){
	readwrite_record('r');	//���ڵ� ���� �ҷ����� 
    title();				//���� ���� ȭ�� ���̱� 
    
    while(1){
        if(kbhit()){		//kbhit() : Ű���� ���� ���� ��ȯ 
			do{
				key=getch();	//�Է¹��� Ű�� key �� ����(����Ű�� Ű �� 2�� �о����) 
			} while(key==224); 	//ù��° �Է¹��� ���� 224�̸� �ι�° �Է¹��� ���� key�� �����ϰ� �ݺ��� ���� 
    	}
    	//����Ű�� ����� ��� Ű���� Ȯ�� �ƽ�Ű �ڵ� : �Ϲ� �ƽ�Ű �ڵ�ʹ� �ٸ��� 2byte�� ����Ͽ� ǥ��.
		//���� getch() �Լ��� ����Ͽ� ����Ű ���� �о���̸� �� ���� �� ��ȯ(ù��° �� 224).
		//����Ű�� �ƽ�Ű ��
			// ��: 224, 72
			// ��: 224, 80
			// ��: 224, 75
			// ��: 224, 77
		
		Sleep(speed);		//���, ms����, �ʱⰪ:100(0.1s) 
        
        switch(key){ //�Է¹��� Ű�� �ľ��ϰ� ����  
            case LEFT:
            case RIGHT:
            case UP:
            case DOWN:
                if((dir==LEFT&&key!=RIGHT)||	//Left�����̰� RightŰ�� ������ �ƴϰų�(�ݴ� ���� Ű�� ������ �ƴ϶��)	
					(dir==RIGHT&&key!=LEFT)||
					(dir==UP&&key!=DOWN)||
					(dir==DOWN&&key!=UP))		//180ȸ�� �̵��� �ƴ϶�� 
                    dir = key;					//����Ű�� ������ ������⿡ ���� 
                
				key=0; 	//Ű�� reset 
            	break;
            case PAUSE: 	// PŰ�� ������ �Ͻ����� 
                pause();	// �Ͻ����� ����� �Լ� 
            	break;
            case 115: 		//SŰ�� ������ �����ڿ� status�� Ȱ��ȭ ��Ŵ  
                if(status_on==0) status_on=1;
                else status_on=0;
                key=0;
                break;
            case ESC: //ESCŰ�� ������ ���α׷� ���� 
                exit(0);
        }
        move(dir); 		//�� �̵�, ���� ��ȯ ���� 
        
        if(status_on==1) status(); // statusǥ�� 
    }    
    return 0;
}
    
///////////////////////////MAIN END////////////////////////////////

//=== ���� ���� ȭ�� 
void title(void){
    int i,j;
    
    system("mode con cols=73 lines=25"); 	//�ܼ� â ũ�� ����(�ܼ� ��ɾ�) 
	//mode con : Console Mode, �ܼ� ����
	//cols : ���� ũ��(Width), lines : ���� ũ��(Height)
    
    while (kbhit()) getch(); //���ۿ� �ִ� Ű���� ���� 
     
    draw_map();    //�� �׵θ��� �׸� 
    
    //<!> �̰� �۵� �ȵ� 
	//PlaySound(TEXT(BGM_PATH), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//undefined reference to __imp_ playsound'
	//Dev-C++ uses MingW.
	//import #pragma comment <File> <= DevC++ ����#pragma�� ���� �ȵǴ� �� ���� 
	//On Dev-C++ go to Project Options and then on the Parameters tab
	//�׷���, ���� �۵� �ȵ� 
    
	// �� �׵θ� ������ ��ĭ���� ä�� 
    for(i=MAP_Y+1;i<MAP_Y+MAP_HEIGHT-1;i++){ 						//MAP_Y=2, MAP_HEIGHT = 20
        for(j=MAP_X+1;j<MAP_X+MAP_WIDTH-1;j++) gotoxy(j,i,"  ");	//MAP_X=3, MAP_WEIGHT = 30
    }
 
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+4,"+--------------------------+");
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+5,"|        S N A K E         |");
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+6,"+--------------------------+");
 
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+8,"     < INPUT YOUR NAME > ");
 
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+10,"   �� ��,��,��,�� : Move    ");
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+11,"   �� P : Pause             ");    
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+12,"   �� ESC : Quit              ");    
 
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+16," BEST SCORE : ");
	printf("%s�� %d��", best_user, best_score);    
    
	gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+17,"      INPUT NAME : ");
	
    game_effect(1);		//���� ���� ���� ��� 
    
    scanf("%s", user_name);    
	
	gotoxy(MAP_X,MAP_Y+MAP_HEIGHT," YOUR SCORE: "); 	//����ǥ�� 
    printf("%3d, LAST SCORE: %3d, BEST SCORE: %s�� %3d", score, last_score, best_user, best_score);
		
	while(1){
        if(kbhit()){ 				//Ű�Է¹��� 
            key=getch();			//�ƹ�Ű�� �������� ���� ���� 
            if(key==ESC) exit(0); 	// ESCŰ�� ���� 
            else break; 			//�ƴϸ� �׳� ��� ���� 
        } 
        gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+8," < PRESS ANY KEY TO START > ");
        Sleep(400);		//���� �Ÿ� 
        gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+8,"                            ");
        Sleep(400);
    }
    reset(); // ������ �ʱ�ȭ�ϰ� ����  
}
 
//===���� �ʱ�ȭ 
void reset(void){
    int i;
    system("cls"); 				//ȭ���� ����
    draw_map(); 				//�� �׵θ��� �׸� 
    while (kbhit()) getch(); 	//���ۿ� �ִ� Ű���� ���� 
    
    dir=LEFT; 	// ���� �ʱ�ȭ  
    speed=100; 	// �ӵ� �ʱ�ȭ 
    length=5; 	//�� ���� �ʱ�ȭ 
    score=0; 	//���� �ʱ�ȭ 
    
    for(i=0;i<length;i++){ 		//�� ���밪 �Է� 
        x[i]=MAP_WIDTH/2+i;		//�� X��ǥ, X�� �߾Ӻ��� i����ŭ �̵��ϸ鼭 "O "�׸��� 
        y[i]=MAP_HEIGHT/2;		//�� Y��ǥ, Y�� �߾� 
        gotoxy(MAP_X+x[i],MAP_Y+y[i],"��");
    }
    gotoxy(MAP_X+x[0],MAP_Y+y[0],"��"); //�� �Ӹ� �׸� 
    food(); 							// food ����  
}

//=== �� �׵θ� �׸��� 
void draw_map(void){ 
    int i,j;
    for(i=0;i<MAP_WIDTH;i++){
        gotoxy(MAP_X+i,MAP_Y,"��");
    }
    for(i=MAP_Y+1;i<MAP_Y+MAP_HEIGHT-1;i++){
        gotoxy(MAP_X,i,"��");
        gotoxy(MAP_X+MAP_WIDTH-1,i,"��");
    }
    for(i=0;i<MAP_WIDTH;i++){
        gotoxy(MAP_X+i,MAP_Y+MAP_HEIGHT-1,"��");
    }
}

//=== �� �����̱�(�Է�:����) 
void move(int dir){
    int i;
    
 	//===== <  food�� �浹���� ��� > =====
    if(x[0]==food_x && y[0]==food_y){ 	
        Beep(260, 100);					//��,Beep(������, �����ӽð�(ms)) 
		score+=10; 						//���� ���� 
        food(); 						//���ο� food �߰� 
        length++; 						//�������� 
        x[length-1] = x[length-2]; 		//���� ���� ���뿡 �� �Է� 
        y[length-1] = y[length-2];
    }
    //===== < ���� �浹���� ��� > ===== 
    if(x[0]==0 || x[0]==MAP_WIDTH-1 || y[0]==0 || y[0]==MAP_HEIGHT-1){ 
        game_over();
        return; //game_over���� ������ �ٽ� �����ϰ� �Ǹ� ���⼭���� �ݺ��ǹǷ� 
                //return�� ����Ͽ� move�� ������ �κ��� ������� �ʵ��� �մϴ�. 
    }
    //===== < �ڱ���� �浹�ߴ��� �˻� > =====
    for(i=1 ; i<length ; i++){ 
        if(x[0]==x[i] && y[0]==y[i]){
            game_over();
            return;
        }
    }    
    // ���� �������� ����
    gotoxy(MAP_X+x[length-1], MAP_Y+y[length-1], "  ");  
    for(i=length-1 ; i>0 ; i--){ //������ǥ�� ��ĭ�� �ű� 
        x[i] = x[i-1];
        y[i] = y[i-1];
    }    
    gotoxy(MAP_X+x[0], MAP_Y+y[0], "��"); //�Ӹ��� �ִ����� �������� ��ħ 
    if(dir==LEFT) --x[0]; //���⿡ ���� ���ο� �Ӹ���ǥ(x[0],y[0])���� ���� 
    if(dir==RIGHT) ++x[0];
    if(dir==UP) --y[0]; 
    if(dir==DOWN) ++y[0];     
    gotoxy(MAP_X+x[i],MAP_Y+y[i],"��"); //���ο� �Ӹ���ǥ���� �Ӹ��� �׸� 
}

//=== ���� �Ͻ� ����  : pŰ�� ������ ��� 
void pause(void){ 	
    while(1){
        if(key==PAUSE){	//ȭ�� �Ͻ� ���� 
            gotoxy(MAP_X+(MAP_WIDTH/2)-8,MAP_Y,"< PAUSE : PRESS ANY KEY TO RESUME > ");
            Sleep(400);
            gotoxy(MAP_X+(MAP_WIDTH/2)-8,MAP_Y,"                                    ");
            Sleep(400);
        }else{
            draw_map();		//�� �׵θ� �׸��� 
            return;
        }
		 
        if(kbhit()){	//Ű�� ������ ��� 
            do{
                key=getch();
            }while(key==224);
        }            
    }
}

//=== �ְ����� ���Ͽ� �����ϱ� �Ǵ� �о���� 
void readwrite_record(char mode){
	FILE *fp = NULL;
	switch(mode){
		case 'r':
			fp = fopen("record.bin","rb");		//notepad++���� bin���� �б� 
			if(fp == NULL){						//���� ���� ����(������ ���� ���) 
				fclose(fp);						//������ ���Ͻ�Ʈ�� ���� ��, 
			
				fp = fopen("record.bin","wb");	//���� ���Ͻ�Ʈ���� ���� ����(���� �ڵ� ����) 
				fprintf(fp,"%s %d","�ӱ�����", best_score);	//Writes the C string pointed by format to the stream
				fclose(fp);
				//exit(0);	//Exit Success, exit(1);-Exit Failure
				break;
			}
			fscanf(fp,"%s %d", best_user, &best_score);	//���Ͽ��� �о ������ ���� 
			
			fclose(fp);
			break;
		case 'w':
			fp = fopen("record.bin","wb");
			if(fp == NULL){
				printf("���� ���� ����");
				exit(0);	//Exit Success, exit(1);-Exit Failure
			}
			fprintf(fp,"%s %d", best_user, best_score);	//���Ͽ� ���� 
			
			fclose(fp);
			break;
	}	
}

void game_effect(int mode){
	switch(mode){
		case 1:
			Beep(_C4, 200);	//��, Beep(������, �����ӽð�(ms)) 
			Beep(_D4, 200);	//�� 
			Beep(_E4, 200);	//�� 
			Beep(_F4, 200);	//�� 
			Beep(_G4, 200);	//�� 
			Beep(_A4, 200);	//�� 
			Beep(_B4, 200);	//�� 
			Beep(_C5, 200);	//��	 
			break;
		case 2:
			Beep(_C5, 200);	//��	 
			Beep(_B4, 200);	//�� 
			Beep(_A4, 200);	//�� 
			Beep(_G4, 200);	//��
			Beep(_F4, 200);	//��
			Beep(_E4, 200);	//�� 
			Beep(_D4, 200);	//��
			Beep(_C4, 200);	//��
	}
} 
//########################################################
//##############  [�Լ�] ���� ���� ####################### 
//########################################################

void game_over(void){ 
	game_effect(2);
    gotoxy(MAP_X+(MAP_WIDTH/2)-6,MAP_Y+5,"+----------------------+");
    gotoxy(MAP_X+(MAP_WIDTH/2)-6,MAP_Y+6,"|      GAME OVER..     |");
    gotoxy(MAP_X+(MAP_WIDTH/2)-6,MAP_Y+7,"+----------------------+");
    gotoxy(MAP_X+(MAP_WIDTH/2)-6,MAP_Y+8," YOUR SCORE : ");
    printf("%d", last_score=score);
    
    gotoxy(MAP_X+(MAP_WIDTH/2)-7,MAP_Y+12," Press any keys to restart.. ");
    
    if(score > best_score) {	//�� ������ �ְ��������� ������ 
        best_score = score;		//�� ������ �ְ������� ���� 
        strcpy(best_user, user_name);		//user_name�� best_user�� ���� 
    	gotoxy(MAP_X+(MAP_WIDTH/2)-4,MAP_Y+10,"�� BEST SCORE ��");
    }
    readwrite_record('w');			//���Ͽ� �����ϱ� 
    Sleep(500);
    
	while (kbhit()) getch();		//Ű�� �������� 
    key=getch();
    title();						//���� ���� 
}
 
void food(void){
    int i;
    
    int food_crush_on=0;	//food�� �� ������ǥ�� ���� ��� on(1) 
    int r=0; 				//���� ������ �絿�Ǵ� ���� 
    gotoxy(MAP_X,MAP_Y+MAP_HEIGHT," YOUR SCORE: "); 	//����ǥ�� 
    printf("%3d, LAST SCORE: %3d, BEST SCORE: %s�� %3d", score, last_score, best_user, best_score);
    
    while(1){            
        food_crush_on=0;    

        srand((unsigned)time(NULL)+r); 		//����ǥ���� 
        
        food_x=(rand()%(MAP_WIDTH-2))+1;    //������ ��ǥ���� ���� 
        food_y=(rand()%(MAP_HEIGHT-2))+1;
		        
        for(i=0;i<length;i++){ 				//food�� �� ����� ��ġ���� Ȯ��  
            if(food_x==x[i]&&food_y==y[i]){
                food_crush_on=1; 			//��ġ�� food_crush_on �� on 
                r++;
				break;
            }
        }
        
        if(food_crush_on==1) continue; 			//������ ��� while���� �ٽ� ���� 
            
        gotoxy(MAP_X+food_x,MAP_Y+food_y,"��"); //�Ȱ����� ��� ��ǥ���� food�� ��� 
        speed-=3; 								//�ӵ� ���� 
        break;        
    }
}
 
void status(void){ 		//���� ������ ���� �ִ� �Լ� 
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
