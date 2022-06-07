#include "Board.hpp"
#include "Player.hpp"

#ifndef Battle_H
#define Battle_H

enum state {P1_WIN, P2_WIN, UNFINISHED};

class Battle
{
	private: 
		Player p1;
		Board p1Board;
		Player p2;
		Board p2Board;
	public:		
		bool playBattle();
		void startBattle();
		void initializeBoard(Board &b);
		void initializeBoardAuto(Board &b, bool print=true);
		state BattleCondition();
		void printBattleState(Player p);
		void getNextMove(Board &b);
		void getNextMoveAuto(Board &b, int remain);
		void getNextMoveAuto2(Board &b);
		std::string getSquare();
		void switchPlayers(std::string playerFrom, std::string playerTo);
		int moves_left_P1=BOARD_DIM*BOARD_DIM;
		int moves_left_P2=BOARD_DIM*BOARD_DIM;
		int nmoves=0;
};

#endif 
