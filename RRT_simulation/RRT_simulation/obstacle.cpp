#include "hybrid_Astar_rrt.h"

//Nearest_node 가 장애물과 겹쳐있으면 0, 안겹쳐있으면 1 반환
bool Obstacle::isObstacle(h_node* Nearest_node) {
	if (rect1.contains(Nearest_node->point) || rect2.contains(Nearest_node->point) || rect3.contains(Nearest_node->point)
		|| rect4.contains(Nearest_node->point) || rect5.contains(Nearest_node->point) || rect6.contains(Nearest_node->point)
		|| rect7.contains(Nearest_node->point) || rect8.contains(Nearest_node->point)
		|| rect9.contains(Nearest_node->point) || rect10.contains(Nearest_node->point)
		|| rect11.contains(Nearest_node->point) || rect12.contains(Nearest_node->point)
		|| rect13.contains(Nearest_node->point) || rect14.contains(Nearest_node->point)
		|| rect15.contains(Nearest_node->point)) return true;
	else return false;
}

void Obstacle::obstacle1(Mat dstImage) {
	rect1.x = 860;
	rect1.y = 250;
	rect1.width = 100;
	rect1.height = 300;

	rect2.x = 1200;
	rect2.y = 250;
	rect2.width = 100;
	rect2.height = 300;

	rectangle(dstImage, rect1, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect2, Scalar(0, 0, 0), 2);

}

void Obstacle::obstacle2(Mat dstImage) {

	rect1.x = 400;
	rect1.y = 0;
	rect1.width = 150;
	rect1.height = 300;

	rect2.x = 0;
	rect2.y = 340;
	rect2.width = 300;
	rect2.height = 300;

	rect3.x = 700;
	rect3.y = 300;
	rect3.width = 200;
	rect3.height = 440;


	rectangle(dstImage, rect1, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect2, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect3, Scalar(0, 0, 0), 2);

}

void Obstacle::obstacle3(Mat dstImage) {
	rect1.x = 250;
	rect1.y = 250;
	rect1.width = 200;
	rect1.height = 200;

	rect2.x = 800;
	rect2.y = 0;
	rect2.width = 400;
	rect2.height = 400;

	rect3.x = 300;
	rect3.y = 500;
	rect3.width = 500;
	rect3.height = 140;


	rectangle(dstImage, rect1, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect2, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect3, Scalar(0, 0, 0), 2);
}


//시작은 
void Obstacle::obstacle4(Mat dstImage) {

	rect1.x = 150;
	rect1.y = 0;
	rect1.width = 300;
	rect1.height = 200;

	rect2.x = 50;
	rect2.y = 250;
	rect2.width = 200;
	rect2.height = 200;

	rect3.x = 200;
	rect3.y = 550;
	rect3.width = 200;
	rect3.height = 90;

	rect4.x = 700;
	rect4.y = 0;
	rect4.width = 200;
	rect4.height = 150;

	rect5.x = 500;
	rect5.y = 150;
	rect5.width = 100;
	rect5.height = 110;

	rect6.x = 400;
	rect6.y = 270;
	rect6.width = 50;
	rect6.height = 50;

	rect7.x = 350;
	rect7.y = 400;
	rect7.width = 50;
	rect7.height = 50;

	rect8.x = 850;
	rect8.y = 200;
	rect8.width = 70;
	rect8.height = 50;

	rect9.x = 670;
	rect9.y = 240;
	rect9.width = 140;
	rect9.height = 80;

	rect10.x = 550;
	rect10.y = 300;
	rect10.width = 100;
	rect10.height = 150;

	rect11.x = 900;
	rect11.y = 300;
	rect11.width = 50;
	rect11.height = 50;

	rect12.x = 800;
	rect12.y = 400;
	rect12.width = 70;
	rect12.height = 50;

	rect13.x = 520;
	rect13.y = 500;
	rect13.width = 100;
	rect13.height = 50;

	rect14.x = 1050;
	rect14.y = 330;
	rect14.width = 200;
	rect14.height = 100;

	rect15.x = 930;
	rect15.y = 530;
	rect15.width = 300;
	rect15.height = 90;

	rectangle(dstImage, rect1, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect2, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect3, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect4, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect5, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect6, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect7, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect8, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect9, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect10, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect11, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect12, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect13, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect14, Scalar(0, 0, 0), 2);
	rectangle(dstImage, rect15, Scalar(0, 0, 0), 2);

}
