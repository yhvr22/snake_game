#include <iostream>
#include <time.h>
#include <cstdlib>
#include <windows.h>
#include <process.h>
#include <conio.h>
using namespace std;

//negative values represent areas that cannot be touched or else game over
#define WALL -2
#define SNAKE -1
#define NOTHING 0
#define MAX 100
#define WIDTH 77
#define HEIGHT 22
#define INIT_SNAKE_LEn 4
#define FOOD 1




#define R 0
#define U 1
#define L 2
#define DW 3
#define EXIT -1
static int dx[5] = { 1, 0, -1, 0 };
static int dy[5] = { 0, -1, 0, 1 };


int in = R;
int itm = NOTHING;

void gotoxy(int column, int row)
{
	HANDLE hStdOut;
	COORD coord;

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	coord.X = column;
	coord.Y = row;
	SetConsoleCursorPosition(hStdOut, coord);
}

void clsscreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;


	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;


	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
		)) return;


	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
        cellCount,
		homeCoords,
		&count
		)) return;


	SetConsoleCursorPosition(hStdOut, homeCoords);
}


int opsdir(int in1, int in2)
{
	if (in1 == L && in2 == R)
		return 1;
	if (in1 == R && in2 == L)
		return 1;
	if (in1 == U && in2 == DW)
		return 1;
	if (in1 == DW && in2 == U)
		return 1;

	return 0;
}

struct coordinate
{
	int x, y;
};

class snake
{
private:
	int len;
	coordinate bdy[WIDTH*HEIGHT];
	int dir;
	int grnd[MAX][MAX];
	int foodcou;
public:
	void initgroun();
	void initsnake();
	void updatesnake(int delay);
	void updatefood();
	void firstdraw();
	int getfoodcou();
};

void snake::initgroun()
{
	int i, j;
	for (i = 0; i < MAX; i++)
		for (j = 0; j < MAX; j++)
			grnd[i][j] = 0;

	for (i = 0; i <= WIDTH + 1; i++)
	{

		grnd[0][i] = WALL;
		grnd[HEIGHT + 1][i] = WALL;
	}
	for (i = 0; i <= HEIGHT + 1; i++)
	{

		grnd[i][0] = WALL;
		grnd[i][WIDTH + 1] = WALL;
	}
}

void snake::initsnake()
{
	len = INIT_SNAKE_LEn;
	bdy[0].x = WIDTH / 2;
	bdy[0].y = HEIGHT / 2;
	dir = in;
	foodcou = 0;

	int i;
	for (i = 1; i < len; i++)
	{
		bdy[i].x = bdy[i - 1].x - dx[dir];
		bdy[i].y = bdy[i - 1].y - dy[dir];
	}

	for (i = 0; i < len; i++)
		grnd[bdy[i].y][bdy[i].x] = SNAKE;
}

void snake::updatesnake(int delay)
{
	int i;
	coordinate prv[WIDTH*HEIGHT];
	for (i = 0; i < len; i++)
	{
		prv[i].x = bdy[i].x;
		prv[i].y = bdy[i].y;
	}

	if (in != EXIT && !opsdir(dir, in))
		dir = in;

	bdy[0].x = prv[0].x + dx[dir];
	bdy[0].y = prv[0].y + dy[dir];
	if (grnd[bdy[0].y][bdy[0].x] < NOTHING)
	{
		itm = -1;
		return;
	}

	if (grnd[bdy[0].y][bdy[0].x] == FOOD)
	{
		len++;
		itm = FOOD;
	}
	else
	{
		grnd[bdy[len - 1].y][bdy[len - 1].x] = NOTHING;
		itm = NOTHING;
		gotoxy(bdy[len - 1].x, bdy[len - 1].y);
		cout << " ";
	}

	for (i = 1; i < len; i++)
	{
		bdy[i].x = prv[i - 1].x;
				bdy[i].y = prv[i - 1].y;
	}

	gotoxy(bdy[1].x, bdy[1].y);
	cout << "+";
	gotoxy(bdy[0].x, bdy[0].y);
	cout << "O";


	for (i = 0; i < len; i++)
		grnd[bdy[i].y][bdy[i].x] = SNAKE;

	Sleep(delay);

	return;
}

void snake::updatefood()
{
	int x, y;
	do
	{
		x = rand() % WIDTH + 1;
		y = rand() % HEIGHT + 1;
	} while (grnd[y][x] != NOTHING);

	grnd[y][x] = FOOD;
	foodcou++;
	gotoxy(x, y);
	cout << "\u2022";
}

void snake::firstdraw()
{
	clsscreen();
	int i, j;
	for (i = 0; i <= HEIGHT + 1; i++)
	{
		for (j = 0; j <= WIDTH + 1; j++)
		{
			switch (grnd[i][j])
			{
			case NOTHING:
				cout << " "; break;
			case WALL:
				if ((i == 0 && j == 0)
					|| (i == 0 && j == WIDTH + 1)
					|| (i == HEIGHT + 1 && j == 0)
					|| (i == HEIGHT + 1 && j == WIDTH + 1))
					cout << "+";
				else
					if (j == 0 || j == WIDTH + 1)
						cout << "|";
					else
						cout << "-";
				break;
			case SNAKE:
				if (i == bdy[0].y && j == bdy[0].x)
					cout << "O";
				else
					cout << "+";
				break;
			default:
			    		cout << "\u2022";
			}
		}
		cout << endl;
	}
}

int snake::getfoodcou()
{
	return foodcou;
}

void userin(void* id)
{
	do
	{
		int c = _getch();
		switch (c)
		{
		case '1': case 'w': in = U   ; break;
		case '2': case 's': in = DW ; break;
		case '3': case 'd': in = R; break;
		case '4': case 'a': in = L ; break;
		case 27:	    in = EXIT ; break;
		}
	} while (in != EXIT && itm >= 0);

	_endthread();
	return;
}

int main()
{
	int delay = 100;
	srand(time(NULL));
	snake s;
	s.initgroun();
	s.initsnake();
	s.updatefood();
	s.firstdraw();
	_beginthread(userin, 0, (void*)0);

	do
	{
		s.updatesnake(delay);
		if (itm == FOOD)
			s.updatefood();
	} while (itm >= 0 && in != EXIT);

	gotoxy(WIDTH / 2 - 5, HEIGHT / 2 - 2);
	cout << "GAME OVER";
	gotoxy(WIDTH / 2 - 8, HEIGHT / 2 + 2);
	cout << "Your score is " << s.getfoodcou() - 1 << "!" << endl;
	gotoxy(WIDTH / 2, HEIGHT / 2);

	_getch();
	return 0;
}
