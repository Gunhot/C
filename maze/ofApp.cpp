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

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check
	hPopup = menu->AddPopupMenu(hMenu, "Toolbar"); 
	bShowInfo = true;  
	menu->AddPopupItem(hPopup, "Show DFS", false, false); 
	bTopmost = false;
	menu->AddPopupItem(hPopup, "Show BFS");
	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		freeMemory();
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		doTopmost(bChecked);
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	ofBackground(255);
	ofSetColor(0);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	
	//천장
	for (int i = 0; i < maze_col; i++) {
		ofDrawLine( 50 * i, 3,  50 * (i + 1), 3);
	}
	//출력
	for (int i = 0; i < maze_row; i++) {
		ofDrawLine(3, 50*(i),3 ,50*(i+1) );
		for (int j = 0; j < maze_col; j++) {
			if (maze[i][j].rightflag == 1) {
				ofDrawLine(50 * (j + 1), 50 * (i), 50 * (j + 1), 50 * (i + 1));
			}
			if (maze[i][j].downflag == 1)
				ofDrawLine(50 * (j), 50 * (i + 1), 50 * (j + 1), 50 * (i +1));
		}
	}
	


	if (isdfs)
	{
		ofSetColor(0, 0, 255);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if (isbfs)
	{
		ofSetColor(0, 0, 255);
		ofSetLineWidth(5);
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;
			
			
			// TO DO
			//fileName is not const char* -> cannot used in fopen
			//make const char* filename and copy fileName to filename.
			char* filename = new char[fileName.size()+ 1];
			copy(fileName.begin(), fileName.end(), filename);
			filename[fileName.size()] = '\0';
			FILE * in = fopen(filename, "r");
			if (in == NULL) {
				printf("cannot find %s\n",filename);
				return false;
			}
			else {
				//우선 미로의 너비와 높이를 구하고 미로의 정보를 저장
				//너비 저장
				while (fgetc(in) != '\n')WIDTH++;
				HEIGHT++;
				//높이 저장
				char c;
				while ((c=fgetc(in)) != EOF) {
					if(c == '\n')HEIGHT++;
				}
				//메모리 할당
				input = (char**)malloc(HEIGHT*sizeof(*input));
				for (int i = 0; i < HEIGHT; i++) {
					input[i] = (char*)malloc(WIDTH * sizeof(char));
				}
				in = fopen(filename, "r");
				//input 저장
				for (int i = 0; i < HEIGHT; i++) {
					for (int j = 0; j < WIDTH; j++) {
						input[i][j] = fgetc(in);
					}
					fgetc(in);
				}
				maze_col = (WIDTH - 1) / 2;
				maze_row = (HEIGHT - 1) / 2;
				//메모리할당
				maze = (room**)malloc(maze_row * sizeof(room*));
				for (int i = 0; i < maze_row; i++)maze[i] = (room*)malloc(maze_col * sizeof(room));
				//미로의 정보저장
				int x = 0;
				int y = 0;
				for (int i = 1; i < HEIGHT; i = i + 2) {
					for (int j = 1; j < WIDTH; j = j + 2) {
						if (input[i][j + 1] == '|')maze[y][x].rightflag = 1;
						else maze[y][x].rightflag = 0;
						if (input[i + 1][j] == '-')maze[y][x].downflag = 1;
						else maze[y][x].downflag = 0;
						maze[y][x].set = ++idx;
						x++;
						if (x == maze_col) {
							x = 0; y++;
						}
					}
				}			
			}
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			adjlist = (nodepointer*)malloc((idx + 1) * sizeof(nodepointer));
			for (int i = 1; i <= idx; i++) {
				adjlist[i] = (nodepointer)malloc(sizeof(node));
			}
			for (int i = 1; i <= idx; i++) {
				adjlist[i]->data = i;
				adjlist[i]->link = NULL;
			}

			for (int i = 0; i < maze_row; i++) {
				for (int j = 0; j < maze_col; j++) {
					if (j != maze_col - 1) {
						if (!maze[i][j].rightflag) {
							add_list(adjlist[maze[i][j].set], adjlist[maze[i][j + 1].set]);
						}
					}
					if (i != maze_row - 1) {
						if (!maze[i][j].downflag) {
							add_list(adjlist[maze[i][j].set], adjlist[maze[i + 1][j].set]);
						};
					}
				}
			}
			
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
bool ofApp::add_list(nodepointer a, nodepointer b) {
	nodepointer temp_a;
	temp_a = (nodepointer)malloc(sizeof(node));
	temp_a->data = b->data; temp_a->link = NULL;
;
	nodepointer temp_b;
	temp_b = (nodepointer)malloc(sizeof(node));
	temp_b->data = a->data; temp_b->link = NULL;
	
	nodepointer curr_a = a;
	for (; curr_a->link != NULL; curr_a = curr_a->link);
	curr_a->link = temp_a;

	nodepointer curr_b = b;
	for (; curr_b->link != NULL; curr_b = curr_b->link);
	curr_b->link = temp_b;
	
	return 1;
}
void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수
	int idx = maze_col * maze_row;
	for (int i = 1; i <= idx; i++) {
		nodepointer cur = adjlist[i];
		nodepointer pre = NULL;
		while (cur) {
			pre = cur;
			cur = cur->link;
			free(pre);
		}
		free(pre);
	}
	for (int i = 1; i <= idx; i++) {
		free(adjlist[i]);
	}
	for (int i = 0; i < maze_row; i++) {
		free(maze[i]);
	}
	free(maze);
	for (int i = 0; i < HEIGHT; i++) {
		free(input[i]);
	}
	free(input);
	free(dfs_path);
	free(bfs_path);

	while (top)pop();
	while (the_top)the_pop();
	while (front)dequeue();
}
void ofApp::push(int item) {
	nodepointer temp = (nodepointer)malloc(sizeof(struct node));
	temp->data = item;
	temp->link = top;
	top = temp;
}
int ofApp::pop() {
	if (top == NULL) {
		printf("stack is empty!\n");
		return -1;
	}
	else {
		nodepointer del = top;
		int item = top->data;
		top = top->link;
		free(del);
		return item;
	}
}
void ofApp::the_push(int item) {
	nodepointer temp = (nodepointer)malloc(sizeof(struct node));
	temp->data = item;
	temp->link = the_top;
	the_top = temp;
}
int ofApp::the_pop() {
	if (the_top == NULL) {
		printf("stack is empty!\n");
		return -1;
	}
	else {
		nodepointer del = the_top;
		int item = the_top->data;
		the_top = the_top->link;
		free(del);
		return item;
	}
}
bool ofApp::DFS()//DFS탐색을 하는 함수
{
	while (top)pop();
	while (the_top)the_pop();
	while (front)dequeue();
	
	isbfs = 0;
	dfs_L = 0;
	if (visited) free(visited);
	visited = (int*)malloc((maze_col * maze_row + 1) * sizeof(int));
	for (int i = 1; i <= maze_col * maze_row; i++)visited[i] = 0;
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	//다훑네
	//자료구조
	//컴실
	all_L = 0;
	int cur_vertex;
	push(1);
	while (top) {
		cur_vertex = pop();
		//만약 현재 고려중인 vertex를 안갔더라면
		if (!visited[cur_vertex]) {
			//방문 후 1로 set
			the_push(cur_vertex);
			all_L++;
			visited[cur_vertex] = 1;
			if(cur_vertex == maze_row * maze_col)break;
			//갈 수 있는 곳 고려
			for (nodepointer adjvertex = adjlist[cur_vertex]->link; adjvertex; adjvertex = adjvertex->link) {
				if(visited[adjvertex->data] == 0)
				push(adjvertex->data);
			}
		}
	}
	if (all) {
		free(all);
	}
	all = (int*)malloc(all_L * sizeof(int));
	int n = all_L;
	//empty stack
	while (top)pop();
	//Destination vertex
	int path = maze_row * maze_col;
	while (the_top) {
		int check = 0;
		//인접한 노드일 경우 스택에 넣는다
		//20은 20의 인접이므로 check = 1로 변경됨
		for (nodepointer cur = adjlist[path]; cur; cur = cur->link) {
			if (cur->data == the_top->data)check = 1;
		}
		//check가 1일 경우 경로 저장
		if (check == 1) {
			path = the_pop();
			push(path);
			all[--n] = path;
			dfs_L++;
		}
		else {
			all[--n] = the_pop();
		}

	}
	dfs_path = (int*)malloc(dfs_L * sizeof(int));
	for (int i = 0; i < dfs_L; i++) {
		dfs_path[i] = pop();
	}
	free(visited);
	isdfs = 1;
	return 1;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	
	int pre_x, pre_y, cur_x, cur_y;
	for (int i = 0; i < dfs_L - 1; i++) {
		pre_x = (dfs_path[i]-1) % maze_col;
		pre_y = (dfs_path[i]-1) / maze_col;
		cur_x = (dfs_path[i + 1]-1) % maze_col;
		cur_y = (dfs_path[i + 1]-1) / maze_col;
		ofDrawLine(50 * (pre_x) + 25, 50 * (pre_y)+25, 50 * (cur_x)+25, 50 * (cur_y)+25);
	}
	ofSetLineWidth(2);
	for (int i = 0; i < all_L; i++) {
		for (int j = 0; j <= all_L; j++) {
			for (nodepointer cur = adjlist[all[i]]->link; cur; cur = cur->link) {
				if (cur->data == all[j]) {
					pre_x = (all[i] - 1) % maze_col;
					pre_y = (all[i] - 1) / maze_col;
					cur_x = (all[j] - 1) % maze_col;
					cur_y = (all[j] - 1) / maze_col;
					ofDrawLine(50 * (pre_x)+25, 50 * (pre_y)+25, 50 * (cur_x)+25, 50 * (cur_y)+25);
				}
			}
		}
	}

}
void ofApp::enqueue(int item) {
	
	nodepointer temp = (nodepointer)malloc(sizeof(struct node));
	temp->data = item;
	temp->link = NULL;
	if (front) {
		rear->link = temp;
	}
	else {
		front = temp;
	}
	rear = temp;
}
int ofApp::dequeue() {
	
		nodepointer del = front;
		int item = del->data;
		front = front->link;
		free(del);
		return item;
}
bool ofApp::BFS() {
	
		isdfs = 0;
		visited = (int*)malloc((maze_col * maze_row + 1) * sizeof(int));
		for (int i = 1; i <= maze_col * maze_row; i++)visited[i] = 0;
		while (top)pop();
		while (the_top)the_pop();
		while (front)dequeue();
		//TO DO
		//BFS탐색을 하는 함수 ( 3주차)
		//다훑네
		enqueue(1);
		the_push(1);
		int cur_vertex;
		all_L = 0;
		while (front) {
			cur_vertex = dequeue();
			//만약 현재 고려중인 vertex를 안갔더라면
			if (!visited[cur_vertex]) {
				//방문 후 1로 set
				the_push(cur_vertex);
				all_L++;
				visited[cur_vertex] = 1;
				if (cur_vertex == maze_row * maze_col)break;
				//갈 수 있는 곳 고려
				for (nodepointer adjvertex = adjlist[cur_vertex]->link; adjvertex; adjvertex = adjvertex->link) {
					if (visited[adjvertex->data] == 0)
						enqueue(adjvertex->data);
				}
			}
		}
		if (all)free(all);
		all = (int*)malloc(all_L * sizeof(int));
		int n = all_L;
		//Destination vertex
		bfs_L = 0;
		int path = maze_row * maze_col;
		while (the_top) {
			int check = 0;
			//인접한 노드일 경우 스택에 넣는다
			//20은 20의 인접이므로 check = 1로 변경됨
			for (nodepointer cur = adjlist[path]; cur; cur = cur->link) {
				if (cur->data == the_top->data)check = 1;
			}
			//check가 1일 경우 경로 저장
			if (check == 1) {
				path = the_pop();
				push(path);
				all[--n] = path;
				bfs_L++;
			}
			else {
				all[--n] = the_pop();
			}
		}
		bfs_path = (int*)malloc(bfs_L * sizeof(int));
		for (int i = 0; i < bfs_L; i++) {
			bfs_path[i] = pop();
		}
		free(visited);
		isbfs = 1;
		
	return 1;
}
void ofApp::bfsdraw() {
	
		int pre_x, pre_y, cur_x, cur_y;
		for (int i = 0; i < bfs_L - 1; i++) {
			pre_x = (bfs_path[i] - 1) % maze_col;
			pre_y = (bfs_path[i] - 1) / maze_col;
			cur_x = (bfs_path[i + 1] - 1) % maze_col;
			cur_y = (bfs_path[i + 1] - 1) / maze_col;
			ofDrawLine(50 * (pre_x)+25, 50 * (pre_y)+25, 50 * (cur_x)+25, 50 * (cur_y)+25);
		}
		ofSetLineWidth(2);
		for (int i = 0; i < all_L; i++) {
			for (int j = 0; j <= all_L; j++) {
				for (nodepointer cur = adjlist[all[i]]->link; cur; cur = cur->link) {
					if (cur->data == all[j]) {
							pre_x = (all[i] - 1) % maze_col;
							pre_y = (all[i] - 1) / maze_col;
							cur_x = (all[j] - 1) % maze_col;
							cur_y = (all[j] - 1) / maze_col;
							ofDrawLine(50 * (pre_x)+25, 50 * (pre_y)+25, 50 * (cur_x)+25, 50 * (cur_y)+25);
					}
				}
			}
		}
		
}

