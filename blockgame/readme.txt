OpenFramework 내에서 작동한다.
examples 폴더에 들어간다. -> templates 폴더에 들어간다. -> emptyExample에 들어간다.
src폴더에 ofApp.cpp ofApp.h를 복사해서 덮어쓴다.

솔루션을 연다.
ofRectangle.h에서 class ofRectangle에 int item과 int exist를 선언한다.
->item은 블록의 item종류를 나타내고 exist는 블록의 존재 유무를 나타낸다.
build 후 run한다.

입력할 수 있는 키는 세 가지다.
왼쪽 방향키 : 바를 왼쪽으로 움직인다.
오른쪽 방향키 : 바를 오른쪽으로 움직인다.
Esc : 게임을 종료한다.

RULE 1 : 공은 game이 끝나지 않는 이상 계속 움직인다.
RULE 2 : 공이 벽이나 블록, 바에 부딪힐 경우 방향이 바뀐다.
RULE 3 : 공이 블록에 부딪힐 경우 블록이 없어진다.
RULE 4 : 공이 색깔이 있는 블록(item)에 부딪힐 경우 같은 색깔의 블록들이 같이 없어진다.
RULE 5 : 공이 바닥에 닿으면 game over
RULE 6 : 방향키를 사용해서 바를 움직여 공이 바닥에 닿지 않게 한다.
RULE 7 : 모든 블록을 부술 경우 game complete
