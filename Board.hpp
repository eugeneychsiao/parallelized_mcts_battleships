

#include "Ship.hpp"
#include "Constants.hpp"
#include <vector>

#ifndef BOARD_H
#define BOARD_H

struct Foo
{
     int value1;
     int value2;
     double val;
     std::vector<int> xValues;
     std::vector<int> yValues;
     std::vector<double> ProbValues;
};

class Board
{
	private:
		char gameBoard[BOARD_DIM][BOARD_DIM];
		std::vector<Ship> shipVec;

	public:
		Board();
		Board(const Board &oldBoard);
		Board& operator=(const Board &right);
		~Board() {return;}; 
		int getNumHits();
		void printPrivateBoard();
		void printPublicBoard();
		char getSpaceValue(int x, int y);
		bool recordHit(int x, int y);
		bool placeShip(int shipNum, int x, int y, bool isHorizontal);
		void simulate(int x, int y, char val);
		Foo ComputeProbability2( int numfinish,int r, int remain, unsigned int* see);
		Foo ComputeProbability( int numfinish,int r, int remain, double value[BOARD_DIM][BOARD_DIM]);

};

#endif




