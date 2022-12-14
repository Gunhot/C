/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

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

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	//속도 조정
	ofSetFrameRate(15);
	//배경색깔 설정
	ofBackground(0);
	//게임화면을 screen에 가득 차게 한다
	ofSetFullscreen(true);

	//play_box의 왼쪽 모서리의 좌표는 10 , 10이다.
	play_box.x = 10;
	play_box.y = 10;
	//play_box의 너비와 높이를 설정한다.
	play_box.width = WIDTH;
	play_box.height = HEIGHT;
	for (int i = 0; i < BLOCK_HEI_NUM; i++) {
		for (int j = 0; j < BLOCK_WID_NUM; j++) {
			//블록은 초기에 모두 존재한다
			block[i][j].exist = 1;
			//블록의 x,y좌표를 설정한다
			block[i][j].y = i * BLOCK_HEI_LEN + 10;
			block[i][j].x = j * BLOCK_WID_LEN + 10;
			//블록의 크기를 설정한다
			block[i][j].height = BLOCK_HEI_LEN;
			block[i][j].width = BLOCK_WID_LEN;
			//블록의 item종류를 random하게 설정한다
			//item이 1이상 될 경우 item이 존재하게 되는 것인데
			//0 ~ 9 중 8과 9만 item이 1이게 되는 것이므로 20프로의 확률로 item을 갖게된다.
			block[i][j].item = (int)((rand() % 10) / 8);
			//item중 일부만 1을 더하여 RED item도 갖게한다.
			if (block[i][j].item) {
				block[i][j].item += rand() % 2;
			}

		}
	}


	for (int i = 0; i < BLOCK_HEI_NUM; i++) {
		for (int j = 0; j < BLOCK_WID_NUM; j++) {
			//블록별로 노드를 생성한다.
			blocklist[i][j] = (nodepointer)malloc(sizeof(struct node));
			blocklist[i][j]->y = i;
			blocklist[i][j]->x = j;
			blocklist[i][j]->link = NULL;
		}
	}
	//	같은 item의 블록을 연결한다.
	for (int i = 0; i < BLOCK_HEI_NUM; i++) {
		for (int j = 0; j < BLOCK_WID_NUM; j++) {
			//item인 블록일 경우
			//해당 item과 같은 item을 갖는 블록을 찾아 연결한다.
			if (block[i][j].item) {
				for (int k = 0; k < BLOCK_HEI_NUM; k++) {
					for (int l = 0; l < BLOCK_WID_NUM; l++) {
						if (block[i][j].item == block[k][l].item) {
							addlist(blocklist[i][j], blocklist[k][l]);
						}
					}
				}
			}
		}
	}
	//글씨의 폰트를 verdana로 설정한다.
	verdana.loadFont("verdana.ttf", 30);
	
	//bar의 위치를 설정한다
	bar.y = HEIGHT - 10;
	bar.x = WIDTH / 2 - 100;
	//bar의 높이를 설정한다
	bar.height = 5;
	//bar의 너비를 설정한다
	bar.width = BAR_WIDTH;

	//공의 위치를 설정한다
	ball.x = WIDTH / 2;
	ball.y = HEIGHT - 50;
	//공이 움직이는 정도를 설정한다
	ball.x_mov = -10;
	ball.y_mov = 10;
	//공의 반지름을 설정한다.
	ball.r = 10;
	//gameover의 flag를 0으로 set한다.
	gameover = 0;
	//현재블록의 개수를 가리키는 curblock_num을 전체 블록의 개수로 초기화한다.
	curblock_num = BLOCK_HEI_NUM * BLOCK_WID_NUM;


} // end Setup

void ofApp::addlist(nodepointer a, nodepointer b) {
	//b의 정보를 갖는 temp node를 만든다.
	nodepointer temp = (nodepointer)malloc(sizeof(struct node));
	temp->x = b->x;
	temp->y = b->y;
	temp->link = NULL;
	//a의 가장 끝에 temp를 연결한다.
	nodepointer cur = a;
	while (cur->link) {
		cur = cur->link;
	}
	cur->link = temp;
}

//--------------------------------------------------------------
int ofApp::move_ball() {
	//공의 이동정도에 따라 공을 이동시킨다.
	ball.x += ball.x_mov;
	ball.y += ball.y_mov;
	//공이 왼쪽벽에 부딪힐 경우
	if (ball.x <= 25) {
		return LEFT_WALL_HIT;
	}
	//공이 오른쪽벽에 부딪힐 경우
	else if (ball.x >= WIDTH - 10) {
		return RIGHT_WALL_HIT;
	}
	//공이 바에 부딪힐 경우
	else if ((ball.x >= bar.x + 10 && ball.x <= bar.x + BAR_WIDTH - 10) && ball.y - 10 >= HEIGHT - 30) {
		return BAR_HIT;
	}
	//공이 바닥에 부딪힐 경우
	else if (ball.y >= HEIGHT - 15) {
		return BOTTOM_HIT;
	}
	side = 0; hitx = 0; hity = 0; curblock_num = 0;
	for (int i = 0; i < BLOCK_HEI_NUM; i++) {
		for (int j = 0; j < BLOCK_WID_NUM; j++) {
			//블록이 존재할 경우
			if (block[i][j].exist) {
				//존재하는 블록의 개수를 센다
				curblock_num++;
				//블록의 중심으로부터 공까지의 거리를 판단하여 블록과 부딪혔는지를 판단한다.
				if ((ball.x - (block[i][j].x + 25)) * (ball.x - (block[i][j].x + 25)) + (ball.y - (block[i][j].y + 25))*(ball.y - (block[i][j].y + 25)) <= 1800) {
					//블록의 위에 부딪혔을 경우
					if (ball.y <= block[i][j].y) {
						side = TOP;
					}
					//블록의 아래에 부딪혔을 경우
					else if (ball.y >= block[i][j].y + BLOCK_HEI_LEN) {
						side = BOTTOM;
					}
					//블록의 왼쪽에 부딪혔을 경우
					else if (ball.x <= block[i][j].x) {
						side = LEFT;
					}
					//블록의 오른쪽에 부딪혔을 경우
					else {
						side = RIGHT;
					}
					//부딪힌 블록의 정보를 기억한다.
					hitx = j; hity = i;
					return BLOCK_HIT;

				}
			}
		}
	}
	//존재하는 볼록이 없을 경우 종료하기 위해 BOTTOM_HIT을 return한다.
	if (curblock_num == 0) {
		return BOTTOM_HIT;
	}
	//공이 천장에 부딪혔을 경우
	if (ball.y <= 25) {
		return TOP_HIT;
	}

	return 0;
}
void ofApp::update() {
	//공을 움직이고 return한 값에 대해 처리한다.
	switch (move_ball()) {
	//공의 움직임을 멈추고 gameover를 1로 set한다.
	case BOTTOM_HIT: {
		gameover = 1;
		ball.x_mov = 0;
		ball.y_mov = 0;
		break;
	}
	//공이 왼쪽벽에 부딪혔을 경우 공의 x좌표 이동정도는 +10이 된다.
	case LEFT_WALL_HIT:
		ball.x_mov = +10;
		break;
	//공이 오른쪽벽에 부딪혔을 경우 공의 x좌표 이동정도는 -10이 된다.
	case RIGHT_WALL_HIT:
		ball.x_mov = -10;
		break;
	//공이 바에 부딪혔을 경우 공의 y좌표는 -10가 된다.
	case BAR_HIT:
		ball.y_mov = -10;
		break;
	//공의 블록에 부딪혔을 경우
	case BLOCK_HIT: {
		//item이 아닌 블록일 경우 바로 지운다.
		if (!block[hity][hitx].item) {
			block[hity][hitx].exist = 0;
		}
		//item인 블록일 경우는 같은 item들의 블록을 찾아 모두 item이 아니게 하고 블록을 다 지운다.
		else {
			nodepointer cur = blocklist[hity][hitx];
			if(block[hity][hitx].item) {
				while (cur) {
					block[cur->y][cur->x].item = 0;
					block[cur->y][cur->x].exist = 0;
					cur = cur->link;
				}
			}
		}
		//부딪힌 블록의 side에 따라 처리한다.
		switch (side) {
		//블록의 왼쪽에 부딪혔을 때 공의 x좌표 이동정도는 -10이 된다.
		case LEFT:
			ball.x_mov = -10;
			break;
		//블록의 오른쪽에 부딪혔을 때 공의 x좌표 이동정도는 +10 된다.
		case RIGHT:
			ball.x_mov = +10;
			break;
		//블록의 위쪽에 부딪혔을 때 공의 y좌표 이동정도는 -10이 된다.
		case TOP:
			ball.y_mov = -10;
			break;
		//블록의 아래쪽에 부딪혔을 때 공의 x좌표 이동정도는 +10 된다.
		case BOTTOM:
			ball.y_mov = +10;
			break;
		}

		break;
	}
	//천장에 부딪혔을 경우 공의 y좌표 이동정도는 +10이 된다.
	case TOP_HIT:
		ball.y_mov = +10;
		break;
	}
}


//--------------------------------------------------------------
void ofApp::draw() {
		//배경을 검은색으로 한다.
		ofBackground(0);
		//색깔을 회색으로 한다.
		ofSetColor(100);
		//선의 굵기를 5로한다.
		ofSetLineWidth(5);

		//playbox는 채우지 않고 그린다.
		ofNoFill();
		ofSetLineWidth(5);
		ofDrawRectangle(play_box);
		//바는 채워서 그린다.
		ofFill();
		ofDrawRectangle(bar);
		//블록을 그린다.
		for (int i = 0; i < BLOCK_HEI_NUM; i++) {
			for (int j = 0; j < BLOCK_WID_NUM; j++) {
				//블록이 존재할 때
				if (block[i][j].exist) {
					//블록의 종류에 따라 그린다.
					switch (block[i][j].item) {
					case BLUE:
						//파란색 item일 경우 파란색으로 그린다.
						ofSetColor(100, 0, 0);
						ofDrawRectangle(block[i][j]);
						//테두리를 그린다.
						ofNoFill();
						ofSetColor(255);
						ofDrawRectangle(block[i][j]);
						ofFill();
						break;
					case RED:
						//빨간색 item일 경우 빨간색으로 그린다.
						ofSetColor(0, 0, 100);
						ofDrawRectangle(block[i][j]);
						//테두리를 그린다.
						ofNoFill();
						ofSetColor(255);
						ofDrawRectangle(block[i][j]);
						ofFill();
						break;
					case 0:
						//item이 아닐 경우 회색으로 그린다.
						ofSetColor(100);
						ofDrawRectangle(block[i][j]);
						//테두리를 그린다.
						ofNoFill();
						ofSetColor(255);
						ofDrawRectangle(block[i][j]);
						ofFill();
						break;
					}
				}
			}
		}
		//공을 그린다.
		ofSetColor(200);
		ofDrawCircle(ball.x, ball.y, ball.r);
	
	//gameover가 1로 set됐을 경우 메시지를 출력하는데 공의 오른쪽에 출력한다.
	if (gameover) {
		//바닥에 닿았을 경우
		if (curblock_num) {
			ofSetColor(100);
			verdana.drawString("gameover\n", ball.x + 20, ball.y);
		}
		//블록을 다 부셨을 경우
		else {
			ofSetColor(100);
			verdana.drawString("game complete\n", ball.x + 20, ball.y);
		}
	}
} 



//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	//Esc를 누를 경우 종료한다.
	if(key == VK_ESCAPE) {
		freeMemory();
		ofExit();
	}
	//오른쪽 방향키를 눌렀을 때 오른쪽으로 이동한다.
	if (key == OF_KEY_RIGHT) {
		//오른쪽 벽에 닿았을 경우는 더 이상 이동할 수 없다.
		bar.x += 30;
		if (bar.x > WIDTH + 10 - 100) {
			bar.x = WIDTH + 10 - 100;
		}
	}
	//왼쪽 방향키를 눌렀을 때 왼쪽으로 이동한다.
	if (key == OF_KEY_LEFT) {
		//왼쪽 벽에 닿았을 경우 더 이상 이동할 수 없다.
		bar.x -= 30;
		if (bar.x <= 15)
			bar.x = 15;
	}
} 

void ofApp::freeMemory() {
	//동적할당한 메모리를 해제한다.
	nodepointer cur;
	nodepointer temp;
	for (int i = 0; i < BLOCK_HEI_NUM; i++) {
		for (int j = 0; j < BLOCK_WID_NUM; j++) {
			if (blocklist[i][j]) {
				cur = blocklist[i][j];
				while (cur) {
					temp = cur;
					cur = cur->link;
					free(temp);
				}
			}
		}
	}
}
