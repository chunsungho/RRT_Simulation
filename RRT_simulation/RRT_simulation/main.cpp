
//#include "rrt_tree.h"
#include "hybrid_Astar_rrt.h"
#include <iostream>
#include <time.h>

#define HOW_MANY_TIMES 100000

using namespace cv;
using namespace std;

int main() {

	Mat dstImage(MAP_SIZE_Y, MAP_SIZE_X, CV_8UC3, Scalar(255, 255, 255));
	Mat dstImage_pathplang(MAP_SIZE_Y, MAP_SIZE_X, CV_8UC3, Scalar(255, 255, 255));
	if (dstImage.empty() || dstImage_pathplang.empty())
		return -1;

	Obstacle obs;		//��ֹ� ����
	obs.obstacle4(dstImage);
	obs.obstacle4(dstImage_pathplang);

	//1~3�� ��
	//Hybrid_Tree RRT(dstImage, 100, 100, MAP_SIZE_X - 200, MAP_SIZE_Y - 200);	//������
	//RRT.createMap(dstImage_pathplang, 100, 100, MAP_SIZE_X - 200, MAP_SIZE_Y - 200);

	//4�� ��
	Hybrid_Tree RRT(dstImage, 300, 500,  1000 , 100);	//������
	RRT.createMap(dstImage_pathplang, 300, 500, 1000, 100);

	h_node* node_random;
	h_node node_new;
	int cnt = 0, cnt_goal = 0;
	int z = HOW_MANY_TIMES;
	float tmp = 20000000;
	int k = 0;
	String str = "Path_planning";
	String num[100] = { "1", "2", "3" , "4", "5", "6", "7", "8", "9" , "10" , "11" , "12" , "13" , "14", "15"
	, "16" , "17", "18", "19", "20", "21" };


	//ù��° ��ΰ� ����Ǹ� Ż��
	while (z--) {

		node_random = RRT.RandomNode(dstImage);

		RRT.NearestNode(node_random, RRT.root_node, 200000000, 0);	//�̰� �ϰ��� ���� ����� ��尡 .pNearestNode �� ����Ǿ��־����
		if (RRT.pNearest_node->node_child[0] != nullptr) RRT.getLeafNode(RRT.pNearest_node);

		//������ΰ� ���� ���ķδ� Ÿ���� �����ϰ� ��� ũ�⸦ ������Ʈ�Ѵ�.
		if (RRT.arrive_flag && !(RRT.IsinEllipse(RRT.root_node, RRT.Goal_node, tmp, RRT.pNearest_node))) continue;
		if (!(RRT.arrive_flag) && !(RRT.isInMap(RRT.pNearest_node))) continue;		//��尡 �� ������ ������ �ű⼭�� child ����������.
		//if ( !(RRT.isInMap(RRT.pNearest_node))) continue;		//��尡 �� ������ ������ �ű⼭�� child ����������.

		if (obs.isObstacle(RRT.pNearest_node))  continue;		//��ֹ��� ��ġ�� child ���� ������

		RRT.NewNode(dstImage, RRT.pNearest_node);

		if (RRT.isGoal()) {
			if (RRT.pathUpdate_flag == 1) RRT.updatePath(dstImage_pathplang);
			RRT.checkDistance();

			RRT.arrive_flag = 1;
			if (tmp > RRT.path_distance) {
				tmp = RRT.path_distance;
				cout << "���� �߰ߵ� �ִܰ���� ���̴� : " << tmp << endl;
				RRT.showPath(dstImage_pathplang);	//���⼭ RRT.path_distance �� ������Ʈ��
				imshow(str + num[cnt_goal++], dstImage_pathplang);
				waitKey(1);
			}


			RRT.path_distance = 0;
			//break; 
		}

		RRT.pNearest_node = RRT.root_node;
		delete node_random;		//memory save

		//ȭ�� �����ֱ�
		cnt++;
		if (cnt == 50) {
			cnt = 0;
			imshow("result1", dstImage);
			waitKey(1);
		}


	}


	cout << "��θ� " << HOW_MANY_TIMES << "�� Ž���Ͽ����ϴ�." << endl;
	//cout << "������ ����� ���̴� " << RRT.path_distance << endl;

	imshow("result1", dstImage);
	waitKey(0);

	return 0;
}

