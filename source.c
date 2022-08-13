#include <stdio.h>
#include <windows.h>
#include <math.h>

#define MAX 40
	//	y  x
int map[5][7] = {
	{0, 0, 0, 1, 0, 0, 0},
	{0, 1, 0, 1, 0, 1, 0},
	{0, 1, 0, 1, 0, 1, 0}, 
	{0, 1, 0, 1, 0, 1, 0},
	{0, 1, 0, 0, 0, 1, 0},
};

typedef struct Node {
	struct Node* pNode; // parent Node
	int g, h, f = 999;
	int vector2[2] = {-1, -1}; // (x, y)
	bool isNull = true, isStart = false, closing = false;
} Node;

typedef struct Path {
	int x, y;
} Path;

void initNode(Node node[MAX]) {	
	Node init;
	for(int i = 0; i < MAX; i++) {
		node[i] = init;
	}
}


Node OpenList[MAX];

Node CurNode;
int oCount, cCount = 0;

Node End;
Node Start;

int dX[4] = {1, -1, 0, 0};
int dY[4] = {0, 0, -1, 1};

double getF(Node node) {
	return node.g + node.h;
} 

int getH(Node node, Node end) {
	//2 * (abs(now.first - goal.first) + abs(now.second - goal.second));
	return 10 * (abs(node.vector2[0] - end.vector2[0]) + abs(node.vector2[1] - end.vector2[1])); // 가중치 휴리스틱 
//	return sqrt(
//			pow(node.vector2[0] - end.vector2[0], 2)
//		 + 
//			pow(node.vector2[1] - end.vector2[1], 2)
//		);
}

bool isOpenList(Node node) {
	for(int i = 0; i < MAX; i++) {
		if( OpenList[i].vector2[0] == node.vector2[0] && OpenList[i].vector2[1] == node.vector2[1] )
			return true;
	}
	return false;
}

void addOpenList(Node node, Node *pNode)  {
	if( node.vector2[0] > 6 || node.vector2[1] > 4 || node.vector2[0] < 0 || node.vector2[1] < 0 ) {
		printf("맵의 범위를 넘어선 할당이 탐색됨\n");
		return;
	}
	printf("들어감 : (%d, %d)\n", node.vector2[0], node.vector2[1]);
	
	if( map[node.vector2[1]][node.vector2[0]] == 1) // 벽이면 할당 안함 
		return;
		
		
	if( node.closing )
		return;
		
	if( isOpenList(node) )
		return;
		
	node.pNode = pNode;	
	if( node.isStart) {
		node.g = 0;
		node.h = getH(node, End);
		node.f = getF(node) + 1;
	}else {
		node.g = node.pNode->g + 1;
		node.h = getH(node, End);
		node.f = getF(node);
		
	}
	
	OpenList[oCount++] = node;
		
}

void addClose(Node node) {
	for( int i = 0; i < MAX; i++) {	
		if( OpenList[i].vector2[0] == node.vector2[0] && OpenList[i].vector2[1] == node.vector2[1] ){
			printf("닫음\n");
			OpenList[i].closing = true;
			break;
		}
	}
}

void printMap() {
	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 7; j++) {
			if( map[i][j] == 0 )
				printf("□");
			else if( map[i][j] == 1) {
				printf("■");
			}else {
				printf("* ");
			}
		}
		printf("\n");
	}
}

void deleteList(Node node) {
	for(int i = 0; i < MAX; i++) {
		if( OpenList[i].vector2[0] == node.vector2[0] && OpenList[i].vector2[1] == node.vector2[1] ){
			Node a;
			a.closing = true;
			OpenList[i] = a;
		}
	}
}

void pathFinding(Node Start, Node end) {
	Path path[MAX]; 
	
	addOpenList(Start, &Start);

	while( OpenList) {
		Sleep(1000);
		for(int i =0; i < MAX; i++) {
			if(OpenList[i].vector2[0] != -1 && !OpenList[i].closing ) {
				CurNode = OpenList[i];
				CurNode.pNode = OpenList[i].pNode;
				break;
			}
		}

		system("cls");
		printf("Start : (%d, %d)\n", Start.vector2[0], Start.vector2[1]);
		printf("End : (%d, %d)\n", End.vector2[0], End.vector2[1]);
		
		for(int i = 0; i < MAX; i++) {
			//printf("[%d] [[%d]] (%d, %d) : %d < %d => %d <= %d\n", i, OpenList[i].closing, OpenList[i].vector2[0], OpenList[i].vector2[1], OpenList[i].h, CurNode.h, OpenList[i].f, CurNode.f);
			if( OpenList[i].closing)
				continue;
		
			if( OpenList[i].f <= CurNode.f && OpenList[i].g < CurNode.g ){
				CurNode = OpenList[i];
				CurNode.pNode = OpenList[i].pNode;
			} 
			
		}
		Node CurpNode;
		CurpNode = *CurNode.pNode;
		
		printf("pNode : (%d, %d)\n", CurpNode.vector2[0], CurpNode.vector2[1]);
		printf("Node : (%d, %d)\n", CurNode.vector2[0], CurNode.vector2[1]);
		map[CurNode.vector2[1]][CurNode.vector2[0]] = 2;
		printMap();
		
		addClose(CurNode);
		// 도착지점에 도착 했으면 실행 하는 구문 
		if( CurNode.vector2[0] == end.vector2[0] && CurNode.vector2[1] == end.vector2[1] ){
			//break;
			 //백트레킹 부분 나중에 fix 하기 
//			int pCount = 0;
//			Node *current = &CurNode;
//			printf("[ ");
//			while( !current->isStart ) {
//			 	path[pCount].x = current->vector2[0];
//			 	path[pCount].y = current->vector2[1];
//			 	printf("(%d, %d), ", current->vector2[0], current->vector2[1]);
//			 	current = current->pNode;
//			 	pCount += 1;
//			}
//			printf(" ]\n");
			break;
		}
		
		Node dNode;
		for(int i = 0; i < 4; i++) {
			dNode.vector2[0] = CurNode.vector2[0] + dX[i];
			dNode.vector2[1] = CurNode.vector2[1] + dY[i];
			addOpenList(dNode, &CurpNode);
		}
	}
	
}


Node makeNode(int x, int y, double g) {
	Node node;
	node.vector2[0] = x; // x
	node.vector2[1] = y; // y
	node.isNull = false;
	node.g = g;
	return node;
}


int main() {

	initNode(OpenList);
	End = makeNode(6, 3, -1);
	Start = makeNode(0, 2, -1);
	Start.isStart = true;	
	Start.h = getH(Start, End);
	Start.f = getF(Start);
	
	pathFinding(Start, End);
	

	return 0;
}
