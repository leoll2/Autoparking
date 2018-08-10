#include <algorithm>
#include "q_matrix.h"

Q_Matrix::Q_Matrix(int s, int a) :
	states(s),
	actions(a),
	Q(s, vector<float>(a, 0));
{}

float Q_Matrix::get_quality(int s, int a) {
	return Q[s][a];
}

float Q_Matrix::get_max_quality(int s) {
	return *std::max_element(Q[s].begin(), Q[s].end())
}