//#include "rrt_tree.h"
//
//pair<int, int> random_list[3840];		//random ��ǥ
//
//float f_distance(Point2f p1, Point2f p2) {
//	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
//}
//
//Tree::Tree(Mat dstImage, float x, float y) {
//
//	/* ������ ��� ���� */
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
//	//GoalNode ����
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
//			�������� �� ��ǥ�� ����(���� �����ڸ�)
//	*/
//	///////////////////////////////////////////////////////////////////////////////////////////
//
//	MakeRandomList();
//
//	///////////////////////////////////////////////////////////////////////////////////////////
//	/*
//			�� �� ������ �ʱ�ȭ
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
//	//��� ������ǥ ����
//	while (n--) {
//		random_list[i] = make_pair(j, 0);
//		i++;
//		j++;
//	}
//
//	//i = 1280
//	//�ϴ� ������ǥ ����
//	n = 1280;
//	j = 0;
//	while (n--) {
//		random_list[i] = make_pair(j, 640);
//		j++;
//		i++;
//	}
//
//	//i=2560
//	//���� ������ǥ ����
//	n = 640;
//	j = 0;
//	while (n--) {
//		random_list[i] = make_pair(0, j);
//		i++;
//		j++;
//	}
//
//	//i = 3200
//	//���� ������ǥ ����
//	n = 640;
//	j = 0;
//	while (n--) {
//		random_list[i] = make_pair(1280, j);
//		i++;
//		j++;
//	}
//}
//
////������� ����
//node_* Tree::RandomNode(Mat dstImage) {
//
//	node_* node_random = new node_;
//
//	/*int k = rand() % 3840;
//	node_random->point.x = random_list[k].first;
//	node_random->point.y = random_list[k].second;*/
//
//	/* ������� ��ǥ �Ҵ� �� �ð�ȭ */
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
//	//base case 1. d_flag�� ���� �־����� ����
//	float current_distance = f_distance(node_current->point, node_random->point);
//
//	if (pre_distance < current_distance) {	//���� ����Ű�� ��尡 �� �־����� ����
//		d_flag++;
//		if (d_flag == 1) {
//			return;	//d_flag == N �� ���� Ž���� �󸶳� �ɵ��ְ� �ϴ����� �����ȴ�.
//		}
//
//	}
//	else {
//		d_flag = 0;
//		if (g_distance > current_distance) {
//			g_distance = current_distance;				//����� ���Ŵϱ� �Ÿ����
//			if (node_current->node_child[1] == nullptr)	//child�� �������������� ���⼭ ���� ������� �ִ�.
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
////new��� ����
//void Tree::NewNode(Mat dstImage, node_* node_random, node_* node_nearest) {
//
//	node_* node_new = new node_;
//
//	// node_new �� child �ʱ�ȭ�۾�
//	for (int i = 0; i < CHILD_SIZE; i++) {
//		node_new->node_child[i] = nullptr;
//	}
//
//	// node_new ��ǥ�����۾�
//	int d = f_distance(node_new->point, node_random->point);
//	int line_dist = 25;
//	//�������� �������� �Ÿ��� 20�̻��̸� �߰��� �� ��带 ����
//	if (d > line_dist) {
//		//y = ax + b
//		float a = (node_random->point.y - node_nearest->point.y) / (node_random->point.x - node_nearest->point.x);
//		float b = a * node_nearest->point.x*(-1) + node_nearest->point.y;
//
//		//new_node �Ҵ�
//		if (node_nearest->point.x < node_random->point.x) node_new->point.x = node_nearest->point.x + line_dist * cos(atan(a));
//		else node_new->point.x = node_nearest->point.x - line_dist * cos(atan(a));
//
//		node_new->point.y = a * node_new->point.x + b;
//
//
//	}
//
//	//20�����̸� ��������ڸ��� �ٷ� �� ��带 ����
//	else {
//		node_new->point.x = node_random->point.x;
//		node_new->point.y = node_random->point.y;
//	}
//
//	circle(dstImage, node_new->point, 2, Scalar(0, 0, 255), 2);
//
//	// node_new �� �θ��带 ����
//	for (int i = 0; i < CHILD_SIZE; i++) {
//		if (pNearest_node->node_child[i] == nullptr) {
//			pNearest_node->node_child[i] = node_new;
//			node_new->node_parent = pNearest_node;
//			//�����̾��ֱ� �Լ� �߰�
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
//	g_distance = 2000000000;	//�̰� �ʱ�ȭ �������
//
//	return;
//}
//
//
//
//bool Tree::isGoal() {
//	//pNearest_node�� goal ���� �Ÿ��� 100 �̸��̸� ��ǥ������ �����ߴٰ� ����.
//
//	int dist = f_distance(pNearest_node->point, Goal_node->point);	//���� ������� ���� ��ǥ�������� �Ÿ� ���
//	if (dist < 30) return true;
//
//	return false;
//}
//
