/*

	ofxWinMenu basic example - ofApp.h

	Copyright (C) 2016-2017 Lynn Jarvis.

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/

	#pragma once

#include "ofMain.h"
//블록의 item을 random하게 생성하기 위함이다. 
#include <time.h>
//블록 하나의 높이이다.
#define BLOCK_HEI_LEN 50
//블록의 가로 개수이다.
#define BLOCK_HEI_NUM 10
//블록 하나의 너비이다.
#define BLOCK_WID_LEN 50
//블록의 세로 개수이다.
#define BLOCK_WID_NUM 10
//바의 너비이다.
#define BAR_WIDTH 100
//playbox의 높이이다.
#define HEIGHT ((BLOCK_HEI_LEN)* 2 * (BLOCK_HEI_NUM))
//playbox의 너비이다.
#define WIDTH ((BLOCK_WID_LEN) * (BLOCK_WID_NUM) )
//왼쪽벽에 공이 부딪혔음을 나타낸다.
#define LEFT_WALL_HIT 1
//오른쪽벽에 공이 부딪혔음을 나타낸다.
#define RIGHT_WALL_HIT 2
//블록에 공이 부딪혔음을 나타낸다.
#define BLOCK_HIT 3
//바에 공이 부딪혔음을 나타낸다.
#define BAR_HIT 4
//천장에 공이 부딪혔음을 나타낸다.
#define TOP_HIT 5
//바닥에 공이 부딛혔음을 나타낸다.
//공이 더 이상 존재하지 않을 경우도 나타낸다.
#define BOTTOM_HIT 6
//블록의 왼쪽에 공이 부딪혔음을 나타낸다.
#define LEFT 1
//블록의 위쪽에 공이 부딪혔음을 나타낸다.
#define TOP 2
//블록의 오른쪽에 공이 부딪혔음을 나타낸다.
#define RIGHT 3
//블록의 아래쪽에 공이 부딪혔음을 나타낸다.
#define BOTTOM 4
//item의 종류 중 하나인 BLUE를 나타낸다
#define BLUE 1
//item의 종류 중 하나인 RED를 나타낸다.
#define RED 2

//공의 구조이다
struct Ball {
	//현재 공의 좌표를 나타낸다
	int x;
	int y;
	//현재 공의 반지름을 나타낸다
	int r;
	//공의 x좌표의 이동 정도를 나타낸다
	int x_mov;
	//공의 y좌표의 이동 정도를 나타낸다
	int y_mov;
};
//item들을 연결할 때 사용할 노드이다
struct node {
	//블록의 index를 나타낸다
	int x;
	int y;
	//다음 item을 가리킨다.
	struct node* link;
};
typedef node* nodepointer;
class ofApp : public ofBaseApp {

	public:
		//프로그램이 실행되면 가장 먼저 실행되는 함수이다.
		//변수들을 설정하고 초기 setting을 진행한다.
		void setup();
		//update를 해준다.
		void update();
		//update된 정보를 바탕으로 새로 그린다.
		void draw();
		//item들을 연결하는 blocklist이다.
		nodepointer blocklist[BLOCK_HEI_NUM][BLOCK_WID_NUM];
		//item block b를 item block a에 연결한다.
		void addlist(nodepointer a, nodepointer b);
		//공과 블록이 놓여지는 field이다.
		ofRectangle play_box;
		
		//각 블록의 정보를 저장한다
		//ofRectangle의 member로 item과 exist를 추가하여
		//item의 종류와 블록의 존재 유무를 나타냈다.
		ofRectangle block[BLOCK_HEI_NUM][BLOCK_WID_NUM];
		//bar의 위치와 크기를 나타낸다.
		ofRectangle bar;
		//공의 위치와 크기 이동정도를 저장한다.
		Ball ball;
		
		//공과 블록이 부딪혔을 때 부딪힌 블록의 인덱스를 저장한다
		int hitx, hity;
		//공과 블록이 부딪혔을 때 부딪힌 블록의 면을 저장한다
		int side;
		//gameover 됐을 때 1로 set된다
		int gameover;
		//존재하는 블록의 개수를 센다.
		int curblock_num;

		//공을 움직이는 함수이다
		int move_ball();
		//keyboard의 입력 값을 처리하는 함수이다
		void keyPressed(int key); // Traps escape key if exit disabled
		//동적할당한 메모리를 해제한다.
		void freeMemory();
		//문구를 출력할 때 사용한다.
		ofTrueTypeFont verdana;

};
