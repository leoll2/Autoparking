#ifndef Q_MATRIX_H
#define Q_MATRIX_H

#include <vector>

class Q_Matrix {
	private:
		int states;
		int	actions;
		std::vector<std::vector<float> > Q;
		
	public:
		/* Constructor taking as arguments the number of states and actions*/
		Q_Matrix(int s, int a);
		
		/* Returns the value in Q for the state-action pair (s,a). */
		float get_quality(int s, int a);
		
		/* Returns the maximum quality value for the state s. */
		float Q_Matrix::get_max_quality(int s);
}

#endif