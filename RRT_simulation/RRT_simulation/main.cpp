
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

	Obstacle obs;		//장애물 생성
	obs.obstacle4(dstImage);
	obs.obstacle4(dstImage_pathplang);

	//1~3번 맵
	//Hybrid_Tree RRT(dstImage, 100, 100, MAP_SIZE_X - 200, MAP_SIZE_Y - 200);	//생성자
	//RRT.createMap(dstImage_pathplang, 100, 100, MAP_SIZE_X - 200, MAP_SIZE_Y - 200);

	//4번 맵
	Hybrid_Tree RRT(dstImage, 300, 500,  1000 , 100);	//생성자
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


	//첫번째 경로가 도출되면 탈출
	while (z--) {

		node_random = RRT.RandomNode(dstImage);

		RRT.NearestNode(node_random, RRT.root_node, 200000000, 0);	//이거 하고나면 가장 가까운 노드가 .pNearestNode 에 저장되어있어야해
		if (RRT.pNearest_node->node_child[0] != nullptr) RRT.getLeafNode(RRT.pNearest_node);

		//도착경로가 생긴 이후로는 타원을 생성하고 계속 크기를 업데이트한다.
		if (RRT.arrive_flag && !(RRT.IsinEllipse(RRT.root_node, RRT.Goal_node, tmp, RRT.pNearest_node))) continue;
		if (!(RRT.arrive_flag) && !(RRT.isInMap(RRT.pNearest_node))) continue;		//노드가 맵 밖으로 나가면 거기서는 child 생성하지마.
		//if ( !(RRT.isInMap(RRT.pNearest_node))) continue;		//노드가 맵 밖으로 나가면 거기서는 child 생성하지마.

		if (obs.isObstacle(RRT.pNearest_node))  continue;		//장애물과 겹치면 child 생성 하지마

		RRT.NewNode(dstImage, RRT.pNearest_node);

		if (RRT.isGoal()) {
			if (RRT.pathUpdate_flag == 1) RRT.updatePath(dstImage_pathplang);
			RRT.checkDistance();

			RRT.arrive_flag = 1;
			if (tmp > RRT.path_distance) {
				tmp = RRT.path_distance;
				cout << "현재 발견된 최단경로의 길이는 : " << tmp << endl;
				RRT.showPath(dstImage_pathplang);	//여기서 RRT.path_distance 가 업데이트돼
				imshow(str + num[cnt_goal++], dstImage_pathplang);
				waitKey(1);
			}


			RRT.path_distance = 0;
			//break; 
		}

		RRT.pNearest_node = RRT.root_node;
		delete node_random;		//memory save

		//화면 보여주기
		cnt++;
		if (cnt == 50) {
			cnt = 0;
			imshow("result1", dstImage);
			waitKey(1);
		}


	}


	cout << "경로를 " << HOW_MANY_TIMES << "번 탐색하였습니다." << endl;
	//cout << "생성된 경로의 길이는 " << RRT.path_distance << endl;

	imshow("result1", dstImage);
	waitKey(0);

	return 0;
}

