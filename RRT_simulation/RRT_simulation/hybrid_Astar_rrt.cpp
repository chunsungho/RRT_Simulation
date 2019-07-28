#include "hybrid_Astar_rrt.h"

pair<int, int> h_random_list[3840];

float f_distance(Point2f p1, Point2f p2) {
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

Hybrid_Tree::Hybrid_Tree(Mat dstImage, float start_x, float start_y, float Goal_x, float Goal_y) {

	/* 최초의 노드 생성 */
	root_node = new h_node;
	root_node->point.x = start_x;
	root_node->point.y = start_y;
	root_node->node_parent = nullptr;


	/* root 노드의 자식 관련 */
	//root노드의 자식 초기화
	for (int i = 0; i < h_CHILD_SIZE; i++) {
		root_node->node_child[i] = nullptr;
	}

	circle(dstImage, root_node->point, 5, Scalar(0, 0, 255), 5);


	//GoalNode 선정
	Goal_node = new h_node;
	Goal_node->point.x = Goal_x;

	Goal_node->point.y = Goal_y;

	circle(dstImage, Goal_node->point, 5, Scalar(0, 0, 255), 5);

	//첫 3개의 child 생성(각도재는 문제때문에 이렇게 함)
	First3Child(dstImage, 0 * PI / 180);

	//생성자에서 맵 생성.
	map.x = 0;
	map.y = 0;
	map.width = MAP_SIZE_X;
	map.height = MAP_SIZE_Y;


	//그외 변수 초기화
	arrive_flag = 0;
	g_distance = 2000000000;
	path_distance = 0;

	pNearest_node = root_node;
}


h_node* Hybrid_Tree::RandomNode(Mat dstImage) {

	//srand(time(NULL));

	node_random = new h_node;

	/* 랜덤노드 좌표 할당 및 시각화 */
	node_random->point.x = rand() % MAP_SIZE_X;
	node_random->point.y = rand() % MAP_SIZE_Y;

	return node_random;

}

void Hybrid_Tree::NearestNode(h_node *h_NodeRandom, h_node* h_NodeCurrent, float pre_distance, int d_flag) {

	//base case 1. d_flag번 연속 멀어지면 종료
	float current_distance = f_distance(h_NodeCurrent->point, h_NodeRandom->point);

	//지금 가리키는 노드가 더 멀어지면 이쪽은 탐색종료
	if (pre_distance < current_distance) {
		d_flag++;
		if (d_flag == 1) {
			return;	//d_flag == N 에 따라 탐색을 얼마나 심도있게 하는지가 결정된다.
		}

	}

	//노드가 더 가까워진거면 실행
	else {
		d_flag = 0;
		if (g_distance > current_distance) {
			g_distance = current_distance;				//가까워 진거니까 거리기록
			pNearest_node = h_NodeCurrent;			//현재 노드를 기록
		}


		if (h_NodeCurrent->node_child[0] != nullptr) {	//자식이 있다면

			for (int i = 0; i < h_CHILD_SIZE; i++) {
				NearestNode(h_NodeRandom, h_NodeCurrent->node_child[i], current_distance, d_flag);
			}

		}


	}

	return;
}

bool Hybrid_Tree::isInMap(h_node* Nearest_node) {
	if (map.contains(Nearest_node->point)) return true;
	else return false;
}


void Hybrid_Tree::NewNode(Mat dstImage, h_node* parent_node) {


	h_node* node_new[3];	//nearest node 에 붙여줄 child 3개 생성

	//child의 좌표생성
	float angle_betweenChild = 50 * PI / 180;
	float angle_brach;	//가지뻗을 노드의 각도

	if (parent_node->node_parent == nullptr) return;
	//가지뻗을 노드의 각도찾기
	if (parent_node->point.x - parent_node->node_parent->point.x == 0) {	//예외처리. 기울기가 무한대인경우 ( 직선이 y축과 평행 )

		if (parent_node->point.y - parent_node->node_parent->point.y > 0) angle_brach = 90 * PI / 180;
		else angle_brach = -90 * PI / 180;
	}
	else {

		angle_brach = atan2(parent_node->point.y - parent_node->node_parent->point.y, parent_node->point.x - parent_node->node_parent->point.x);
	}

	for (int i = 0; i < 3; i++) {
		node_new[i] = new h_node;	//child 생성

		parent_node->node_child[i] = node_new[i];	//nearest node 의 child로 지정
		node_new[i]->node_parent = parent_node;		//child의 부모를 지정

		//child 의 child 를 nullptr 로 지정
		for (int j = 0; j < 3; j++) {
			node_new[i]->node_child[j] = nullptr;
		}

		Point2f tmp1, tmp2, goal_point;

		if (i != 1) {
			tmp1 = translate_point(parent_node->point, angle_brach, 46);
			tmp2 = rotation_point(parent_node->point, tmp1, (1 - i) * 90 * PI / 180);
			goal_point = rotation_point(tmp2, parent_node->point, (1 - i) * angle_betweenChild);

		}
		else {// i == 1
			goal_point = translate_point(parent_node->point, angle_brach, 39);
		}

		node_new[i]->point = goal_point;

		circle(dstImage, node_new[i]->point, 2, Scalar(0, 0, 255), 2);

		line(dstImage, parent_node->point, node_new[i]->point, Scalar(0, 0, 255), 1);


	}

	g_distance = 2000000000;	//이거 초기화 해줘야함
}


void Hybrid_Tree::First3Child(Mat dstImage, float root_angle) {
	h_node* node_new[3];	//nearest node 에 붙여줄 child 3개 생성

	//child의 좌표생성
	float angle_betweenChild = 40 * PI / 180;

	for (int i = 0; i < 3; i++) {
		node_new[i] = new h_node;	//child 생성

		root_node->node_child[i] = node_new[i];	//nearest node 의 child로 지정
		node_new[i]->node_parent = root_node;	//child의 부모를 지정


		//child 의 child 를 nullptr 로 지정
		for (int j = 0; j < 3; j++) {
			node_new[i]->node_child[j] = nullptr;
		}

		Point2f tmp1, tmp2, goal_point;

		if (i != 1) {
			tmp1 = translate_point(root_node->point, root_angle, 46);	//tmp1 = root_node 를 해당 각도,거리만큼 민다.
			tmp2 = rotation_point(root_node->point, tmp1, (1 - i) * 90 * PI / 180);		//tmp2 = root_node 를 중심으로 tmp1을 90도 회전
			goal_point = rotation_point(tmp2, root_node->point, (1 - i) * angle_betweenChild);		//goal_point = tmp2를 중심으로 root_node를 해당 각도만큼 돌린다.

		}
		else {// i == 1
			goal_point = translate_point(root_node->point, root_angle, 32);
		}

		node_new[i]->point = goal_point;

		//circle(dstImage, tmp, 5, Scalar(0, 0, 255), 5);
		circle(dstImage, node_new[i]->point, 2, Scalar(0, 0, 255), 2);
		line(dstImage, root_node->point, node_new[i]->point, Scalar(0, 0, 255), 1);

	}
}


//base_point 대해서 target_point를 얼마만큼 회전
//각도는 라디안으로 넣어줘야함
//각도 양수로 넣으면 시계방향으로 돌아감. (z축이 화면 들어가는 방향)
Point2f rotation_point(Point2f base_point, Point2f target_point, float theta) {
	Point2f goal_point;
	goal_point.x = cos(theta) * (target_point.x - base_point.x)
		- sin(theta)* (target_point.y - base_point.y) + base_point.x;
	goal_point.y = sin(theta) * (target_point.x - base_point.x)
		+ cos(theta) * (target_point.y - base_point.y) + base_point.y;
	return goal_point;
}

//starting_point 를 어떤 기울기의 직선 위에서 얼마만큼 밀것인가.
//angle 은 라디안으로 넣어줘야한다.
Point2f translate_point(Point2f starting_point, float angle, float len) {
	Point2f goal_point;
	goal_point.x = starting_point.x + len * cos(angle);
	goal_point.y = starting_point.y + len * sin(angle);

	return 	goal_point;
}

bool Hybrid_Tree::isGoal() {

	//pNearest_node가 goal 에서 거리가 100 미만이면 목표지점에 도달했다고 본다.
	for (int i = 0; i < 3; i++) {
		int dist = f_distance(pNearest_node->node_child[i]->point, Goal_node->point);	//새로 만들어진 노드와 목표지점과의 거리 계산
		if (dist < 100) {
			node_Goalin = pNearest_node->node_child[i];
			return true;
		}

	}


	return false;
}

//이 함수가 끝나면 pNearest_node 에는 leaf 노드가 들어간 상태가 되어야한다.
void Hybrid_Tree::getLeafNode(h_node* pNearest_node) {
	int random = rand() % 3;
	//노드가 중간가지이면 끝까지 내려가서 실행한다.
	while (pNearest_node->node_child[0] != nullptr) {
		pNearest_node = pNearest_node->node_child[random];
	}
}

void Hybrid_Tree::showPath(Mat dstImage) {
	h_node* node_temp = node_Goalin;
	pathUpdate_flag = 1;
	//path_distance += f_distance(Goal_node->point, node_Goalin->point);

	line(dstImage, Goal_node->point, node_temp->point, Scalar(255, 0, 0), 1);

	while (node_temp) {	//root_node 가 나올때까지 올라간다.
		circle(dstImage, node_temp->point, 2, Scalar(255, 0, 0), 2);
		if (node_temp->node_parent != nullptr) {
			//path_distance += 39;
			line(dstImage, node_temp->node_parent->point, node_temp->point, Scalar(255, 0, 0), 1);
		}
		que.push(node_temp);
		node_temp = node_temp->node_parent;
	}
}

void Hybrid_Tree::checkDistance() {
	h_node* node_temp = node_Goalin;
	path_distance += f_distance(Goal_node->point, node_Goalin->point);
	while (node_temp) {	//root_node 가 나올때까지 올라간다.
		if (node_temp->node_parent != nullptr) {
			path_distance += 39;
		}
		node_temp = node_temp->node_parent;
	}
}

//main 문에서 showPath 를 다시 호출( 원래 경로랑 업데이트된 경로 둘다 파란색이 됨)
//-> que 를 root_node 가 나올떄까지 꺼내면서 빨간색으로 다시 칠한다.

void Hybrid_Tree::updatePath(Mat dstImage) {
	pathUpdate_flag = 0;
	h_node* node_temp;
	node_temp = que.front();

	//line : goal_node와 node_goalin 
	line(dstImage, Goal_node->point, node_temp->point, Scalar(0, 0, 255), 1);

	while (node_temp != root_node) {

		circle(dstImage, node_temp->point, 2, Scalar(0, 0, 255), 2);
		line(dstImage, node_temp->node_parent->point, node_temp->point, Scalar(0, 0, 255), 1);

		que.pop();
		node_temp = que.front();
	}
	que.pop(); //root_node 를 pop

}

//시작점과 끝점을 초점으로하는 타원안에 pNearestNode 가 들어가면 true 반환
bool Hybrid_Tree::IsinEllipse(h_node* start_node, h_node* end_node, float l, h_node* pNearest_node) {
	float x1 = start_node->point.x, y1 = start_node->point.y;
	float x2 = end_node->point.x, y2 = end_node->point.y;
	float a = l / 2;
	float c_sqr = powf((x1 - (x1 + x2) / 2), 2) + powf((y1 - (y1 + y2) / 2), 2); //초점좌표의 제곱
	float b = sqrt(a*a - c_sqr);
	float w = atan2(y2 - y1, x2 - x1);	//라디안으로 나올걸

	float x = pNearest_node->point.x, y = pNearest_node->point.y;

	if (powf((cos(w) * (x - (x1 + x2) / 2) + sin(w) * (y - (y1 + y2) / 2)) / a, 2)
		+ powf((-sin(w) * (x - (x1 + x2) / 2) + cos(w) * (y - (y1 + y2) / 2)) / b, 2) < 1)
		return true;

	return false;
}

void Hybrid_Tree::createMap(Mat dstImage, float start_x, float start_y, float Goal_x, float Goal_y) {
	Point2f start_p, end_p;
	start_p.x = start_x;
	start_p.y = start_y;
	end_p.x = Goal_x;
	end_p.y = Goal_y;

	circle(dstImage, start_p, 5, Scalar(0, 0, 255), 5);
	circle(dstImage, end_p, 5, Scalar(255, 0, 0), 5);

}