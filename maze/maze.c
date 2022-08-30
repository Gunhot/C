#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct _room {
	short down_flag;
	short right_flag;
	int set;
}room;
int WIDTH;
int HEIGHT;
room** maze;

void build_maze() {
	int h = 0;
	int w = 0;

	//격자무늬로 배열된 방들에서부터 시작한다.
	//아래벽을 부시면서 통합되지 않은 방을 최신화 해줄 필요 없도록 모든 줄을 초기화 한다.
	int init_set = 0;
	for (h = 0; h < HEIGHT; h++) {
		for (w = 0; w < WIDTH; w++) {
			maze[h][w].set = ++init_set;
			maze[h][w].down_flag = 1;
			maze[h][w].right_flag = 1;
		}
	}

	//현재 행에서 첫 번째 방부터 시작하여 마지막 방까지 
	for (h = 0; h < HEIGHT; h++) {
		if (h == HEIGHT - 1) {
			for (w = 0; w < WIDTH - 1; w++) {
				//둘의 집합이 다를 때
				if (maze[h][w].set != maze[h][w + 1].set) {
					//필수 : 벽을 허물어야함
					maze[h][w].right_flag = 0;
					//집합을 합친다.
					int erase = maze[h][w + 1].set;
					for (int i = h; i >= 0; i--) {
						for (int j = 0; j < WIDTH; j++) {
							if (maze[i][j].set == erase)maze[i][j].set = maze[h][w].set;
						}
					}
				}
			}
		}
		else {
			for (w = 0; w < WIDTH - 1; w++) {
				//둘의 집합이 다를 때만 -> cycle 방지
				if (maze[h][w].set != maze[h][w + 1].set) {
					//벽의 제거 여부를 결정한다 
					if ((maze[h][w].right_flag = rand() % 2) == 0) {
						//제거했을 경우 집합을 통일
						int erase = maze[h][w + 1].set;
						for (int i = h; i >= 0; i--) {
							for (int j = 0; j < WIDTH; j++) {
								if (maze[i][j].set == erase)maze[i][j].set = maze[h][w].set;
							}
						}
					}
				}
			}
			//check가 0일 경우 해당 집합이 아직 폐쇄되어 있음을 나타낸다.
			int check = 0;
			for (w = 0; w < WIDTH; w++) {
				if ((maze[h][w].down_flag = rand() % 2) == 0) {
					maze[h + 1][w].set = maze[h][w].set;

					//가장 위
					if (h == 0) {
						//아래가 뚫려있을 경우
						if (maze[h][w].down_flag == 0) {
							check = 1;
						}
					}
					//가장 아래
					else if (h == HEIGHT - 1) {
						//위에가 뚫려있을 경우
						if (maze[h][w - 1].down_flag == 0) {
							check = 1;
						}
					}
					//중간
					else {
						//위아래로 하나라도 뚫려있을 경우
						if (maze[h][w].set == maze[h + 1][w].set || maze[h][w].set == maze[h - 1][w].set) {
							check = 1;
						}
					}
				}
				//set가 바꼈는데도 check가 0일 경우 반드시 아래벽을 제거해야함
				else {

					if (((w != WIDTH - 1 && maze[h][w].set != maze[h][w + 1].set) || w == WIDTH - 1) && check == 0) {
						maze[h][w].down_flag = 0;
						maze[h + 1][w].set = maze[h][w].set;
					}
				}
				//set가 바꼈다면 check는 초기화
				if (maze[h][w].set != maze[h][w + 1].set) check = 0;
			}
		}
	}
}
void print_maze() {
	/*for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			printf("%d ", maze[i][j].set);
		}
		printf("\n");
	}*/
	FILE* out;
	int h, w;
	out = fopen("maze.maz", "w");
	//천장 출력
	for (w = 0; w < WIDTH; w++) fprintf(out, "+-");
	fprintf(out, "+\n");

	//각 방의 right_flag와 down_flag에 맞춰 출력

	for (h = 0; h < HEIGHT; h++) {
		fprintf(out, " |");

		for (w = 0; w < WIDTH; w++) {
			//방출력
			fprintf(out, "  ");
			//오른쪽 벽 출력
			if (maze[h][w].right_flag)fprintf(out, "|");
			else fprintf(out, " ");
		}
		fprintf(out, "\n");

		for (w = 0; w < WIDTH; w++){
			fprintf(out, "+");
			//아래쪽 벽 출력
			if (maze[h][w].down_flag) fprintf(out, "-");
			else fprintf(out, " ");
		}
		fprintf(out, "+\n");
	}
	fclose(out);
}
void main() {
	//N is WIDTH
	scanf("%d", &WIDTH);
	//M is HEIGHT
	scanf("%d", &HEIGHT);


	maze = (room**)malloc(HEIGHT * sizeof(room*));
	for (int i = 0; i < HEIGHT; i++) {
		*(maze + i) = (room*)malloc(WIDTH * sizeof(room));
	}
	srand(time(NULL));

	build_maze();
	print_maze();
	//메모리 해제
	for (int i = 0; i < HEIGHT; i++) {
			free(maze[i]);
	}
	free(maze);
}