#include "hybrid_Astar_rrt.h"

pair<int, int> h_random_list[3840];

float f_distance(Point2f p1, Point2f p2) {
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

Hybrid_Tree::Hybrid_Tree(Mat dstImage, float start_x, float start_y, float Goal_x, float Goal_y) {

	/* ������ ��� ���� */
	root_node = new h_node;
	root_node->point.x = start_x;
	root_node->point.y = start_y;
	root_node->node_parent = nullptr;


	/* root ����� �ڽ� ���� */
	//root����� �ڽ� �ʱ�ȭ
	for (int i = 0; i < h_CHILD_SIZE; i++) {
		root_node->node_child[i] = nullptr;
	}

	circle(dstImage, root_node->point, 5, Scalar(0, 0, 255), 5);


	//GoalNode ����
	Goal_node = new h_node;
	Goal_node->point.x = Goal_x;

	Goal_node->point.y = Goal_y;

	circle(dstImage, Goal_node->point, 5, Scalar(0, 0, 255), 5);

	//ù 3���� child ����(������� ���������� �̷��� ��)
	First3Child(dstImage, 0 * PI / 180);

	//�����ڿ��� �� ����.
	map.x = 0;
	map.y = 0;
	map.width = MAP_SIZE_X;
	map.height = MAP_SIZE_Y;


	//�׿� ���� �ʱ�ȭ
	arrive_flag = 0;
	g_distance = 2000000000;
	path_distance = 0;

	pNearest_node = root_node;
}


h_node* Hybrid_Tree::RandomNode(Mat dstImage) {

	//srand(time(NULL));

	node_random = new h_node;

	/* ������� ��ǥ �Ҵ� �� �ð�ȭ */
	node_random->point.x = rand() % MAP_SIZE_X;
	node_random->point.y = rand() % MAP_SIZE_Y;

	return node_random;

}

void Hybrid_Tree::NearestNode(h_node *h_NodeRandom, h_node* h_NodeCurrent, float pre_distance, int d_flag) {

	//base case 1. d_flag�� ���� �־����� ����
	float current_distance = f_distance(h_NodeCurrent->point, h_NodeRandom->point);

	//���� ����Ű�� ��尡 �� �־����� ������ Ž������
	if (pre_distance < current_distance) {
		d_flag++;
		if (d_flag == 1) {
			return;	//d_flag == N �� ���� Ž���� �󸶳� �ɵ��ְ� �ϴ����� �����ȴ�.
		}

	}

	//��尡 �� ��������Ÿ� ����
	else {
		d_flag = 0;
		if (g_distance > current_distance) {
			g_distance = current_distance;				//����� ���Ŵϱ� �Ÿ����
			pNearest_node = h_NodeCurrent;			//���� ��带 ���
		}


		if (h_NodeCurrent->node_child[0] != nullptr) {	//�ڽ��� �ִٸ�

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


	h_node* node_new[3];	//nearest node �� �ٿ��� child 3�� ����

	//child�� ��ǥ����
	float angle_betweenChild = 50 * PI / 180;
	float angle_brach;	//�������� ����� ����

	if (parent_node->node_parent == nullptr) return;
	//�������� ����� ����ã��
	if (parent_node->point.x - parent_node->node_parent->point.x == 0) {	//����ó��. ���Ⱑ ���Ѵ��ΰ�� ( ������ y��� ���� )

		if (parent_node->point.y - parent_node->node_parent->point.y > 0) angle_brach = 90 * PI / 180;
		else angle_brach = -90 * PI / 180;
	}
	else {

		angle_brach = atan2(parent_node->point.y - parent_node->node_parent->point.y, parent_node->point.x - parent_node->node_parent->point.x);
	}

	for (int i = 0; i < 3; i++) {
		node_new[i] = new h_node;	//child ����

		parent_node->node_child[i] = node_new[i];	//nearest node �� child�� ����
		node_new[i]->node_parent = parent_node;		//child�� �θ� ����

		//child �� child �� nullptr �� ����
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

	g_distance = 2000000000;	//�̰� �ʱ�ȭ �������
}


void Hybrid_Tree::First3Child(Mat dstImage, float root_angle) {
	h_node* node_new[3];	//nearest node �� �ٿ��� child 3�� ����

	//child�� ��ǥ����
	float angle_betweenChild = 40 * PI / 180;

	for (int i = 0; i < 3; i++) {
		node_new[i] = new h_node;	//child ����

		root_node->node_child[i] = node_new[i];	//nearest node �� child�� ����
		node_new[i]->node_parent = root_node;	//child�� �θ� ����


		//child �� child �� nullptr �� ����
		for (int j = 0; j < 3; j++) {
			node_new[i]->node_child[j] = nullptr;
		}

		Point2f tmp1, tmp2, goal_point;

		if (i != 1) {
			tmp1 = translate_point(root_node->point, root_angle, 46);	//tmp1 = root_node �� �ش� ����,�Ÿ���ŭ �δ�.
			tmp2 = rotation_point(root_node->point, tmp1, (1 - i) * 90 * PI / 180);		//tmp2 = root_node �� �߽����� tmp1�� 90�� ȸ��
			goal_point = rotation_point(tmp2, root_node->point, (1 - i) * angle_betweenChild);		//goal_point = tmp2�� �߽����� root_node�� �ش� ������ŭ ������.

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


//base_point ���ؼ� target_point�� �󸶸�ŭ ȸ��
//������ �������� �־������
//���� ����� ������ �ð�������� ���ư�. (z���� ȭ�� ���� ����)
Point2f rotation_point(Point2f base_point, Point2f target_point, float theta) {
	Point2f goal_point;
	goal_point.x = cos(theta) * (target_point.x - base_point.x)
		- sin(theta)* (target_point.y - base_point.y) + base_point.x;
	goal_point.y = sin(theta) * (target_point.x - base_point.x)
		+ cos(theta) * (target_point.y - base_point.y) + base_point.y;
	return goal_point;
}

//starting_point �� � ������ ���� ������ �󸶸�ŭ �а��ΰ�.
//angle �� �������� �־�����Ѵ�.
Point2f translate_point(Point2f starting_point, float angle, float len) {
	Point2f goal_point;
	goal_point.x = starting_point.x + len * cos(angle);
	goal_point.y = starting_point.y + len * sin(angle);

	return 	goal_point;
}

bool Hybrid_Tree::isGoal() {

	//pNearest_node�� goal ���� �Ÿ��� 100 �̸��̸� ��ǥ������ �����ߴٰ� ����.
	for (int i = 0; i < 3; i++) {
		int dist = f_distance(pNearest_node->node_child[i]->point, Goal_node->point);	//���� ������� ���� ��ǥ�������� �Ÿ� ���
		if (dist < 100) {
			node_Goalin = pNearest_node->node_child[i];
			return true;
		}

	}


	return false;
}

//�� �Լ��� ������ pNearest_node ���� leaf ��尡 �� ���°� �Ǿ���Ѵ�.
void Hybrid_Tree::getLeafNode(h_node* pNearest_node) {
	int random = rand() % 3;
	//��尡 �߰������̸� ������ �������� �����Ѵ�.
	while (pNearest_node->node_child[0] != nullptr) {
		pNearest_node = pNearest_node->node_child[random];
	}
}

void Hybrid_Tree::showPath(Mat dstImage) {
	h_node* node_temp = node_Goalin;
	pathUpdate_flag = 1;
	//path_distance += f_distance(Goal_node->point, node_Goalin->point);

	line(dstImage, Goal_node->point, node_temp->point, Scalar(255, 0, 0), 1);

	while (node_temp) {	//root_node �� ���ö����� �ö󰣴�.
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
	while (node_temp) {	//root_node �� ���ö����� �ö󰣴�.
		if (node_temp->node_parent != nullptr) {
			path_distance += 39;
		}
		node_temp = node_temp->node_parent;
	}
}

//main ������ showPath �� �ٽ� ȣ��( ���� ��ζ� ������Ʈ�� ��� �Ѵ� �Ķ����� ��)
//-> que �� root_node �� ���Ë����� �����鼭 ���������� �ٽ� ĥ�Ѵ�.

void Hybrid_Tree::updatePath(Mat dstImage) {
	pathUpdate_flag = 0;
	h_node* node_temp;
	node_temp = que.front();

	//line : goal_node�� node_goalin 
	line(dstImage, Goal_node->point, node_temp->point, Scalar(0, 0, 255), 1);

	while (node_temp != root_node) {

		circle(dstImage, node_temp->point, 2, Scalar(0, 0, 255), 2);
		line(dstImage, node_temp->node_parent->point, node_temp->point, Scalar(0, 0, 255), 1);

		que.pop();
		node_temp = que.front();
	}
	que.pop(); //root_node �� pop

}

//�������� ������ ���������ϴ� Ÿ���ȿ� pNearestNode �� ���� true ��ȯ
bool Hybrid_Tree::IsinEllipse(h_node* start_node, h_node* end_node, float l, h_node* pNearest_node) {
	float x1 = start_node->point.x, y1 = start_node->point.y;
	float x2 = end_node->point.x, y2 = end_node->point.y;
	float a = l / 2;
	float c_sqr = powf((x1 - (x1 + x2) / 2), 2) + powf((y1 - (y1 + y2) / 2), 2); //������ǥ�� ����
	float b = sqrt(a*a - c_sqr);
	float w = atan2(y2 - y1, x2 - x1);	//�������� ���ð�

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