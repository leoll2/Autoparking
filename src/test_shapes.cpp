#include <iostream>
#include "shapes.h"

int main() {
	
	/*TEST1*/
	vector<Coordinate> v1 = {{0, 0}, {1, 0}, {0, 1}};
	vector<Coordinate> v2 = {{1, 1}, {2, 1}, {1, 2}};
	Polygon p1(3, v1);
	Polygon p2(3, v2);
	if (!check_collision(p1, p2) && !check_collision(p2, p1))
		cout << "Test 1 passed" << endl;
	else
		cout << "Test 1 not passed" << endl;
		
	/*TEST2*/
	vector<Coordinate> v3 = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
	vector<Coordinate> v4 = {{1, 1}, {2, 1}, {1, 2}};
	Polygon p3(4, v3);
	Polygon p4(3, v4);
	if (check_collision(p3, p4) && check_collision(p4, p3))
		cout << "Test 2 passed" << endl;
	else
		cout << "Test 2 not passed" << endl;
		
	/*TEST3*/
	vector<Coordinate> v5 = {{0, 0}, {1, 0}, {0, 1}, {5, 5}};
	vector<Coordinate> v6 = {{1, 1}, {2, 1}, {1, 2}};
	Polygon p5(4, v5);
	Polygon p6(3, v6);
	if (check_collision(p5, p6) && check_collision(p6, p5))
		cout << "Test 3 passed" << endl;
	else
		cout << "Test 3 not passed" << endl;

	/*TEST4*/
	vector<Coordinate> v7 = {{1, 1}, {3, 1}, {2, 2}};
	vector<Coordinate> v8 = {{-1, -1}, {2, 0}, {5, 1}, {3, -4}};
	Polygon p7(3, v5);
	Polygon p8(4, v6);
	if (!check_collision(p7, p8) && !check_collision(p8, p7))
		cout << "Test 4 passed" << endl;
	else
		cout << "Test 4 not passed" << endl;

	return 0;
}