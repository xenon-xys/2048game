#include <iostream>
#include <stdio.h>
#include <graphics.h>
#include <easyx.h>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <conio.h>
#pragma comment(lib,"winmm.lib")

#define BOARD 100
#define FRAME 15
#define BORDER 50
#define UP_BORDER 200
#define BOARD_NUM 4
#define FRAME_NUM 5
#define LR_BORDER_NUM 2

#define WIN_WIDTH (FRAME * FRAME_NUM + BOARD * BOARD_NUM + BORDER * LR_BORDER_NUM)
#define WIN_HEIGHT (FRAME * FRAME_NUM + BOARD * BOARD_NUM + BORDER + UP_BORDER)

#define INIT_X 50
#define INIT_Y 200

int map[BOARD_NUM][BOARD_NUM];
int score = 0;
int max_score = 0;

int x_l_esc = WIN_WIDTH * 3 / 4;
int y_t = UP_BORDER * 7 / 16;
int x_r_esc = x_l_esc + 70;
int y_d = UP_BORDER * 9 / 16;
int x_l_restart = WIN_WIDTH / 4;
int x_r_restart = x_l_restart + 70;

enum Color {
	z = RGB(194, 166, 101),
	tt1 = RGB(238, 228, 218),
	tt2 = RGB(237, 224, 200),
	tt3 = RGB(242, 177, 121),
	tt4 = RGB(245, 149, 99),
	tt5 = RGB(246, 124, 95),
	tt6 = RGB(246, 94, 59),
	tt7 = RGB(242, 177, 121),
	tt8 = RGB(237, 204, 94),
	tt9 = RGB(255, 0, 128),
	tt10 = RGB(145, 0, 72),
	tt11 = RGB(242, 17, 158),
	b = RGB(153, 130, 76),
};
Color arr[13] = { z,tt1,tt2,tt3,tt4,tt5,tt6,tt7,tt8,tt9,tt10,tt11,b };

int randnum() {
	
	if (rand() % 10 == 0) { //刷新4的概率是1/10
		std::cout << 4 << std::endl;
		return 4;
	}
	else {
		std::cout << 2 << std::endl;
		return 2;
	}
}

//边框15px * 5 方格100px * 4 左右边框50px 下边框50px 上边框200px（标题、分数）
void drawback() {
	setbkcolor(RGB(209, 178, 104));
	cleardevice();

	setfillcolor(RGB(153,130,76));
	solidroundrect(INIT_X, INIT_Y, INIT_X + BOARD*BOARD_NUM+FRAME*FRAME_NUM,INIT_Y+ BOARD * BOARD_NUM + FRAME * FRAME_NUM,20,20);

	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "黑体");
	settextcolor(BLACK);
	char title[20] = "";
	sprintf_s(title, "2048 feat by xenon");
	int titlewidth = textwidth(title);
	int titleheight = textheight(title);
	outtextxy(0 + (WIN_WIDTH / 2 - titlewidth / 2), 0 + (UP_BORDER / 4 - titleheight / 2), title);

	setbkmode(TRANSPARENT);
	settextstyle(25, 0, "黑体");
	settextcolor(RGB(89, 76, 45));
	char strtemp_s[20] = "";
	sprintf_s(strtemp_s, "当前分数：%d", score);
	int tempwidth_s = textwidth(strtemp_s);
	int tempheight_s = textheight(strtemp_s);
	outtextxy(0+(WIN_WIDTH/4-tempwidth_s/2), UP_BORDER/2+(UP_BORDER/4-tempheight_s/2), strtemp_s);

	setbkmode(TRANSPARENT);
	settextstyle(25, 0, "黑体");
	settextcolor(RGB(89, 76, 45));
	char strtemp_m[20] = "";
	sprintf_s(strtemp_m, "最大分数：%d", max_score);
	int tempwidth_m = textwidth(strtemp_m);
	int tempheight_m = textheight(strtemp_m);
	outtextxy(WIN_WIDTH/2 + (WIN_WIDTH / 4 - tempwidth_m / 2), UP_BORDER / 2 + (UP_BORDER / 4 - tempheight_m / 2), strtemp_m);

	setlinecolor(BLACK);
	setlinestyle(PS_SOLID,3);
	setfillcolor(BROWN);
	fillrectangle(x_l_esc,y_t,x_r_esc,y_d);
	settextstyle(20, 0, "黑体");
	settextcolor(WHITE);
	char text_esc[20] = "";
	sprintf_s(text_esc, "退出");
	int text_esc_w = textwidth(text_esc);
	int text_esc_h = textheight(text_esc);
	outtextxy(x_l_esc + (x_r_esc - x_l_esc) / 2 - text_esc_w / 2, y_t + (y_d - y_t) / 2 - text_esc_h / 2, text_esc);

	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	setfillcolor(BROWN);
	fillrectangle(x_l_restart, y_t, x_r_restart, y_d);
	settextstyle(20, 0, "黑体");
	settextcolor(WHITE);
	char text_restart[20] = "";
	sprintf_s(text_restart, "重开");
	int text_restart_w = textwidth(text_restart);
	int text_restart_h = textheight(text_restart);
	outtextxy(x_l_restart + (x_r_restart - x_l_restart) / 2 - text_restart_w / 2, y_t + (y_d - y_t) / 2 - text_restart_h / 2, text_restart);
	
	for (int i = 0;i < BOARD_NUM;i++) {
		for (int j = 0;j < BOARD_NUM;j++) {
			
			int index = (int)log2(map[i][j] == 0 ? 1 : map[i][j]);
			COLORREF tcolor = arr[index];
			setfillcolor(tcolor);
			int x1 = INIT_X + BOARD * j + FRAME * (j + 1);
			int y1 = INIT_Y + BOARD * i + FRAME * (i + 1);
			int x2 = x1 + BOARD;
			int y2 = y1 + BOARD;
			solidroundrect(x1, y1, x2, y2,10,10);

			if (map[i][j] != 0) {
				setbkmode(TRANSPARENT);
				if (map[i][j] > 1000) {
					settextstyle(35, 0, "黑体");
				}
				else if(map[i][j]>100){
					settextstyle(42, 0, "黑体");
				}
				else {
					settextstyle(50, 0, "黑体");
				}
				settextcolor(RGB(89, 76, 45));

				//std::stringstream str;
				//str << map[i][j];
				//std::string s = str.str();
				//outtextxy(x1, y1, s.c_str());

				char str[10] = "";
				sprintf_s(str, "%d", map[i][j]);
				int text_width = textwidth(str);
				int text_height = textheight(str);
				int delta_x = BOARD / 2 - text_width / 2;
				int delta_y = BOARD / 2 - text_height / 2;
				outtextxy(x1 + delta_x, y1 + delta_y, str);
			}
		}
	}
}

void generaterand() {
	rerand:
	int i = rand() % BOARD_NUM;
	int j = rand() % BOARD_NUM;
	if (map[i][j] != 0) {
		goto rerand;
	}
	map[i][j] = randnum();
}

void initmap() {
	for (int i = 0;i < BOARD_NUM;i++) {
		for (int j = 0;j < BOARD_NUM;j++) {
			map[i][j] = 0;
		}
	}
	generaterand();
	generaterand();
}

bool moveup() {
	bool flag = false;
	for (int col = 0;col < BOARD_NUM;col++) {
		int temp = 0;
		for (int i = 1;i < BOARD_NUM;i++) {
			if (map[i][col] != 0) {
				if (map[temp][col] == 0) { //上方格子为空，则上移
					map[temp][col] = map[i][col];
					map[i][col] = 0;
					flag = true;
				}
				else if (map[temp][col]==map[i][col]) { //上方存在相同格子，相加并temp下一格
					map[temp][col] *= 2;
					score += map[temp][col];
					map[i][col] = 0;
					temp++;
					flag = true;
				}
				else { //上方存在不同格子
					map[temp + 1][col] = map[i][col]; //先保证temp下面一格一定会有数字
					if (temp + 1 != i) //如果此时遍历的i不是temp正下方那格，会产生移动
					{
						map[i][col] = 0;
						flag = true;
					}
					temp++;
				}
			}
		}
	}
	return flag;
}
bool movedown() {
	bool flag = false;
	for (int col = 0;col < BOARD_NUM;col++) {
		int temp = BOARD_NUM-1;
		for (int i = BOARD_NUM-2;i >= 0;i--) {
			if (map[i][col] != 0) {
				if (map[temp][col] == 0) { 
					map[temp][col] = map[i][col];
					map[i][col] = 0;
					flag = true;
				}
				else if (map[temp][col] == map[i][col]) { 
					map[temp][col] *= 2;
					score += map[temp][col];
					map[i][col] = 0;
					temp--;
					flag = true;
				}
				else { 
					map[temp - 1][col] = map[i][col]; 
					if (temp - 1 != i) 
					{
						map[i][col] = 0;
						flag = true;
					}
					temp--;
				}
			}
		}
	}
	return flag;
}
bool moveleft() {
	bool flag = false;
	for (int row = 0;row < BOARD_NUM;row++) {
		int temp = 0;
		for (int i = 1;i < BOARD_NUM;i++) {
			if (map[row][i] != 0) {
				if (map[row][temp] == 0) {
					map[row][temp] = map[row][i];
					map[row][i] = 0;
					flag = true;
				}
				else if (map[row][temp] == map[row][i]) {
					map[row][temp] *= 2;
					score += map[row][temp];
					map[row][i] = 0;
					temp++;
					flag = true;
				}
				else {
					map[row][temp+1] = map[row][i];
					if (temp + 1 != i)
					{
						map[row][i] = 0;
						flag = true;
					}
					temp++;
				}
			}
		}
	}
	return flag;
}
bool moveright() {
	bool flag = false;
	for (int row = 0;row < BOARD_NUM;row++) {
		int temp = BOARD_NUM-1;
		for (int i = BOARD_NUM-2;i >=0;i--) {
			if (map[row][i] != 0) {
				if (map[row][temp] == 0) {
					map[row][temp] = map[row][i];
					map[row][i] = 0;
					flag = true;
				}
				else if (map[row][temp] == map[row][i]) {
					map[row][temp] *= 2;
					score += map[row][temp];
					map[row][i] = 0;
					temp--;
					flag = true;
				}
				else {
					map[row][temp - 1] = map[row][i];
					if (temp - 1 != i)
					{
						map[row][i] = 0;
						flag = true;
					}
					temp--;
				}
			}
		}
	}
	return flag;
}

void keydeal(bool &is_changed) {
	char key = _getch();
	switch (key) {
	case 72:
		is_changed=moveup();
		break;
	case 80:
		//movedown();
		break;
	case 75:
		//moveleft();
		break;
	case 77:
		//moveright();
		break;
	}
}

bool check_2048() {
	for (int i = 0;i < BOARD_NUM;i++) {
		for (int j = 0;j < BOARD_NUM;j++) {
			if (map[i][j] == 2048) {
				return true;
			}
		}
	}
	return false;
}
bool check_full() {
	for (int i = 0;i < BOARD_NUM;i++) {
		for (int j = 0;j < BOARD_NUM;j++) {
			if (map[i][j] == 0) {
				return false;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) { 
			if (map[i][j] == map[i][j + 1]) {
				return false; 
			}
		}
	}
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 3; i++) {
			if (map[i][j] == map[i + 1][j]) {
				return false;
			}
		}
	}
	return true;
}

const int Interval = 200; //按键间隔200ms以上
DWORD last_keytime = 0;

int main() {

	srand((unsigned int)time(NULL) + clock());

	initgraph(WIN_WIDTH, WIN_HEIGHT/*,SHOWCONSOLE*/);
	BeginBatchDraw();

	initmap();

	HWND hnd = GetHWnd();

	mciSendString("open ./res/那个夏天.mp3 alias bgm", 0, 0, 0);
	mciSendString("play bgm", 0, 0, 0);

	while (1) {
		drawback();
		FlushBatchDraw();
		/*if (_kbhit()) {
			bool is_changed = false;
			keydeal(is_changed);
			if (is_changed) {
				generaterand();
			}
		}*/

		
		ExMessage msg;
		if (peekmessage(&msg, EM_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if (msg.x >= x_l_esc && msg.x <= x_r_esc && msg.y >= y_t && msg.y <= y_d) { //退出程序
					closegraph();
					mciSendString("close bgm", 0, 0, 0);
					exit(0);
				}
				else if (msg.x >= x_l_restart && msg.x <= x_r_restart && msg.y >= y_t && msg.y <= y_d) { //重开一局
					score = 0;
					initmap();
					drawback();
					FlushBatchDraw();
				}
			}
		}

		bool is_changed = false;
		DWORD now_keytime = GetTickCount();
		if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000)) {
			if (now_keytime - last_keytime >= Interval) {
				is_changed = moveup();
				last_keytime = now_keytime;
			}
		}
		else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000)) {
			if (now_keytime - last_keytime >= Interval) {
				is_changed = movedown();
				last_keytime = now_keytime;
			}
		}
		else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000)) {
			if (now_keytime - last_keytime >= Interval) {
				is_changed = moveleft();
				last_keytime = now_keytime;
			}
		}
		else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000)) {
			if (now_keytime - last_keytime >= Interval) {
				is_changed = moveright();
				last_keytime = now_keytime;
			}
		}

		max_score = (max_score > score) ? max_score : score;
		
		if (is_changed) {
			generaterand();
			if (check_2048()) {
				drawback();
				FlushBatchDraw();
				char text[50] = "";
				sprintf_s(text, "恭喜你成功合成了2048，本次分数为%d，按OK再来一次吧", score);
				MessageBox(hnd, text, "结束", MB_OK);
				score = 0;
				initmap();
			}
			else if (check_full()) {
				drawback();
				FlushBatchDraw();
				char text[50] = "";
				sprintf_s(text, "格子被填满了，本次分数为%d，按OK再来一次吧", score);
				MessageBox(hnd, text, "结束", MB_OK);
				score = 0;
				initmap();
			}
		}

		Sleep(16);
	}


	//while (1) {
	//	std::cout << randnum() << std::endl;
	//	Sleep(200);
	//}
	
	//getchar();
	EndBatchDraw();
	closegraph();
}