#ifndef HYBRID_ASTAR_RRT

#include <opencv2/opencv.hpp>
#include <vector>
#include <queue>

using namespace cv;
using namespace std;

#define HYBRID_ASTAR_RRT

#define MAP_SIZE_Y 640
#define MAP_SIZE_X 1280

#define h_CHILD_SIZE 3
#define PI 3.1415926535

#define LINK_LENGTH 20



typedef struct Hybrid_node h_node;
struct Hybrid_node {
	Point2f point;
	h_node* node_parent;
	h_node* node_child[h_CHILD_SIZE];
};

class Hybrid_Tree {
public:
	Hybrid_Tree(Mat dstImage, float start_x, float start_y, float Goal_x, float Goal_y);

	h_node* RandomNode(Mat dstImage);
	void NearestNode(h_node* h_NodeRandom, h_node* h_NodeCurrent, float pre_distance, int d_flag);
	void NewNode(Mat dstImage, h_node* parent_node);
	void First3Child(Mat dstImage, float root_angle);
	void getLeafNode(h_node* pNearest_node);
	bool isGoal();
	bool isInMap(h_node* Nearest_node);	//map 전체에 rect 씌워서 -> contain == 1 일때만 노드에서 뻗어나가게 하자.
	void showPath(Mat dstImage);
	bool IsinEllipse(h_node* root_node, h_node* goal_node, float l, h_node* pNearest_node);
	void createMap(Mat dstImage, float start_x, float start_y, float Goal_x, float Goal_y);
	void updatePath(Mat dstImage);
	void checkDistance();

	queue<h_node*> que;
	Rect map;
	h_node* node_random;
	h_node* Goal_node;
	h_node* root_node;
	h_node* pNearest_node;
	h_node* node_Goalin;
	float g_distance;
	float path_distance;
	int arrive_flag;
	int pathUpdate_flag;
};

class Obstacle {
public:
	void obstacle1(Mat dstImage);
	void obstacle2(Mat dstImage);
	void obstacle3(Mat dstImage);
	void obstacle4(Mat dstImage);
	bool isObstacle(h_node* Nearest_node);
	bool isObstacle2(h_node* Nearest_node);

	Rect rect1;
	Rect rect2;
	Rect rect3;
	Rect rect4;
	Rect rect5;
	Rect rect6;
	Rect rect7;
	Rect rect8;
	Rect rect9;
	Rect rect10;
	Rect rect11;
	Rect rect12;
	Rect rect13;
	Rect rect14;
	Rect rect15;

};


float f_distance(Point2f p1, Point2f p2);
Point2f rotation_point(Point2f base_point, Point2f target_point, float theta);
Point2f translate_point(Point2f starting_point, float angle, float len);

#endif // !1

