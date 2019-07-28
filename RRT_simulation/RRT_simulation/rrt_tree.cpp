//#include "rrt_tree.h"
//
//pair<int, int> random_list[3840];		//random 좌표
//
//float f_distance(Point2f p1, Point2f p2) {
//	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
//}
//
//Tree::Tree(Mat dstImage, float x, float y) {
//
//	/* 최초의 노드 생성 */
//	root_node = new node_;
//	root_node->point.x = x;
//	root_node->point.y = y;
//	root_node->node_parent = nullptr;
//
//	for (int i = 0; i < CHILD_SIZE; i++) {
//		root_node->node_child[i] = nullptr;
//	}
//
//
//	circle(dstImage, root_node->point, 5, Scalar(0, 0, 255), 5);
//
//
//	//GoalNode 선정
//	Goal_node = new node_;
//	Goal_node->point.x = MAP_SIZE_X - 200;
//
//	Goal_node->point.y = MAP_SIZE_Y - 100;
//
//	circle(dstImage, Goal_node->point, 5, Scalar(255, 0, 0), 5);
//
//
//	///////////////////////////////////////////////////////////////////////////////////////////
//	/*
//			랜덤으로 쓸 좌표들 생성(맵의 가장자리)
//	*/
//	///////////////////////////////////////////////////////////////////////////////////////////
//
//	MakeRandomList();
//
//	///////////////////////////////////////////////////////////////////////////////////////////
//	/*
//			그 외 변수들 초기화
//	*/
//	///////////////////////////////////////////////////////////////////////////////////////////
//	g_distance = 2000000000;
//
//	pNearest_node = nullptr;
//
//}
//
//void Tree::MakeRandomList() {
//	int n = 1280, i = 0, j = 0;
//	//상단 랜덤좌표 생성
//	while (n--) {
//		random_list[i] = make_pair(j, 0);
//		i++;
//		j++;
//	}
//
//	//i = 1280
//	//하단 랜덤좌표 생성
//	n = 1280;
//	j = 0;
//	while (n--) {
//		random_list[i] = make_pair(j, 640);
//		j++;
//		i++;
//	}
//
//	//i=2560
//	//좌측 랜덤좌표 생성
//	n = 640;
//	j = 0;
//	while (n--) {
//		random_list[i] = make_pair(0, j);
//		i++;
//		j++;
//	}
//
//	//i = 3200
//	//우측 랜덤좌표 생성
//	n = 640;
//	j = 0;
//	while (n--) {
//		random_list[i] = make_pair(1280, j);
//		i++;
//		j++;
//	}
//}
//
////랜덤노드 생성
//node_* Tree::RandomNode(Mat dstImage) {
//
//	node_* node_random = new node_;
//
//	/*int k = rand() % 3840;
//	node_random->point.x = random_list[k].first;
//	node_random->point.y = random_list[k].second;*/
//
//	/* 랜덤노드 좌표 할당 및 시각화 */
//	node_random->point.x = rand() % MAP_SIZE_X;
//	node_random->point.y = rand() % MAP_SIZE_Y;
//
//
//	//circle(dstImage, node_random->point, 2, Scalar(255, 0, 0), 2);
//
//	return node_random;
//}
//
//
//void Tree::NearestNode(node_ *node_random, node_* node_current, float pre_distance, int d_flag) {
//
//	//base case 1. d_flag번 연속 멀어지면 종료
//	float current_distance = f_distance(node_current->point, node_random->point);
//
//	if (pre_distance < current_distance) {	//지금 가리키는 노드가 더 멀어지면 실행
//		d_flag++;
//		if (d_flag == 1) {
//			return;	//d_flag == N 에 따라 탐색을 얼마나 심도있게 하는지가 결정된다.
//		}
//
//	}
//	else {
//		d_flag = 0;
//		if (g_distance > current_distance) {
//			g_distance = current_distance;				//가까워 진거니까 거리기록
//			if (node_current->node_child[1] == nullptr)	//child에 여유공간있으면 여기서 가지 뻗어나갈수 있다.
//				pNearest_node = node_current;
//		}
//		
//		
//		
//
//	}
//
//
//
//	//general case
//	for (int i = 0; i < CHILD_SIZE; i++) {
//		if (node_current->node_child[i] != nullptr) {
//
//			NearestNode(node_random, node_current->node_child[i], current_distance, d_flag);
//
//		}
//	}
//
//	return;
//}
//
//
//
////new노드 생성
//void Tree::NewNode(Mat dstImage, node_* node_random, node_* node_nearest) {
//
//	node_* node_new = new node_;
//
//	// node_new 의 child 초기화작업
//	for (int i = 0; i < CHILD_SIZE; i++) {
//		node_new->node_child[i] = nullptr;
//	}
//
//	// node_new 좌표설정작업
//	int d = f_distance(node_new->point, node_random->point);
//	int line_dist = 25;
//	//랜덤노드와 원점사이 거리가 20이상이면 중간에 새 노드를 생성
//	if (d > line_dist) {
//		//y = ax + b
//		float a = (node_random->point.y - node_nearest->point.y) / (node_random->point.x - node_nearest->point.x);
//		float b = a * node_nearest->point.x*(-1) + node_nearest->point.y;
//
//		//new_node 할당
//		if (node_nearest->point.x < node_random->point.x) node_new->point.x = node_nearest->point.x + line_dist * cos(atan(a));
//		else node_new->point.x = node_nearest->point.x - line_dist * cos(atan(a));
//
//		node_new->point.y = a * node_new->point.x + b;
//
//
//	}
//
//	//20이하이면 랜덤노드자리에 바로 새 노드를 생성
//	else {
//		node_new->point.x = node_random->point.x;
//		node_new->point.y = node_random->point.y;
//	}
//
//	circle(dstImage, node_new->point, 2, Scalar(0, 0, 255), 2);
//
//	// node_new 와 부모노드를 연결
//	for (int i = 0; i < CHILD_SIZE; i++) {
//		if (pNearest_node->node_child[i] == nullptr) {
//			pNearest_node->node_child[i] = node_new;
//			node_new->node_parent = pNearest_node;
//			//직선이어주기 함수 추가
//			line(dstImage, pNearest_node->point, node_new->point, Scalar(255, 0, 0), 1);
//			break;
//		}
//
//	}
//	pNearest_node = node_new;
//
//	//que.push(*node_new);
//	st.push(*node_new);
//
//	g_distance = 2000000000;	//이거 초기화 해줘야함
//
//	return;
//}
//
//
//
//bool Tree::isGoal() {
//	//pNearest_node가 goal 에서 거리가 100 미만이면 목표지점에 도달했다고 본다.
//
//	int dist = f_distance(pNearest_node->point, Goal_node->point);	//새로 만들어진 노드와 목표지점과의 거리 계산
//	if (dist < 30) return true;
//
//	return false;
//}
//
