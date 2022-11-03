// 2071064, ����
// 2�ο� snake ���α׷�
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// ���� ����
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define ESC 0x1b //  ESC ������ ����

#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.

#define UP  0x48 // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define UP2		'w'//player2 �� AWSD �� ����Ű ���
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'

#define SHOOT1 '/'
#define SHOOT2 'r'

#define WIDTH 90
#define HEIGHT 25

#define SNAKE_LENGTH 6
#define SNAKE_LENGTH_MAX 30
COORD snake[SNAKE_LENGTH_MAX]; // snake �� ��ǥ�� gotoxy2()���� ����ϱ� ����.
COORD snake2[SNAKE_LENGTH_MAX]; // snake �� ��ǥ�� gotoxy2()���� ����ϱ� ����.

clock_t gold_start_time1, gold_start_time2;
int gold_runtime1[2] = { 0 }, gold_runtime2[2] = { 0 };
char body_ch1, body_ch2;
unsigned char shoot_ch1[5] = { 0 }, shoot_ch2[5] = { 0 };
int shooting1[5] = { 0 }, shooting2[5] = { 0 };

int snake_length = SNAKE_LENGTH;
int keep_moving = 0; // 1:����̵�, 0:��ĭ���̵�
int p1_frame_sync = 2; // ó�� ������ 2 frame ���� �̵�
int p1_frame_sync_count = 0;
int oldx, oldy, newx, newy;

int snake_length2 = SNAKE_LENGTH;
int keep_moving2 = 0; // 1:����̵�, 0:��ĭ���̵�.
int p2_frame_sync = 2; 
int p2_frame_sync_count = 0;
int oldx2, oldy2, newx2, newy2;

int golds[WIDTH][HEIGHT] = { 0 }; // 1�̸� Gold �ִٴ� ��
int golds_count = 0;
int gold_time = 0;
int goldinterval = 5; // GOLD ǥ�� ����
int gold_frame_sync = 20; // 20 frame ���� �ѹ��� gold �� �����δ�.

COORD gold2[3];
int golds_count2 = 0;
int gold_time2 = 0;
int goldinterval2 = 3; // GOLD ǥ�� ����
int goldDelete2 = 7;// GOLD ���� ����
int gold_frame_sync2 = 40; // 40 frame ���� �ѹ��� gold �� �����δ�.

COORD gold3[3];
int golds_count3 = 0;
int gold_time3 = 0;
int goldinterval3 = 5; // GOLD ǥ�� ����
int goldDelete3 = 10;// GOLD ���� ����
int gold_frame_sync3 = 40; // 40 frame ���� �ѹ��� gold �� �����δ�.

int Shootings[WIDTH][HEIGHT] = { 0 };
int Shoot1[5][2] = { 0 };
int Shoot2[5][2] = { 0 };
int shooting_count = 0;
int Shootings_time = 0;
int Shootings_interval = 10; // GOLD ǥ�� ����
int shooting_score[2] = { 3,3 };
int gold_frame_sync4 = 10; // 40 frame ���� �ѹ��� gold �� �����δ�.

int frame_count = 0; // game ���� frame count �� �ӵ� ���������� ���ȴ�.
int Delay = 100; // 100 msec delay, �� ���� ���̸� �ӵ��� ��������.
int game_over = 0;
int die_decision = -1;
int called[2];

void removeCursor(void) { // Ŀ���� �Ⱥ��̰� �Ѵ�

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void gotoxy(int x, int y) //���� ���ϴ� ��ġ�� Ŀ�� �̵�
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API �Լ��Դϴ�. �̰� ���ʿ� �����
}
//���� ���ϴ� ��ġ�� Ŀ�� �̵�
// Snake���� ��ǥ�� COORD �迭�� �̿��ϱ� ����.
void gotoxy2(COORD pos)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API �Լ��Դϴ�. �̰� ���ʿ� �����
}
void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}
void draw_hline(int y, int x1, int x2, char ch)
{
	gotoxy(x1, y);
	for (; x1 <= x2; x1++)
		putchar(ch);
}

#define SHOOTING_SHOOT	"��"
#define SHOOTING_SHOOT2	"��"
void Shoot(int num, char way, int k) {
	int x, y;
	//num�� ������ �߻��� player
	if (num == 1) {
		x = snake[0].X; y = snake[0].Y;
		switch (way) {
		case UP:
			y = y - 2; break;
		case DOWN:
			y = y + 2; break;
		case LEFT:
			x = x - 2; break;
		case RIGHT:
			x = x + 2; break;
		}
		Shoot1[k][0] = x; Shoot1[k][1] = y;
	}
	else {
		x = snake2[0].X; y = snake2[0].Y;
		switch (way) {
		case UP2:
			y = y - 2; break;
		case DOWN2:
			y = y + 2; break;
		case LEFT2:
			x = x - 2; break;
		case RIGHT2:
			x = x + 2; break;
		}
		Shoot2[k][0] = x; Shoot2[k][1] = y;
	}
	switch (way) {
	case UP: case UP2: case DOWN: case DOWN2:
		gotoxy(x, y);
		printf(SHOOTING_SHOOT2);
		break;
	default:
		gotoxy(x, y);
		printf(SHOOTING_SHOOT);
	}
}
void move_Shoot(int num, char way, int k) {
	int x, y;
	//num�� ������ �߻��� player
	if (num == 1) {
		x = Shoot1[k][0]; y = Shoot1[k][1];
		gotoxy(x, y);
		printf(" ");
		switch (way) {
		case UP:
			if (y <= 4) {
				shooting1[k] = 0;
				return;
			}
			y = y - 2; break;
		case DOWN:
			if (y >= HEIGHT - 2) {
				shooting1[k] = 0;
				return;
			}
			y = y + 2; break;
		case LEFT:
			if (x <= 2) {
				shooting1[k] = 0;
				return;
			}
			x = x - 2; break;
		case RIGHT:
			if (x >= WIDTH - 2) {
				shooting1[k] = 0;
				return;
			}
			x = x + 2; break;
		}
		Shoot1[k][0] = x; Shoot1[k][1] = y;
		if (snake2[0].X == x && snake2[0].Y == y) {
			die_decision = 2;
			return;
		}
		for (int i = 1; i <= snake_length2; i++) {
			if (snake2[i].X == x && snake2[i].Y == y) {
				snake_length2--;
				gotoxy2(snake[snake_length]);//������ǥ
				printf("  "); // ������ �����
				return;
			}
		}
	}
	else {
		x = Shoot2[k][0]; y = Shoot2[k][1];
		gotoxy(x, y);
		printf(" ");
		switch (way) {
		case UP2:
			if (y <= 4) {
				shooting2[k] = 0;
				return;
			}
			y = y - 2; break;
		case DOWN2:
			if (y >= HEIGHT - 2) {
				shooting2[k] = 0;
				return;
			}
			y = y + 2; break;
		case LEFT2:
			if (x <= 2) {
				shooting2[k] = 0;
				return;
			}
			x = x - 2; break;
		case RIGHT2:
			if (x >= WIDTH - 2) {
				shooting2[k] = 0;
				return;
			}
			x = x + 2; break;
		}
		Shoot2[k][0] = x; Shoot2[k][1] = y;
		if (snake[0].X == x && snake[0].Y == y) {
			die_decision = 1;
			return;
		}
		for (int i = 1; i <= snake_length; i++) {
			if (snake[i].X == x && snake[i].Y == y) {
				snake_length--;
				gotoxy2(snake[snake_length]);//������ǥ
				printf("  "); // ������ �����
				return;
			}
		}
	}
	switch (way) {
	case UP: case UP2: case DOWN: case DOWN2:
		gotoxy(x, y);
		printf(SHOOTING_SHOOT2);
		break;
	default:
		gotoxy(x, y);
		printf(SHOOTING_SHOOT);
	}
}
void show_shoot_score(int player)
{
	textcolor(BLACK, WHITE);
	if (shooting_score[player] > 8)
		return;
	switch (player) {
	case 0: // player 1
		gotoxy(WIDTH - 25, 2);
		printf("           ");
		gotoxy(WIDTH - 25, 2);
		break;
	case 1: // player 2
		gotoxy(13, 2);
		printf("           ");
		gotoxy(13, 2);
		break;
	}
	int count = shooting_score[player];
	while (count--)
		printf("* ");
	textcolor(WHITE, BLACK);
}

void showsnake(int p) {
	int i;
	int x, y;
	if (p == 0)
	{
		x = snake[0].X; y = snake[0].Y;
		textcolor(YELLOW2, BLACK);
		if (keep_moving == 0) {
			gotoxy2(snake[0]); printf("��");
		}
		else {
			gotoxy2(snake[0]); printf("��"); // �Ӹ��� ���� �׸���
			for (i = 1; i < snake_length; i++) {
				gotoxy2(snake[i]);
				printf("��"); // ������ �׸���.
			}
		}
		textcolor(WHITE, BLACK);
	}
	else
	{
		x = snake2[0].X; y = snake2[0].Y;
		textcolor(RED2, BLACK);
		if (keep_moving2 == 0) {
			gotoxy2(snake2[0]); printf("��");
		}
		else {
			gotoxy2(snake2[0]); printf("��");
			for (i = 1; i < snake_length2; i++) {
				gotoxy2(snake2[i]);
				printf("��"); // ������ �׸���.
			}
		}
		textcolor(WHITE, BLACK);
	}
	if (golds[x][y] || golds[x + 1][y]) {
		if (p == 0)
			snake_length++;
		else
			snake_length2++;
		golds[x][y] = 0; golds[x + 1][y] = 0;
		golds_count--;
	}
	for (int j = 0; j < 3; j++ && x != 0 && y != 0) {
		if (gold2[j].X == x && gold2[j].Y == y) {
			gold2[j].X = 0; gold2[j].Y = 0;
			golds_count2--;
			if (p == 0) {
				p1_frame_sync = 1;
				if (gold_runtime1[0] == 0)
					gold_start_time1 = clock();
				if (gold_runtime1[1] != 0)
					gold_runtime1[1] = 0;
				gold_runtime1[0] += 3;
				gotoxy(WIDTH - 3, 4);
				printf("����");
			}
			else {
				p2_frame_sync = 1;
				if (gold_runtime2[0] == 0)
					gold_start_time2 = clock();
				if (gold_runtime2[1] != 0)
					gold_runtime2[1] = 0;
				gold_runtime2[0] += 3;
				gotoxy(0, 4);
				printf("����");
			}
		}
	}
	for (int j = 0; j < 3; j++ && x != 0 && y != 0) {
		if (gold3[j].X == x && gold3[j].Y == y) {
			gold3[j].X = 0; gold3[j].Y = 0;
			golds_count3--;
			if (p == 0) {
				p1_frame_sync = 5;
				if (gold_runtime1[1] == 0)
					gold_start_time1 = clock();
				if (gold_runtime1[0] != 0)
					gold_runtime1[0] = 0;
				gold_runtime1[1] += 5;
				gotoxy(WIDTH - 3, 4);
				printf("����");
			}
			else {
				p2_frame_sync = 5;
				if (gold_runtime2[1] == 0)
					gold_start_time2 = clock();
				if (gold_runtime2[0] != 0)
					gold_runtime2[0] = 0;
				gold_runtime2[1] += 5;
				gotoxy(0, 4);
				printf("����");
			}
		}
	}
	if (Shootings[x][y] || Shootings[x + 1][y]) {
		Shootings[x][y] = 0; Shootings[x + 1][y] = 0;
		shooting_count--;
		if (shooting_score[p] <= 7) {
			shooting_score[p]++;
			show_shoot_score(p);
		}
	}
}
void movesnake(int x, int y, int no)
{
	for (int i = 0; i < 5; i++) {
		if (shooting1[i])
			move_Shoot(1, shoot_ch1[i], i);
		if (shooting2[i])
			move_Shoot(2, shoot_ch2[i], i);
	}

	int i;
	if (no == 0)
	{
		gotoxy2(snake[snake_length - 1]);//������ǥ
		printf("  "); // ������ �����
		for (i = snake_length - 1; i >= 1; i--) { // ��ü ��ǥ�� ��ĭ�� �ڷ� �̵��Ѵ�.
			snake[i] = snake[i - 1];
		}
		snake[0].X = x; snake[0].Y = y; // ���ο� �Ӹ� ��ǥ�� �߰��Ѵ�. �Ӹ��� �þ��.
		showsnake(0);
	}
	else
	{
		gotoxy2(snake2[snake_length2 - 1]);//������ǥ
		printf("  "); // ������ �����
		for (i = snake_length2 - 1; i >= 1; i--) { // ��ü ��ǥ�� ��ĭ�� �ڷ� �̵��Ѵ�.
			snake2[i] = snake2[i - 1];
		}
		snake2[0].X = x; snake2[0].Y = y; // ���ο� �Ӹ� ��ǥ�� �߰��Ѵ�. �Ӹ��� �þ��.
		showsnake(1);
	}
}

// ȭ�� ������ ���ϴ� �������� �����Ѵ�.
void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}
// box �׸��� �Լ�, ch ���ڿ��� (x1,y1) ~ (x2,y2) box�� �׸���.
void draw_box2(int x1, int y1, int x2, int y2)
{
	int x, y;
	//int len = strlen(ch);
	for (x = x1; x < x2; x += 2) { // �ѱ��� 2ĭ�� �����Ѵ�.
		gotoxy(x, y1);
		printf("%s", "��"); // �� ������ ����Ű ������ ����
		gotoxy(x, y2);
		printf("%s", "��");
	}
	for (y = y1; y < y2; y++) {
		gotoxy(x1, y);
		printf("%s", "��");
		gotoxy(x2, y);
		printf("%s", "��");
	}
	gotoxy(x1, y1); printf("��");
	gotoxy(x1, y2); printf("��");
	gotoxy(x2, y1); printf("��");
	gotoxy(x2, y2); printf("��");
}

#define GOLD	"O"
void show_gold()
{
	if (golds_count >= 2)
		return;
	int x, y;
check_xy:
	do {
		x = rand() % (WIDTH - 4) + 2;
	} while (x % 2 == 1);
	y = rand() % (HEIGHT - 6) + 3; // ���� ����� ���Ѵ�

	if (x == snake[0].X && y == snake[0].Y)
		goto check_xy;
	else if (x == snake2[0].X && y == snake2[0].Y)
		goto check_xy;

	textcolor(GRAY2, BLACK);
	gotoxy(x, y);
	printf(GOLD);

	golds[x][y] = 1;
	golds_count++;
	textcolor(WHITE, BLACK);
}
#define SPEED_GOLD	"��"
void show_speedUp_gold()
{
	if (golds_count2 >= 3)
		return;
	int x, y;
check_xy:
	x = rand() % (WIDTH - 4) + 2;
	y = rand() % (HEIGHT - 6) + 3;  // ���� ����� ���Ѵ�

	if (x == snake[0].X && y == snake[0].Y)
		goto check_xy;
	else if (x == snake2[0].X && y == snake2[0].Y)
		goto check_xy;

	textcolor(BLUE2, BLACK);
	gotoxy(x, y);
	printf(SPEED_GOLD);
	int i = 0;
	for (i = 0; i < 3; i++)
		if (gold2[i].X == 0 && gold2[i].Y == 0)
			break;
	gold2[i].X = x;
	gold2[i].Y = y;
	golds_count2++;
	textcolor(WHITE, BLACK);
}
void Delete_speedUp_gold() {
	int k = rand() % 3;
	gotoxy(gold2[k].X, gold2[k].Y);
	printf(" ");
	gold2[k].X = 0; gold2[k].Y = 0;
	golds_count2--;
}
#define SPEED_GOLD2	"��"
void show_speedDown_gold()
{
	if (golds_count3 >= 3)
		return;
	int x, y;
check_xy:
	x = rand() % (WIDTH - 4) + 2;
	y = rand() % (HEIGHT - 6) + 3;  // ���� ����� ���Ѵ�

	if (x == snake[0].X && y == snake[0].Y)
		goto check_xy;
	else if (x == snake2[0].X && y == snake2[0].Y)
		goto check_xy;

	textcolor(RED1, BLACK);
	gotoxy(x, y);
	printf(SPEED_GOLD2);
	int i = 0;
	for (i = 0; i < 3; i++)
		if (gold3[i].X == 0 && gold3[i].Y == 0)
			break;
	gold3[i].X = x;
	gold3[i].Y = y;
	golds_count3++;
	textcolor(WHITE, BLACK);
}
void Delete_speedDown_gold() {
	int k = rand() % 3;
	gotoxy(gold3[k].X, gold3[k].Y);
	printf(" ");
	gold3[k].X = 0; gold3[k].Y = 0;
	golds_count3--;
}
#define SHOOTING_GOLD	"T"
void show_shooting_gold()
{
	if (shooting_count >= 1)
		return;
	int x, y;
check_xy:

	x = rand() % (WIDTH - 4) + 2;
	y = rand() % (HEIGHT - 6) + 3; // ���� ����� ���Ѵ�

	if (x == snake[0].X && y == snake[0].Y)
		goto check_xy;
	else if (x == snake2[0].X && y == snake2[0].Y)
		goto check_xy;

	gotoxy(x, y);
	printf(SHOOTING_GOLD);
	Shootings[x][y] = 1;
	shooting_count++;
}
// gold�� �����δ�.
void move_gold2() {
	int x, y, dx, dy, newx, newy;

	// gold ���� ���� �� �ִ�.
	if (golds_count2 == 0)
		return;
	for (int i = 0; i < 3; i++) {
		if (gold2[i].X == 0 && gold2[i].Y == 0)
			break;
		else {
			x = gold2[i].X;
			y = gold2[i].Y;
		check_xy:
			do {
				dx = rand() % 4 - 2; // -2 0 2
			} while (dx % 2 == 1);
			dy = rand() % 3 - 1; // -1 0 1
			newx = x + dx;
			newy = y + dy;
			if (newx == WIDTH - 3) newx = WIDTH - 2;
			if (newx < 3) newx = 3;
			if (newy > HEIGHT - 5) newy = HEIGHT - 2;
			if (newy < 5) newy = 5;
			if (x == snake[0].X && y == snake[0].Y)
				goto check_xy;
			else if (x == snake2[0].X && y == snake2[0].Y)
				goto check_xy;

			gotoxy(x, y);
			textcolor(BLACK, BLACK);
			printf(" "); // erase gold
			textcolor(BLUE2, BLACK);
			gotoxy(newx, newy);
			printf(SPEED_GOLD);
			gold2[i].X = newx; gold2[i].Y = newy;
			textcolor(WHITE, BLACK);
		}
	}
}
void move_gold3() {
	int x, y, dx, dy, newx, newy;

	// gold ���� ���� �� �ִ�.
	if (golds_count3 == 0)
		return;
	for (int i = 0; i < 3; i++) {
		if (gold3[i].X == 0 && gold3[i].Y == 0)
			break;
		else {
			x = gold3[i].X;
			y = gold3[i].Y;
		check_xy:
			do {
				dx = rand() % 4 - 2; // -2 0 2
			} while (dx % 2 == 1);
			dy = rand() % 3 - 1; // -1 0 1
			newx = x + dx;
			newy = y + dy;
			if (newx == WIDTH - 3) newx = WIDTH - 2;
			if (newx < 3) newx = 3;
			if (newy > HEIGHT - 5) newy = HEIGHT - 2;
			if (newy < 5) newy = 5;
			if (x == snake[0].X && y == snake[0].Y)
				goto check_xy;
			else if (x == snake2[0].X && y == snake2[0].Y)
				goto check_xy;

			gotoxy(x, y);
			textcolor(BLACK, BLACK);
			printf(" "); // erase gold
			textcolor(RED1, BLACK);
			gotoxy(newx, newy);
			printf(SPEED_GOLD2);
			gold3[i].X = newx; gold3[i].Y = newy;
			textcolor(WHITE, BLACK);
		}
	}
}
void move_shooting() {
	int x, y, dx, dy, newx, newy;
	int newgolds[WIDTH][HEIGHT] = { 0 };
	static call_count3 = 0;

	// gold ���� ���� �� �ִ�.
	if (shooting_count == 0)
		return;
	for (x = 2; x < WIDTH - 3; x++) {
		for (y = 5; y < HEIGHT - 5; y++) {
			if (Shootings[x][y]) {
				check_xy:
				do {
					dx = rand() % 4 - 2; // -2 0 2
				} while (dx % 2 == 1);
				dy = rand() % 3 - 1; // -1 0 1
				newx = x + dx;
				newy = y + dy;
				if (newx == WIDTH - 3) newx = WIDTH - 2;
				if (newx < 3) newx = 3;
				if (newy > HEIGHT - 5) newy = HEIGHT - 1;
				if (newy < 5) newy = 5;
				if (x == snake[0].X && y == snake[0].Y)
					goto check_xy;
				else if (x == snake2[0].X && y == snake2[0].Y)
					goto check_xy;

				gotoxy(x, y);
				textcolor(WHITE, BLACK);
				printf(" "); // erase gold
				gotoxy(newx, newy);
				printf(SHOOTING_GOLD);
				newgolds[newx][newy] = 1; // �̵��� golds�� ��ǥ
			}
		}
	}
	memcpy(Shootings, newgolds, sizeof(newgolds)); // �ѹ��� gold ��ġ�� �����Ѵ�.
}
int slap_die() {
	int i = 0;

	if (snake2[0].X == snake[0].X && snake2[0].Y == snake[0].Y && snake->X != 0 && snake->Y != 0)
		return 0;
	//�� snake�� �Ӹ��� �ھ��� �� // ���º�

	for (i = 1; i < snake_length2; i++) {
		if (snake[0].X == snake2[i].X && snake[0].Y == snake2[i].Y)
			return 1;
		//snake1�� snake2 ���� �Ӹ��� �ھ��� �� // player 1 ��
	}

	for (i = 1; i < snake_length; i++) {
		if (snake2[0].X == snake[i].X && snake2[0].Y == snake[i].Y)
			return 2;
		//snake2�� snake1 ���� �Ӹ��� �ھ��� �� // player 2 ��
	}
	return die_decision;
}
char init_game()
{

	char ch = 0;

	cls(BLACK, WHITE);
	removeCursor();

	draw_box2(0, 0, WIDTH + 1, HEIGHT);

	gotoxy(18, 4);
	printf("player2 : ");
	textcolor(RED2, BLACK);
	printf("��");

	gotoxy(WIDTH - 28, 4);
	textcolor(YELLOW1, BLACK);
	printf("��");
	textcolor(WHITE, BLACK);
	printf(" : player1");
	gotoxy(22, 20);
	printf("HOW TO PLAY (H)");
	gotoxy(WIDTH - 30, 20);
	printf("PLAY (P)");

	keep_moving = 0;
	keep_moving2 = 0;

	while (1) {
		int c1;
		c1 = rand() % 16;
		textcolor(c1, BLACK);
		gotoxy(22, 10); printf("�����  �����  �����  ��  ��� �����\n");
		gotoxy(22, 11);	printf("��        ��    ��        ��  ��  ��   ��    ��\n");
		gotoxy(22, 12);	printf("�����  ��    ��  �����  ���     �����\n");
		gotoxy(22, 13);	printf("      ��  ��    ��  ��    ��  ��  ��   ��\n");
		gotoxy(22, 14);	printf("�����  ��    ��  �����  ��  ��� �����\n");

		Sleep(300);
		if (kbhit()) {
			ch = getch();
			textcolor(BLACK, WHITE);
			if (ch == 'p' || ch == 'h')
				break;
		}
	}
	return ch;
}
char how_play_game() {
	char ch;
	int page = 1;

PAGE:
	cls(BLACK, WHITE);
	removeCursor();
	draw_box2(0, 0, WIDTH, 24);
	textcolor(WHITE, WHITE);
	draw_hline(1, 2, WIDTH, ' ');
	draw_hline(2, 2, WIDTH, ' ');

	gotoxy(13, 1);
	textcolor(RED2, WHITE);
	printf("player2 : ��");
	gotoxy(WIDTH - 25, 1);
	textcolor(YELLOW1, WHITE);
	printf("�� : player1");

	if (page % 2 == 1) {
		gotoxy(15, 5);
		textcolor(RED2, BLACK);
		printf("����ࢾ");
		gotoxy(13, 7);
		printf("�� player 2 ");
		textcolor(YELLOW2, BLACK);
		gotoxy(WIDTH - 26, 5);
		printf("�ڡ����");
		gotoxy(WIDTH - 27, 7);
		printf("�� player 1 ");
		textcolor(BLACK, WHITE);
		gotoxy(WIDTH / 2 - 3, 6);
		printf(" PLAYER ");
		gotoxy(WIDTH / 2 - 4, 9);
		printf(" �� �� Ű ");
		gotoxy(WIDTH / 2 - 6, 12);
		printf(" - �� �� �� - ");

		textcolor(WHITE, BLACK);
		gotoxy(18, 9);
		printf("W");
		gotoxy(15, 10);
		printf("A  S  D :");
		gotoxy(WIDTH - 20, 9);
		printf("��");
		gotoxy(WIDTH - 25, 10);
		printf(": �� �� ��");

		gotoxy(WIDTH / 2 - 14, 14);
		printf("���� ���� �Ӹ��� ������ ��");
		gotoxy(WIDTH / 2 - 23, 15);
		printf("��, ������ �� ���� �Ӹ��� �굵�� �����ؾ� ��");
		gotoxy(WIDTH / 2 - 14, 16);
		printf("�Ӹ��� �Ӹ��� ����� �� ���º�");
		gotoxy(WIDTH / 2 - 21, 18);
		printf("���� �Ӹ��� ������ �ڽ��� snake���� ����");
		gotoxy(WIDTH / 2 - 28, 19);
		printf("snake���̰� ��� ���ҵǾ� �Ӹ��� ������ �� ���� ������ �й�");
		gotoxy(WIDTH / 2 - 15, 21);
		printf("�ڽ��� �� �������� �̵� �Ұ���");

		gotoxy(WIDTH - 10, HEIGHT - 4);
		printf("1/2");
		gotoxy(WIDTH - 10, HEIGHT - 3);
		printf("PLAY (P)");
	}
	if (page % 2 == 0) {

		textcolor(BLACK, WHITE);
		gotoxy(WIDTH - 25, 2);
		printf("* * �� shooting iteam");

		gotoxy(WIDTH / 2 - 5, 4);
		printf(" - ITEAM - ");
		gotoxy(WIDTH / 2 - 9, 10);
		printf(" - SHOOTING ITEAM - ");
		gotoxy(WIDTH / 2 - 12, 17);
		printf(" shooting iteam ��� ��� ");

		textcolor(WHITE, BLACK);
		gotoxy(WIDTH / 2 - 9, 6);
		printf("O : snake ���� ����");
		gotoxy(WIDTH / 2 - 14, 7);
		printf("�� : 3�ʰ� �Ͻ������� �ӵ� ����");
		gotoxy(WIDTH / 2 - 14, 8);
		printf("�� : 5�ʰ� �Ͻ������� �ӵ� ����");
		gotoxy(WIDTH / 2 - 12, 10);
		printf("T : shooting ������ ȹ��");
		gotoxy(WIDTH / 2 - 12, 12);
		printf("��ܿ� ������ ���� *�� ǥ��");
		gotoxy(WIDTH / 2 - 9, 13);
		printf("�ִ� �ϰ� �� ȹ�� ����");
		gotoxy(WIDTH / 2 - 14, 14);
		printf("���� snake �Ӹ��� ������ �¸�");
		gotoxy(WIDTH / 2 - 22, 15);
		printf("���� snake ���� ������ ���� �� ���� �پ��");

		gotoxy(WIDTH / 2 - 16, 19);
		printf("1. �߻��ϰ� ���� ������ �ٶ󺻴�");
		gotoxy(WIDTH / 2 - 16, 20);
		printf("2. �ڽ��� �߻��ư�� ������");

		draw_box2(WIDTH - 22, 8, WIDTH - 6, 12);
		gotoxy(WIDTH - 20, 9);
		printf("�߻��ư");
		gotoxy(WIDTH - 20, 10);
		printf("player 1 : / ");
		gotoxy(WIDTH - 20, 11);
		printf("player 2 : R ");

		gotoxy(WIDTH - 10, HEIGHT - 4);
		printf("2/2");
		gotoxy(WIDTH - 10, HEIGHT - 3);
		printf("PLAY (P)");
	}
	while (1) {
		if (kbhit()) {
			ch = getch();
			if (ch == RIGHT) {
				page++;
				goto PAGE;
			}
			if (ch == LEFT) {
				page--;
				goto PAGE;
			}
			if (ch == 'p')
				return ch;
		}
	}
}
void start_game() {
	snake_length = snake_length2 = SNAKE_LENGTH;
	called[0] = called[1] = 0;
	keep_moving = keep_moving2 = game_over = 0;
	die_decision = -1;
	gold_runtime1[0] = gold_runtime2[0] = 0;
	gold_runtime1[1] = gold_runtime2[1] = 0;
	shooting_score[0] = shooting_score[1] = 3;
	p1_frame_sync = p2_frame_sync = 2;
	snake->X = 0; snake->Y = 0;
	snake2->X = 0;	snake2->Y = 0;
	golds_count = gold_time = golds_count2 = gold_time2 = golds_count3 = gold_time3 = 0;
	Shootings_time = shooting_count = 0;
	for (int i = 0; i < 3; i++) {
		gold2[i].X = 0; gold2[i].Y = 0;
		gold3[i].X = 0; gold3[i].Y = 0;
	}
	cls(BLACK, WHITE);
	removeCursor();

	draw_box2(0, 0, WIDTH, 24);
	textcolor(WHITE, WHITE);
	draw_hline(1, 2, WIDTH, ' ');
	draw_hline(2, 2, WIDTH, ' ');

	gotoxy(13, 1);
	textcolor(RED2, WHITE);
	printf("player2 : ��");
	gotoxy(WIDTH - 25, 1);
	textcolor(YELLOW1, WHITE);
	printf("�� : player1");
	textcolor(WHITE, BLACK);
}
char end_game() {
	char ch = 0;
	int win_check = 0;
	clock_t start, duration;

	cls(BLACK, WHITE);
	removeCursor();
	//putstar(oldx,oldy,STAR);
	draw_box2(0, 0, WIDTH + 1, 24);

	textcolor(WHITE, WHITE);
	draw_hline(1, 2, WIDTH, ' ');

	gotoxy(13, 1);
	textcolor(RED2, WHITE);
	printf("player2 : ��");
	gotoxy(WIDTH - 25, 1);
	textcolor(YELLOW1, WHITE);
	printf("�� : player1");
	textcolor(WHITE, BLACK);

	switch (die_decision) {
	case 0:
		win_check = 0;
		break;
	case 1:
		textcolor(RED2, BLACK);
		start = clock();
		while ((clock() - start) / (CLOCKS_PER_SEC / 10) < 6) {
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 1) {
				gotoxy(WIDTH / 2 - 7, 7);
				printf("   ���  ���   ");
				if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 2) {
					gotoxy(WIDTH / 2 - 7, 8);
					printf(" �������� ");
					if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 3) {
						gotoxy(WIDTH / 2 - 7, 9);
						printf(" �������� ");
						if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 4) {
							gotoxy(WIDTH / 2 - 7, 10);
							printf("   ������   ");
							if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 5) {
								gotoxy(WIDTH / 2 - 7, 11);
								printf("      ���      ");
							}
						}
					}
				}
			}
		}
		win_check = 1;
		break;
	case 2:
		gotoxy(32, 17);
		textcolor(YELLOW2, BLACK);
		start = clock();
		while ((clock() - start) / (CLOCKS_PER_SEC / 10) < 6) {
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 1) {
				gotoxy(WIDTH / 2 - 6, 7);
				printf("     ��     ");
				if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 2) {
					gotoxy(WIDTH / 2 - 6, 8);
					printf("�������");
					if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 3) {
						gotoxy(WIDTH / 2 - 6, 9);
						printf("  �����  ");
						if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 4) {
							gotoxy(WIDTH / 2 - 6, 10);
							printf("�������");
							if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 5) {
								gotoxy(WIDTH / 2 - 6, 11);
								printf("     ��     ");
							}
						}
					}
				}
			}
		}
		win_check = 1;
		break;
	}
	textcolor(WHITE, BLACK);
	if (win_check == 1) {
		start = clock();
		while ((clock() - start) / (CLOCKS_PER_SEC / 10) < 7) {
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 1) {
				gotoxy(27, 13);
				printf("��          ��  �����  ������\n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 2) {
				gotoxy(27, 14);
				printf("��    ��    ��     ��     ��      ��\n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 3) {
				gotoxy(27, 15);
				printf("��  ��  ��  ��     ��     ��      ��\n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 4) {
				gotoxy(27, 16);
				printf("��  ��  ��  ��     ��     ��      ��\n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 5) {
				gotoxy(27, 17);
				printf("��  ��  ��  ��     ��     ��      ��\n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 6) {
				gotoxy(27, 18);
				printf("���      ���  �����  ��      ��\n");
			}
		}
	}
	if (win_check == 0) {
		start = clock();
		while ((clock() - start) / (CLOCKS_PER_SEC / 10) < 7) {
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 1) {
				gotoxy(22, 10);
				printf("����     �����     ����    ��          ��\n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 2) {
				gotoxy(22, 11);
				printf("��    ��   ��     ��   ��    ��   ��    ��    �� \n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 3) {
				gotoxy(22, 12);
				printf("��    ��   �����    ��    ��   ��  ��  ��  ��\n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 4) {
				gotoxy(22, 13);
				printf("��    ��   ��    ��    �����   ��  ��  ��  ��\n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 5) {
				gotoxy(22, 14);
				printf("��    ��   ��     ��   ��    ��   ��  ��  ��  ��\n");
			}
			if ((clock() - start) / (CLOCKS_PER_SEC / 10) >= 6) {
				gotoxy(22, 15);
				printf("����     ��      ��  ��    ��   ���      ���\n");
			}
		}
	}
	while (1) {
		gotoxy(32, 21);
		textcolor(WHITE, BLACK);
		printf("Hit (R) to Restart (Q) to Quit");
		Sleep(300);
		if (kbhit()) {
			ch = getch();
			if (ch == 'r' || ch == 'q')
				return ch;
		}
	}
}
void player1(unsigned char ch)
{
	int x, y;
	int move_flag = 0;
	static unsigned char last_ch = 0;

	if (called[0] == 0) { // ó�� �Ǵ� Restart
		newx = oldx = WIDTH - 20; newy = oldy = 10;
		snake[0].X = oldx; snake[0].Y = oldy;
		showsnake(0);
		snake_length = SNAKE_LENGTH;
		called[0] = 1;
		last_ch = 0;
		ch = 0;
	}

	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;
	if (snake_length >= 2)
		body_ch1 = ch;
	switch (ch) {
	case UP:
		if (oldy > 3)
			newy = oldy - 1;
		else {
			last_ch = DOWN; newy = oldy + 1; // �ݻ�Ǵ� ���
			if (snake_length == 1) {
				game_over = 1;
				die_decision = 1;
				break;
			}
			snake_length--; // ���� �ε����� ������ �پ���
			gotoxy2(snake[snake_length]);//������ǥ
			printf("  "); // ������ �����
		}
		move_flag = 1;
		keep_moving = 1;
		break;
	case DOWN:
		if (oldy < HEIGHT - 2)
			newy = oldy + 1;
		else {
			last_ch = UP; newy = oldy - 1;  // �ݻ�Ǵ� ���
			if (snake_length == 1) {
				game_over = 1;
				die_decision = 1;
				break;
			}
			snake_length--; // ���� �ε����� ������ �پ���
			gotoxy2(snake[snake_length]);//������ǥ
			printf("  "); // ������ �����
		}
		move_flag = 1;
		keep_moving = 1;
		break;
	case LEFT:
		if (oldx > 2)
			newx = oldx - 2;
		else {
			last_ch = RIGHT; newx = oldx + 2;
			ch = RIGHT;
			if (snake_length == 1) {
				game_over = 1;
				die_decision = 1;
				break;
			}
			snake_length--; // ���� �ε����� ������ �پ���
			gotoxy2(snake[snake_length]);//������ǥ
			printf("  "); // ������ �����
		}
		move_flag = 1;
		keep_moving = 1;
		break;
	case RIGHT:
		if (oldx < WIDTH - 2)
			newx = oldx + 2;
		else {
			last_ch = LEFT; newx = oldx - 2;
			ch = LEFT;
			if (snake_length == 1) {
				game_over = 1;
				die_decision = 1;
				break;
			}
			snake_length--; // ���� �ε����� ������ �پ���
			gotoxy2(snake[snake_length]);//������ǥ
			printf("  "); // ������ �����
		}
		move_flag = 1;
		keep_moving = 1;
		break;
	}
	if (move_flag) {
		showsnake(0);
		oldx = newx; // ������ ��ġ�� ����Ѵ�.
		oldy = newy;
	}
	movesnake(newx, newy, 0);
}
void player2(unsigned char ch)
{
	int x, y;
	int move_flag = 0;
	static unsigned char last_ch2 = 0;

	if (called[1] == 0) { // ó�� �Ǵ� Restart
		newx2 = oldx2 = 20; newy2 = oldy2 = 10;
		snake2[0].X = oldx2; snake2[0].Y = oldy2;
		showsnake(1);
		snake_length2 = SNAKE_LENGTH;
		called[1] = 1;
		last_ch2 = 0;
		ch = 0;
	}

	if (keep_moving2 && ch == 0)
		ch = last_ch2;
	last_ch2 = ch;
	if (snake_length2 >= 2)
		body_ch2 = ch;
	switch (ch) {
	case UP2:
		if (oldy2 > 3)
			newy2 = oldy2 - 1;
		else {
			last_ch2 = DOWN2; newy2 = oldy2 + 1; // �ݻ�Ǵ� ���
			if (snake_length2 == 1) {
				game_over = 1;
				die_decision = 2;
				break;
			}
			snake_length2--; // ���� �ε����� ������ �پ���
			gotoxy2(snake2[snake_length2]);//������ǥ
			printf("  "); // ������ �����
		}
		move_flag = 1;
		keep_moving2 = 1;
		break;
	case DOWN2:
		if (oldy2 < HEIGHT - 2)
			newy2 = oldy2 + 1;
		else {
			last_ch2 = UP2; newy2 = oldy2 - 1;  // �ݻ�Ǵ� ���
			if (snake_length2 == 1) {
				game_over = 1;
				die_decision = 2;
				break;
			}
			snake_length2--; // ���� �ε����� ������ �پ���
			gotoxy2(snake2[snake_length2]);//������ǥ
			printf("  "); // ������ �����
		}
		move_flag = 1;
		keep_moving2 = 1;
		break;
	case LEFT2:
		if (oldx2 > 2)
			newx2 = oldx2 - 2;
		else {
			last_ch2 = RIGHT2; newx2 = oldx2 + 2;
			ch = RIGHT2;
			if (snake_length2 == 1) {
				game_over = 1;
				die_decision = 2;
				break;
			}
			snake_length2--; // ���� �ε����� ������ �پ���
			gotoxy2(snake2[snake_length2]);//������ǥ
			printf("  "); // ������ �����
		}
		move_flag = 1;
		keep_moving2 = 1;
		break;
	case RIGHT2:
		if (oldx2 < WIDTH - 2)
			newx2 = oldx2 + 2;
		else {
			last_ch2 = LEFT2; newx2 = oldx2 - 2;
			ch = LEFT2;
			if (snake_length2 == 1) {
				game_over = 1;
				die_decision = 2;
				break;
			}
			snake_length2--; // ���� �ε����� ������ �پ���
			gotoxy2(snake2[snake_length2]);//������ǥ
			printf("  "); // ������ �����
		}
		move_flag = 1;
		keep_moving2 = 1;
		break;
	}
	if (move_flag) {
		showsnake(1);
		oldx2 = newx2; // ������ ��ġ�� ����Ѵ�.
		oldy2 = newy2;
	}
	movesnake(newx2, newy2, 1);
}
// ����Ű�� snake�� �����δ�.
void main()
{

	unsigned char ch; // Ư��Ű 0xe0 �� �Է¹������� unsigned char �� �����ؾ� ��
	unsigned char check_ch = 0; // ���� ������ Ű����

	int i;
	int run_time, run_time2, run_time3, run_time4;
	int start_time;

START:
	ch = init_game(); // ���� �ʱ� ȭ��

	if (ch == 'h')
		ch = how_play_game();

	start_game();
	show_shoot_score(0); show_shoot_score(1);

	start_time = time(NULL);

	while (1) {
		run_time = time(NULL) - start_time;
		run_time2 = time(NULL) - start_time;
		run_time3 = time(NULL) - start_time;
		run_time4 = time(NULL) - start_time;
		if (run_time > gold_time && (run_time % goldinterval == 0)) {
			show_gold();
			gold_time = run_time; // ������ GOLD ǥ�� �ð� ���
		}
		if (run_time2 > gold_time2 && (run_time2 % goldinterval2 == 0)) {
			show_speedUp_gold();
			gold_time2 = run_time2; // ������ GOLD ǥ�� �ð� ���
		}
		if (run_time2 > gold_time2 && (run_time2 % goldDelete2 == 0) && golds_count2 != 0) {
			Delete_speedUp_gold();
			gold_time2 = run_time2; // ������ GOLD ǥ�� �ð� ���
		}
		if (run_time3 > gold_time3 && (run_time3 % goldinterval3 == 0)) {
			show_speedDown_gold();
			gold_time3 = run_time3; // ������ GOLD ǥ�� �ð� ���
		}
		if (run_time3 > gold_time3 && (run_time3 % goldDelete3 == 0) && golds_count3 != 0) {
			Delete_speedDown_gold();
			gold_time3 = run_time3; // ������ GOLD ǥ�� �ð� ���
		}
		if (run_time4 > Shootings_time && (run_time4 % Shootings_interval == 0)) {
			show_shooting_gold();
			Shootings_time = run_time4; // ������ GOLD ǥ�� �ð� ���
		}
		
		if ((clock() - gold_start_time1) / CLOCKS_PER_SEC >= gold_runtime1[0] && 
			gold_runtime1[0] > 0) {
			p1_frame_sync = 2;
			gold_runtime1[0] = 0;
			gotoxy(WIDTH - 3, 4);
			printf("����");
		}
		if ((clock() - gold_start_time2) / CLOCKS_PER_SEC >= gold_runtime2[0] &&
			gold_runtime2[0] > 0) {
			p2_frame_sync = 2;
			gold_runtime2[0] = 0;
			gotoxy(0, 4);
			printf("����");
		}
		if ((clock() - gold_start_time1) / CLOCKS_PER_SEC >= gold_runtime1[1] &&
			gold_runtime1[1] > 0) {
			p1_frame_sync = 2;
			gold_runtime1[1] = 0;
			gotoxy(WIDTH - 3, 4);
			printf("����");
		}
		if ((clock() - gold_start_time2) / CLOCKS_PER_SEC >= gold_runtime2[1] &&
			gold_runtime2[1] > 0) {
			p2_frame_sync = 2;
			gold_runtime2[1] = 0;
			gotoxy(0, 4);
			printf("����");
		}

		if (kbhit() == 1) {  // Ű���尡 ������ ������
			char c1;
			c1 = getch(); // key ���� �д´�
			if (c1 == '\0') // VS �� NULL ���� Error�̴�.
				continue;
			else
				ch = c1; // NULL�� �ƴϸ� ���� �Է��̴�.
			if (ch == ESC) { // ESC ������ ���α׷� ����
				game_over = 1;
				if (snake_length < snake_length2)
					die_decision = 1;
				else if (snake_length > snake_length2)
					die_decision = 2;
				else
					die_decision = 0;
				goto QUIT;
			}

			for (int i = 0; i < 5; i++) {
				if (shooting1[i])
					move_Shoot(1, shoot_ch1[i], i);
				if (shooting2[i])
					move_Shoot(2, shoot_ch2[i], i);
			}
			if (ch == SPECIAL1 || ch == SPECIAL2) { // ���� Ư��Ű
				// ���� ��� UP key�� ��� 0xe0 0x48 �ΰ��� ���ڰ� ���´�.
				ch = getch();
				switch (ch) {
				case UP:
					if (snake[1].X == snake[0].X && snake[1].Y == snake[0].Y - 1 && ch == UP) {//�������� �̵� �Ұ�
						ch = 0;
						break;
					}
				case DOWN:
					if (snake[1].X == snake[0].X && snake[1].Y == snake[0].Y + 1 && ch == DOWN) {
						ch = 0;
						break;
					}
				case LEFT:
					if (snake[1].X == snake[0].X - 2 && snake[1].Y == snake[0].Y && ch == LEFT) {
						ch = 0;
						break;
					}
				case RIGHT:
					if (snake[1].X == snake[0].X + 2 && snake[1].Y == snake[0].Y && ch == RIGHT) {
						ch = 0;
						break;
					}
					player1(ch);//player1�� ���� ��ȯ
					if (frame_count % p2_frame_sync == 0)
						player2(0);
					break;
				default: // ����Ű�� �ƴϸ� ����� 1�̸� ��� ����
					if (frame_count % p1_frame_sync == 0)
						player1(0);
					if (frame_count % p2_frame_sync == 0)
						player2(0);
				}
			}
			else if (ch == SHOOT1 || ch == SHOOT2) {
				int k = 0;
				switch (ch) {
				case SHOOT1:
					if (shooting_score[0] == 0)
						break;
					for (int t = 0; shooting1[t] != 0; t++)
						k++;
					shooting1[k] = 1;
					shoot_ch1[k] = check_ch;
					shooting_score[0]--;
					show_shoot_score(0);
					Shoot(1, shoot_ch1[k], k);
					break;
				case SHOOT2:
					if (shooting_score[1] == 0)
						break;
					for (int t = 0; shooting1[t] == 1; t++)
						k++;
					shooting2[k] = 1;
					shoot_ch2[k] = check_ch;
					shooting_score[1]--;
					show_shoot_score(1);
					Shoot(2, shoot_ch2[k], k);
					break;
				}
			}
			else {
				// Player2�� AWSD �� �����δ�.
				switch (ch) {
				case UP2:
					if (snake2[1].X == snake2[0].X && snake2[1].Y == snake2[0].Y - 1 && ch == UP2) { //�������� �̵� �Ұ�
						ch = 0;
						break;
					}
				case DOWN2:
					if (snake2[1].X == snake2[0].X && snake2[1].Y == snake2[0].Y + 1 && ch == DOWN2) {
						ch = 0;
						break;
					}
				case LEFT2:
					if (snake2[1].X == snake2[0].X - 2 && snake2[1].Y == snake2[0].Y && ch == LEFT2) {
						ch = 0;
						break;
					}
				case RIGHT2:
					if (snake2[1].X == snake2[0].X + 2 && snake2[1].Y == snake2[0].Y && ch == RIGHT2) {
						ch = 0;
						break;
					}
					player2(ch);
					player1(0);
					break;
				default:// ���� ��ȯ�� �ƴϸ�
					player1(0);
					player2(0);
				}
			}
			check_ch = ch;
		}
		else {
			// keyboard �� �������� ������ ��� �����δ�.
			// �̵����̴� �������� ��� �̵�
			if (frame_count % p1_frame_sync == 0)
				player1(0);
			if (frame_count % p2_frame_sync == 0)
				player2(0);
		}
		if (frame_count % gold_frame_sync2 == 0)
			move_gold2(); // GOLD �� ��ġ�� �����Ѵ�.
		if (frame_count % gold_frame_sync3 == 0)
			move_gold3(); // GOLD �� ��ġ�� �����Ѵ�
		if (frame_count % gold_frame_sync4 == 0)
			move_shooting(); // GOLD �� ��ġ�� �����Ѵ�.

		Sleep(Delay); // Delay�� ���̸� �ӵ��� ��������.
		frame_count++; // frame_count ������ �ӵ� ������ �Ѵ�.

		die_decision = slap_die(); //�����̳��� ��ġ������ ��
		if (die_decision == 1 || die_decision == 2 || die_decision == 0)
			game_over = 1;

	QUIT:
		if (game_over == 1) {
			ch = end_game();

			if (ch == 'r')
				goto START;

			gotoxy(25, 23);
			break;
		}
	}
}