#include "Battle.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstdlib>
#include <climits>

#include <ctime>

class MCTS
{
	private:
		void Simulator(Board b);
		int Rollout(Board b, int numfinish, int remain, int rn, int num, int see);
		void findMaxArray(int* bx, int* by, double Array[BOARD_DIM][BOARD_DIM]);
		int Values[BOARD_DIM][BOARD_DIM];
		void findMax(int* x, int* y);
		int NumHits;
	public:
		int r;
		int rem;
		unsigned int* seeds[BRANCH];
		int count;
		void SelectAction(Board b, int* x, int* y);
		MCTS(int R, int N, int remain)
		{
			NumHits=N;
			rem=remain;
			r=R;	
			count=0;
			for (int i = 0; i < BOARD_DIM; i++) {
				for (int j = 0; j < BOARD_DIM; j++) {	
					
					Values[i][j]=INT_MAX;
					
				}
			}
			
			unsigned int a[BRANCH];
			for (int i=0; i<BRANCH; i++)
			{
				a[i]=rand();
				seeds[i]=&a[i];
				
			}
		};
};
