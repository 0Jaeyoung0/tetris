#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#ifndef _WIN32
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#else
#include <process.h>
#include <windows.h>
#include <conio.h>
#endif
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>

/* 타이머  */
#define CCHAR 0
#ifdef CTIME
#undef CTIME
#endif
#define CTIME 1

/* 왼쪽, 오른쪽, 아래, 회전  */
#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define ROTATE 3

/* 블록 모양 */
#define I_BLOCK 0
#define	T_BLOCK 1
#define S_BLOCK 2
#define Z_BLOCK 3
#define L_BLOCK 4
#define J_BLOCK 5
#define O_BLOCK 6

/* UI 위치 */
#define TETRIS_TABLE_X 0
#define TETRIS_TABLE_Y 0

#define NEXT_BLOCK_TABLE_X 25
#define NEXT_BLOCK_TABLE_Y 2

#define SCORE_TABLE_X 25
#define SCORE_TABLE_Y 10

/* 게임 시작, 게임 종료 */
#define GAME_START 0
#define GAME_END 1

#define FILE_NAME "results.txt"

/*

 * 블록 모양(I, T, S, Z, L, J, O) 
 * 4*4 배열의 2차원 배열
 * 모든 블록의 모양을 표시
 *
 * 블록의 모양을 표시
 * 왼쪽, 오른쪽, 아래, 회전 
 * 4*4 배열의 2차원 배열
 * 모든 블록의 모양을 표시
 *
 * 4*4*4 배열의 3차원 배열
 */


char i_block[4][4][4] = {
    {  // 첫 번째 회전 상태
        {0, 0, 0, 0}, 
		{1, 1, 1, 1}, 
		{0, 0, 0, 0}, 
		{0, 0, 0, 0} 
	},
    {  // 두 번째 회전 상태
        {0, 1, 0, 0}, 
		{0, 1, 0, 0}, 
		{0, 1, 0, 0}, 
		{0, 1, 0, 0} 
	},
    {  // 세 번째 회전 상태
        {0, 0, 0, 0}, 
		{1, 1, 1, 1}, 
		{0, 0, 0, 0}, 
		{0, 0, 0, 0} 
	},
    {  // 네 번째 회전 상태
        {0, 1, 0, 0}, 
		{0, 1, 0, 0}, 
		{0, 1, 0, 0}, 
		{0, 1, 0, 0} 
	}
};

char t_block[4][4][4] = {
	{
		{1, 0, 0, 0},   
		{1, 1, 0, 0},   
		{1, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{1, 1, 1, 0},   
		{0, 1, 0, 0},   
		{0, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 0, 1, 0},   
		{0, 1, 1, 0},   
		{0, 0, 1, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},   
		{0, 1, 0, 0},   
		{1, 1, 1, 0},   
		{0, 0, 0, 0}
	}
};


char s_block[4][4][4] = {
	{
		{1, 0, 0, 0},   
		{1, 1, 0, 0},   
		{0, 1, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 1, 1, 0},   
		{1, 1, 0, 0},   
		{0, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 1, 0, 0},   
		{0, 1, 1, 0},   
		{0, 0, 1, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},   
		{0, 1, 1, 0},   
		{1, 1, 0, 0},   
		{0, 0, 0, 0}
	}
};

char z_block[4][4][4] = {
	{
		{0, 1, 0, 0},   
		{1, 1, 0, 0},   
		{1, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{1, 1, 0, 0},   
		{0, 1, 1, 0},   
		{0, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 0, 1, 0},   
		{0, 1, 1, 0},   
		{0, 1, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},   
		{1, 1, 0, 0},   
		{0, 1, 1, 0},   
		{0, 0, 0, 0}
	}	
};

char l_block[4][4][4] = {
	{
		{0, 1, 0, 0},   
		{0, 1, 0, 0},   
		{0, 1, 1, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},   
		{1, 1, 1, 0},   
		{1, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{1, 1, 0, 0},   
		{0, 1, 0, 0},   
		{0, 1, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 0, 1, 0},   
		{1, 1, 1, 0},   
		{0, 0, 0, 0},   
		{0, 0, 0, 0}
	}
};

char j_block[4][4][4] = {
	{
		{0, 1, 0, 0},   
		{0, 1, 0, 0},   
		{1, 1, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{1, 0, 0, 0},   
		{1, 1, 1, 0},   
		{0, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 1, 1, 0},   
		{0, 1, 0, 0},   
		{0, 1, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},   
		{1, 1, 1, 0},   
		{0, 0, 1, 0},   
		{0, 0, 0, 0}
	}
};


char o_block[4][4][4] = {
	{
		{1, 1, 0, 0},   
		{1, 1, 0, 0},   
		{0, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{1, 1, 0, 0},   
		{1, 1, 0, 0},   
		{0, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{1, 1, 0, 0},   
		{1, 1, 0, 0},   
		{0, 0, 0, 0},   
		{0, 0, 0, 0}
	},
	{
		{1, 1, 0, 0},   
		{1, 1, 0, 0},   
		{0, 0, 0, 0},   
		{0, 0, 0, 0}
	}
};


/* 테트리스 판을 2차원 배열로 표현
 * 2차원 배열의 2차원 배열
 * 모든 블록의 모양을 표시
 *
 * 20*8 배열
 * 모든 블록의 모양을 표시
 * 모든 블록의 모양을 표시*/

char tetris_table[21][10];

/* 게임 종료 땨마다
 * 이름과 득점수와 
 * 날짜와 시간과 순위를 저장
 * */
typedef struct result
{
	char name[30];
	long point;
	int year;
	int month;
	int day;
	int hour;
	int min;
	int rank;
	struct result* next;
} result;

result* head = NULL;

int block_number = 0;  /*블록 번호*/
int next_block_number = 0; /*다음 블록 번호 */
int block_state = 0; /*블록 상태, 왼쪽, 오른쪽, 아래, 회전  */
int next_block_state = 0; /*다음 블록 상태, 왼쪽, 오른쪽, 아래, 회전  */

int x = 3, y = 0; /*블록의 위치*/

int game = GAME_END; /*게임 시작, 게임 종료*/
int best_point = 0; /* 최고 점수*/

long point = 0; /* 현재 점수*/

int delay_count = 4;





char (*get_block_shape(int block_number, int block_state))[4];
int display_menu(void); /* 메뉴 표시*/
int game_start();
int check_collision();
int check_game_over();
int clear_lines(); /* 정리한 라인 수 반환 */
void search_result();
void print_result();
void disable_input_buffering();
void enable_input_buffering();
void disable_input_blocking();
void enable_input_blocking();
void init_tetris_table();
void init_game();
void save_block();
void calc_init_position(); /* 블락마다 초기 시작위치 결정*/
void drop_new_block();
void drop_timer_handler(int signum);
void start_drop_timer();
void stop_drop_timer();
void clear_game();
void save_result();
void display_game(); /* 현재 게임 상태 표시 */
void clear_all();
void move_cursor(int row, int col);
void print_tetris_table();
void print_block(int block_number, int block_state, int x, int y);
void print_score();
void insert_result(result* new_result);
void save_result_to_file();
void load_result_from_file();
result* new_result();




/// 테트리스 게임 메인 함수
/// 메뉴를 표시하고 사용자의 선택에 따라 게임을 시작하거나 결과를 검색하거나 종료합니다.
/// @param  
/// @return 
int main(void)
{
	int menu = 1;
	load_result_from_file();

	while(menu)
	{
		menu = display_menu();

		if(menu == 1)
		{
			game = GAME_START;
			menu = game_start();
		}
		else if(menu == 2)
		{
			search_result();
		}
		else if(menu == 3)
		{
			print_result();
		}
		else if(menu == 4)
		{
			exit(0);
		}
	}

	return 0;
}

/* 메뉴 표시*/
int display_menu(void)
{
	// 
	int menu = 0;

	while(1)
	{
		clear_all();
		printf("\n\n\t\t\t\tText Tetris");
		printf("\n\t\t\t============================");
		printf("\n\t\t\t\tGAME MENU\t\n");
		printf("\n\t\t\t============================");
		printf("\n\t\t\t   1) Game Start");
		printf("\n\t\t\t   2) Search history");
		printf("\n\t\t\t   3) Record Output");
		printf("\n\t\t\t   4) QUIT");
		printf("\n\t\t\t============================");
		printf("\n\t\t\t\t\t SELECT : ");
		scanf("%d", &menu);
		while(getchar() != '\n');

		if(menu < 1 || menu > 4)
		{
			continue;
		}
		else
		{
			return menu;
		}
	}
	return 0;
}

int game_start()
{
	init_game();

	while(game == GAME_START)
	{
		#ifndef _WIN32
		char ch = getchar();
		#else
		char ch = getch();
		#endif

		switch(ch)
		{
			case EOF:
			{
				continue;
				break;
			}

			case 'j':
			{
				x--;
				if(check_collision())
				{
					x++;
				}
				break;
			}

			case 'l':
			{
				x++;
				if(check_collision())
				{
					x--;
				}
				break;
			}

			case 'k':
			{
				y++;
				if(check_collision())
				{
					y--;
					delay_count--;
				}
				break;
			}

			case 'i':
			{
				int prev_state = block_state;
				block_state = (block_state + 1) % 4;
				if(check_collision())
				{
					block_state = prev_state;
				}
				break;
			}

			case 'a':
			{
				while(!check_collision())
				{
					y++;
				}
				y--;
				save_block();
				int clear_line_num = clear_lines();
				if(check_game_over())
				{
					game = GAME_END;
				}
				else
				{
					point += 100 * clear_line_num + 10;
					drop_new_block();
				}
				break;
			}

			case 'p':
			{
				game = GAME_END;
				break;
			}

			default:
				break;
		}

		display_game();
	}

	clear_game();

	return 1;
}

void search_result()
{
	clear_all();

	if(head == NULL)
	{
        printf("No saved results.\n");
    }
	else
	{
		char search_name[30];
		printf("Enter name to search: ");
		scanf("%29s", search_name);
		while(getchar() != '\n');

		printf("\n==== Search Results for '%s' ====\n", search_name);
    	printf("%-5s %-30s %-10s %-12s %-6s\n", "Rank", "Name", "Points", "Date", "Time");
    	printf("---------------------------------------------------------------\n");

		int found = 0;
		result* temp = head;
		while(temp != NULL)
		{
			if(strcmp(temp -> name, search_name) == 0)
			{
				printf(
					"%-5d %-30s %-10ld %04d-%02d-%02d   %02d:%02d\n",
					temp -> rank,
					temp -> name,
					temp -> point,
					temp -> year,
					temp -> month,
					temp -> day,
					temp -> hour,
					temp -> min
				);

				found = 1;
			}
			temp = temp -> next;
		}	
		
		if(!found)
		{
			printf("No results found for '%s'.\n", search_name);
		}
		
		printf("\n===============================\n");
	}

	printf("Press Enter to continue...");
    getchar();
}

void print_result()
{
    clear_all();

    if(head == NULL)
	{
        printf("No saved results.\n");
    }
	else
	{
		printf("==== Saved Results ====\n");
		printf("%-5s %-30s %-10s %-12s %-6s\n", "Rank", "Name", "Point", "Date", "Time");
		printf("---------------------------------------------------------------\n");
	
		result* temp = head;
		while(temp != NULL)
		{
			printf(
				"%-5d %-30s %-10ld %04d-%02d-%02d   %02d:%02d\n",
				temp -> rank,
				temp -> name,
				temp -> point,
				temp -> year,
				temp -> month,
				temp -> day,
				temp -> hour,
				temp -> min
			);
			temp = temp -> next;
		}
	}

    printf("Press Enter to continue...");
    getchar();
}

void disable_input_buffering()
{
	#ifndef _WIN32
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag &= ~ICANON;
    t.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &t);
	#endif
}

void enable_input_buffering()
{
	#ifndef _WIN32
    struct termios t;
    tcgetattr(0, &t);
    t.c_lflag |= ICANON;
    t.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &t);
	#endif
}

void disable_input_blocking()
{
	#ifndef _WIN32
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	#endif
}

void enable_input_blocking()
{
	#ifndef _WIN32
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
	#endif
}

void init_tetris_table()
{
	for(int i = 0; i < 21; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			if(i == 20)
			{
				tetris_table[i][j] = 2;
			}
			else
			{
				if(j == 0 || j == 9)
				{
					tetris_table[i][j] = 2;
				}
				else
				{
					tetris_table[i][j] = 0;
				}
			}
		}
	}
}

void init_game()
{
	disable_input_buffering();
	disable_input_blocking();
	init_tetris_table();
	srand(time(NULL));

	delay_count = 4;
	block_number = rand() % 7;
	block_state = rand() % 4;
	next_block_number = rand() % 7;
	next_block_state = rand() % 4;
	calc_init_position();

	point = 0;

	if(head == NULL)
	{
		best_point = 0;
	}
	else
	{
		best_point = head -> point;
	}

	#ifndef _WIN32
	signal(SIGALRM, drop_timer_handler);
	#endif

    start_drop_timer();
}

void display_game()
{
	clear_all();
	print_tetris_table();
	print_block(block_number, block_state, x, y);
	move_cursor(NEXT_BLOCK_TABLE_Y, NEXT_BLOCK_TABLE_X);
	printf("< NEXT BLOCK >");
	print_block(next_block_number, next_block_state, NEXT_BLOCK_TABLE_X / 2 + 2, NEXT_BLOCK_TABLE_Y + 2);
	print_score();
	fflush(stdout);
}

void clear_all()
{
	#ifndef _WIN32
	system("clear");
	#else
	system("cls");
	#endif
}

void print_tetris_table()
{
	move_cursor(TETRIS_TABLE_Y, TETRIS_TABLE_X);

	for(int i = 0; i < 21; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			if(tetris_table[i][j] == 0)
			{
				printf("  ");
			}
			else if(tetris_table[i][j] == 1)
			{
				printf("■ ");
			}
			else
			{
				printf("□ ");
			}
		}
		printf("\n");
	}
}

void print_block(int block_number, int block_state, int x, int y)
{
	char (*block_shape)[4] = get_block_shape(block_number, block_state);

	for(int i = 0; i < 4; i++) 
	{
        for(int j = 0; j < 4; j++) 
		{
            if(block_shape[i][j] == 1)
			{
				if(y + i >= 0)
				{
					move_cursor(y + i, 2 * x + 2 * j);
                	printf("■ ");
				}
            }
        }
    }
}

void print_score()
{
	move_cursor(SCORE_TABLE_Y, SCORE_TABLE_X);
	printf("Score: %ld", point);
	move_cursor(SCORE_TABLE_Y + 1, SCORE_TABLE_X);
	printf("Best Score: %ld", best_point <= point ? point : best_point);
}

// 커서를 (row, col) 위치로 이동 (0부터 시작)
void move_cursor(int row, int col)
{
    printf("\033[%d;%dH", row + 1, col + 1);
}

char (*get_block_shape(int block_number, int block_state))[4]
{
	switch(block_number)
	{
		case I_BLOCK:
			return i_block[block_state];

		case T_BLOCK:
			return t_block[block_state];
		
		case S_BLOCK:
			return s_block[block_state];

		case Z_BLOCK:
			return z_block[block_state];

		case L_BLOCK:
			return l_block[block_state];

		case J_BLOCK:
			return j_block[block_state];

		case O_BLOCK:
			return o_block[block_state];
		
		default:
			return NULL;
	}
}

int check_collision()
{
	char (*block_shape)[4] = get_block_shape(block_number, block_state);

	for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(block_shape[i][j] == 1)
            {
				int new_x = x + j;
                int new_y = y + i;

                if(tetris_table[new_y][new_x] == 1 || tetris_table[new_y][new_x] == 2)
				{
					return 1;
				}
            }
        }
    }

    return 0;
}

void save_block()
{
	char (*block_shape)[4] = get_block_shape(block_number, block_state);

	for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(block_shape[i][j] == 1)
            {
				int new_x = x + j;
                int new_y = y + i;

				if((new_x > 0 && new_x < 9) && (new_y >= 0 && new_y < 20))
                	tetris_table[new_y][new_x] = 1;
            }
        }
    }
}

void drop_new_block()
{
	delay_count = 4;
	block_number = next_block_number;
	block_state = next_block_state;
	next_block_number = rand() % 7;
	next_block_state = rand() % 4;
	calc_init_position();
}

void calc_init_position() /* 블락마다 초기 시작위치 결정*/
{
	char (*block_shape)[4] = get_block_shape(block_number, block_state);

	x = 3;
	y = -4;

	for(int i = 3; i >= 0; i--)
    {
        for(int j = 0; j < 4; j++)
        {
            if(block_shape[i][j] == 1)
            {
				return;
            }
        }

		y++;
    }
}

int clear_lines()
{
	int lines = 0;

    for(int i = 19; i >= 0; i--)
    {
        int full = 1;
        for(int j = 1; j < 9; j++)
        {
            if(tetris_table[i][j] == 0)
            {
                full = 0;
                break;
            }
        }

        if(full)
        {
            lines++;

            for(int k = i; k > 0; k--)
            {
                for(int j = 1; j < 9; j++)
                {
                    tetris_table[k][j] = tetris_table[k - 1][j];
                }
            }

            for(int j = 1; j < 9; j++)
            {
                tetris_table[0][j] = 0;
            }

            i++; // 같은 줄을 다시 검사
        }
    }

    return lines;
}

int check_game_over()
{
	for(int i = 1; i < 9; i++)
	{
		if(tetris_table[0][i] == 1)
		{
			return 1;
		}
	}

	return 0;
}

void drop_timer_handler(int signum)
{
	#ifndef _WIN32
    y++;
    if(check_collision())
	{
        y--;
		delay_count--;

		if(delay_count <= 0)
		{
			save_block();
			int clear_line_num = clear_lines();
			if(check_game_over())
			{
				game = GAME_END;
			}
			else
			{
				point += 100 * clear_line_num + 10;
				drop_new_block();
			}
		}
    }

    display_game();
	#else
	while(game == GAME_START)
	{
		Sleep(1000);
		y++;
		if(check_collision())
		{
			y--;
			delay_count--;

			if(delay_count <= 0)
			{
				save_block();
				int clear_line_num = clear_lines();
				if(check_game_over())
				{
					game = GAME_END;
				}
				else
				{
					point += 100 * clear_line_num + 10;
					drop_new_block();
				}
			}
		}

		display_game();
	}

	_endthread();
	#endif
}

void start_drop_timer()
{
	#ifndef _WIN32
    struct itimerval timer;
    timer.it_value.tv_sec = CTIME;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = CTIME;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
	#else
	_beginthread((void(*)(void*))drop_timer_handler, 0, NULL);
	#endif
}

void stop_drop_timer()
{
	#ifndef _WIN32
    struct itimerval timer;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
	#endif
}

void clear_game()
{
	stop_drop_timer();
	enable_input_blocking();
	enable_input_buffering();

	clear_all();

	printf("====================================\n");
	printf("              GAME OVER             \n");
	printf("====================================\n\n");
	printf("       Your score: %ld\n\n", point);
	printf("       Enter your name: ");

	save_result();
}

void save_result()
{
	result* temp = new_result();

	scanf("%29s", temp -> name);
	while(getchar() != '\n');

	temp -> point = point;

	time_t now = time(NULL);
    struct tm* t = localtime(&now);

    temp -> year = t -> tm_year + 1900;
    temp -> month = t -> tm_mon + 1;
    temp -> day = t -> tm_mday;
    temp -> hour = t -> tm_hour;
    temp -> min = t -> tm_min;

	insert_result(temp);
	save_result_to_file();
}

result* new_result()
{
	result* temp = (result* )malloc(sizeof(result));

	strcpy(temp -> name, "");
	temp -> point = 0;
    temp -> year = 0;
    temp -> month = 0;
    temp -> day = 0;
    temp -> hour = 0;
    temp -> min = 0;
    temp -> rank = 0;

	temp -> next = NULL;

	return temp;
}

void insert_result(result* new_result)
{
	if(head == NULL)
	{
		new_result -> rank = 1;
		head = new_result;
		return;
	}

	result* temp = head;

	if(new_result -> point > head -> point)
	{
		new_result -> rank = 1;
		new_result -> next = head;
        head = new_result;
	}
	else
	{
		while(temp -> next != NULL && temp -> next -> point >= new_result -> point)
		{
			temp = temp -> next;
		}

		new_result -> rank = temp -> rank + 1;
		new_result -> next = temp -> next;
		temp -> next = new_result;
	}

	temp = new_result -> next;
	long current_rank = new_result -> rank + 1;
	while(temp != NULL)
	{
		temp -> rank = current_rank;
		
		temp = temp -> next;
		current_rank++;
	}
}

void save_result_to_file()
{
	FILE* file = fopen(FILE_NAME, "w");
    if(file == NULL)
	{
        printf("\nFile write fail\n");
        return;
    }

    result* temp = head;
    while(temp != NULL)
	{
        fprintf(
            file,
            "%s,%ld,%d,%d,%d,%d,%d,%d\n",
			temp -> name,
			temp -> point,
			temp -> year,
			temp -> month,
			temp -> day,
			temp -> hour,
			temp -> min,
			temp -> rank
        );

        temp = temp->next;
    }

    fclose(file);
}

void load_result_from_file()
{
	FILE* file = fopen(FILE_NAME, "r");
    if(file == NULL) 
	{
        return;
    }

	result* tmp;
	char line[256];
	while(fgets(line, sizeof(line), file))
	{
		result* temp = new_result();

		sscanf(
            line,
            "%[^,],%ld,%d,%d,%d,%d,%d,%d",
            temp -> name,
            &temp -> point,
            &temp -> year,
            &temp -> month,
            &temp -> day,
            &temp -> hour,
            &temp -> min,
            &temp -> rank
        );

		if(head == NULL)
		{
			head = temp;
		}
		else
		{
			tmp -> next = temp;
		}

		tmp = temp;
	}

	fclose(file);
}