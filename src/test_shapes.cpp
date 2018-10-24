#include <iostream>
#include "shapes.h"

int main() {
	
	std::cout << "TESTING COLLISIONS BETWEEN POLYGONS..." << std::endl;
	/*TEST1*/
	std::vector<Coordinate> v1 = {{0, 0}, {1, 0}, {0, 1}};
	std::vector<Coordinate> v2 = {{1, 1}, {2, 1}, {1, 2}};
	Polygon p1(v1);
	Polygon p2(v2);
	if (!check_collision(p1, p2) && !check_collision(p2, p1))
		std::cout << "Test 1 passed" << std::endl;
	else
		std::cout << "Test 1 not passed" << std::endl;
		
	/*TEST2*/
	std::vector<Coordinate> v3 = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
	std::vector<Coordinate> v4 = {{1, 1}, {2, 1}, {1, 2}};
	Polygon p3(v3);
	Polygon p4(v4);
	if (check_collision(p3, p4) && check_collision(p4, p3))
		std::cout << "Test 2 passed" << std::endl;
	else
		std::cout << "Test 2 not passed" << std::endl;
		
	/*TEST3*/
	std::vector<Coordinate> v5 = {{0, 0}, {1, 0}, {0, 1}, {5, 5}};
	std::vector<Coordinate> v6 = {{1, 1}, {2, 1}, {1, 2}};
	Polygon p5(v5);
	Polygon p6(v6);
	if (check_collision(p5, p6) && check_collision(p6, p5))
		std::cout << "Test 3 passed" << std::endl;
	else
		std::cout << "Test 3 not passed" << std::endl;

	/*TEST4*/
	std::vector<Coordinate> v7 = {{1, 1}, {3, 1}, {2, 2}};
	std::vector<Coordinate> v8 = {{-1, -1}, {2, 0}, {5, 1}, {3, -4}};
	Polygon p7(v7);
	Polygon p8(v8);
	if (!check_collision(p7, p8) && !check_collision(p8, p7))
		std::cout << "Test 4 passed" << std::endl;
	else
		std::cout << "Test 4 not passed" << std::endl;


	std::cout << "TESTING COLLISIONS BETWEEN ARCS AND POLYGONS..." << std::endl;
	/*TEST5*/
	std::vector<Coordinate> v9 = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
	Polygon p9(v9);
	Arc a1(Coordinate(2,2), 2, 0, 3.0);
	if (!check_collision(p9, a1))
		std::cout << "Test 5 passed" << std::endl;
	else
		std::cout << "Test 5 not passed" << std::endl;
		
	/*TEST6*/
	std::vector<Coordinate> v10 = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
	Polygon p10(v10);
	Arc a2(Coordinate(2,2), 2, 3.0, 1.0);
	if (check_collision(p10, a2))
		std::cout << "Test 6 passed" << std::endl;
	else
		std::cout << "Test 6 not passed" << std::endl;
	
	/*TEST7*/
	std::vector<Coordinate> v11 = {{0, 2}, {1.5, 1}, {1, -1}, {-1, -1}, {-1.5, 1}};
	Polygon p11(v11);
	Arc a3(Coordinate(0,0), 2.1, 0.3, 1.7);
	if (!check_collision(p11, a3))
            std::cout << "Test 7 passed" << std::endl;
	else
            std::cout << "Test 7 not passed" << std::endl;
		
	/*TEST8*/
	std::vector<Coordinate> v12 = {{0, 2}, {1.5, 1}, {1, -1}, {-1, -1}, {-1.5, 1}};
	Polygon p12(v12);
	Arc a4(Coordinate(0,0), 1.7, 0.3, 1.4);
	if (check_collision(p12, a4))
            std::cout << "Test 8 passed" << std::endl;
	else
            std::cout << "Test 8 not passed" << std::endl;
	
        
        /*TEST9*/
	std::vector<Coordinate> v13 = {{350, 900}, {600, 900}, {600, 1200}, {350, 1200}};
	Polygon p13(v13);
	Arc a5(Coordinate(-275, 425), 743.8, 0.664, 0.3598);
	if (!check_collision(p13, a5))
            std::cout << "Test 9 passed" << std::endl;
	else
            std::cout << "Test 9 not passed" << std::endl;
	
	return 0;
}