#include <iostream>
#include "shapes.h"

int main() {
	
	cout << "TESTING COLLISIONS BETWEEN POLYGONS..." << endl;
	/*TEST1*/
	vector<Coordinate> v1 = {{0, 0}, {1, 0}, {0, 1}};
	vector<Coordinate> v2 = {{1, 1}, {2, 1}, {1, 2}};
	Polygon p1(v1);
	Polygon p2(v2);
	if (!check_collision(p1, p2) && !check_collision(p2, p1))
		cout << "Test 1 passed" << endl;
	else
		cout << "Test 1 not passed" << endl;
		
	/*TEST2*/
	vector<Coordinate> v3 = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
	vector<Coordinate> v4 = {{1, 1}, {2, 1}, {1, 2}};
	Polygon p3(v3);
	Polygon p4(v4);
	if (check_collision(p3, p4) && check_collision(p4, p3))
		cout << "Test 2 passed" << endl;
	else
		cout << "Test 2 not passed" << endl;
		
	/*TEST3*/
	vector<Coordinate> v5 = {{0, 0}, {1, 0}, {0, 1}, {5, 5}};
	vector<Coordinate> v6 = {{1, 1}, {2, 1}, {1, 2}};
	Polygon p5(v5);
	Polygon p6(v6);
	if (check_collision(p5, p6) && check_collision(p6, p5))
		cout << "Test 3 passed" << endl;
	else
		cout << "Test 3 not passed" << endl;

	/*TEST4*/
	vector<Coordinate> v7 = {{1, 1}, {3, 1}, {2, 2}};
	vector<Coordinate> v8 = {{-1, -1}, {2, 0}, {5, 1}, {3, -4}};
	Polygon p7(v7);
	Polygon p8(v8);
	if (!check_collision(p7, p8) && !check_collision(p8, p7))
		cout << "Test 4 passed" << endl;
	else
		cout << "Test 4 not passed" << endl;


	cout << "TESTING COLLISIONS BETWEEN ARCS AND POLYGONS..." << endl;
	/*TEST5*/
	vector<Coordinate> v9 = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
	Polygon p9(v9);
	Arc a1(Coordinate(2,2), 2, 0, 3.0);
	if (!check_collision(p9, a1))
		cout << "Test 5 passed" << endl;
	else
		cout << "Test 5 not passed" << endl;
		
	/*TEST6*/
	vector<Coordinate> v10 = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
	Polygon p10(v10);
	Arc a2(Coordinate(2,2), 2, 3.0, 1.0);
	if (check_collision(p10, a2))
		cout << "Test 6 passed" << endl;
	else
		cout << "Test 6 not passed" << endl;
	
	/*TEST7*/
	vector<Coordinate> v11 = {{0, 2}, {1.5, 1}, {1, -1}, {-1, -1}, {-1.5, 1}};
	Polygon p11(v11);
	Arc a3(Coordinate(0,0), 2.1, 0.3, 1.7);
	if (!check_collision(p11, a3))
		cout << "Test 7 passed" << endl;
	else
		cout << "Test 7 not passed" << endl;
		
	/*TEST8*/
	vector<Coordinate> v12 = {{0, 2}, {1.5, 1}, {1, -1}, {-1, -1}, {-1.5, 1}};
	Polygon p12(v12);
	Arc a4(Coordinate(0,0), 1.7, 0.3, 1.4);
	if (check_collision(p12, a4))
		cout << "Test 8 passed" << endl;
	else
		cout << "Test 8 not passed" << endl;
	
	return 0;
}