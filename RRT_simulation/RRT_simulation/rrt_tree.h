//#ifndef RRT_TREE_H
//
//#include <opencv2/opencv.hpp>
//#include <vector>
//#include <stack>
//
//using namespace cv;
//using namespace std;
//
//#define RRT_TREE_H
//
//#define MAP_SIZE_Y 640
//#define MAP_SIZE_X 1280
//
//#define CHILD_SIZE 2
//
//
//typedef struct node node_;
//struct node {
//	Point2f point;
//
//	node_* node_parent;
//	node_* node_child[CHILD_SIZE];
//
//};
//
//
//
//
//class Tree {
//public:
//	Tree();
//	Tree(Mat dstImage, float x, float y);
//
//	node_* RandomNode(Mat dstImage);
//	void NearestNode(node_ *node_random, node_* node_current, float pre_distance, int d_flag);
//	void NewNode(Mat dstImage, node_* node_random, node_* node_nearest);
//	void DeleteNode(node_ node);
//	void MakeRandomList();
//	bool isGoal();
//
//	//queue<node_> que;
//	stack<node_> st;
//
//
//	node_* Goal_node;
//	node_* root_node;
//	node_* pNearest_node;
//	float g_distance;
//};
//
//
//float f_distance(Point2f p1, Point2f p2);
//
//
//
//#endif // !RRT_TREE_H
//
