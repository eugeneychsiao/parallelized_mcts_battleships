#include "Battle.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "MCTS.hpp"

// function to play Battle
bool Battle::playBattle()
{
	int firstPlayer, currentPlayer, toPlay;
	Board * bptr;
	Player * pptr;
	time_t timer;
	time(&timer);
	
	startBattle();

	std::cout<<"Which player will make the first move (enter 1 or 2)?\n";
	firstPlayer=getInt(1,2);


	if (firstPlayer==1)
	{
		pptr = &p1; 
		bptr = &p2Board;
	}
	else {
		pptr = &p2;
		bptr = &p1Board;
	}

	while(BattleCondition()==UNFINISHED)
	{
		if ((*pptr).isPlayerAutomatic()==1)
		{
			getNextMoveAuto2((*bptr));
			
		}
		else if((*pptr).isPlayerAutomatic()==2)
		{
			if ((*pptr).getPlayerNum()==1)
				getNextMoveAuto((*bptr), moves_left_P1);
			if ((*pptr).getPlayerNum()==2)
				getNextMoveAuto((*bptr), moves_left_P2);
			nmoves+=1;
			std::cout<<nmoves<<std::endl;
			
		}
		else
		{
			printBattleState(*(pptr));
			getNextMove(*(bptr));
			printBattleState(*(pptr));
		}

		// switch pointers at the end of each turn
		if ((*pptr).getPlayerNum()==1)
		{
			if (!p1.isPlayerAutomatic() && !p2.isPlayerAutomatic()) 
			{
				switchPlayers(p1.getName(), p2.getName());
			}
			pptr = &p2; 
			bptr = &p1Board;
			moves_left_P1-=1;
			
		}
		else {
			if (!p1.isPlayerAutomatic() && !p2.isPlayerAutomatic())
			{
				switchPlayers(p2.getName(), p1.getName());
			}
			pptr = &p1;
			bptr = &p2Board;
			moves_left_P2-=1;
		}
	}

	if (BattleCondition()==P1_WIN)
		std::cout<<p1.getName()<<" wins!!! number of moves: "<<nmoves<<std::endl;
	else 
		std::cout<<p2.getName()<<" wins!!! number of moves: "<<nmoves<<std::endl;
	
	time_t end;
	time(&end);
	std::cout<<difftime(end,timer)<<std::endl;

	std::cout<<"I hope you enjoyed the Battleship Battle. Bye!\n\n";
	return true;

}

void Battle::startBattle()
{
	std::string nameTemp;
	int autoTemp;


	p1.setPlayerNum(1);
	p2.setPlayerNum(2);

	std::cout<<"What is player 1's name (<=32 chars)?\n";
	p1.setName(getString(32));
	std::cout<<"Welcome, "<<p1.getName()<<"!"<<std::endl;

	std::cout<<"What is player 2's name (<=32 chars)?\n";
	p2.setName(getString(32));
	std::cout<<"Welcome, "<<p2.getName()<<"!"<<std::endl;

	std::cout<<"What type of player is "<<p1.getName()
					<<"? (enter 0 for non-auto, 1 for autoRandom, 2 for autoMCTS)\n";
	p1.setAuto(getInt(0,2));

	std::cout<<"What type of player is "<<p2.getName()
					<<"? (enter 0 for non-auto, 1 for autoRandom, 2 for autoMCTS)\n";
	p2.setAuto(getInt(0,2));


	if (p1.isPlayerAutomatic())
		initializeBoardAuto(p1Board, false);
	else
	{
		std::cout<<p1.getName()<<", how would you like your board to be set?"
						<<" (enter 0 for non-auto, 1 for auto)\n";
		autoTemp=getInt(0,1);
		if (autoTemp)
			initializeBoardAuto(p1Board, true);
		else initializeBoard(p1Board);

	}


	if (!p2.isPlayerAutomatic() && !p1.isPlayerAutomatic())
	{
		switchPlayers(p1.getName(), p2.getName());
	}

	if (p2.isPlayerAutomatic())
		initializeBoardAuto(p2Board, false);
	else
	{
		
		std::cout<<p2.getName()<<", how would you like your board to be set?"
						<<" (enter 0 for non-auto, 1 for auto)\n";
		autoTemp=getInt(0,1);
		if (autoTemp)
			initializeBoardAuto(p2Board, true);
		else initializeBoard(p2Board);

	}
	if (!p2.isPlayerAutomatic() && !p1.isPlayerAutomatic())
	{
		std::cout<<std::string(100,'\n');
	}

	return;
}


void Battle::printBattleState(Player p)
{
	std::cout<<p.getName()<<"'s Battle STATE:\n\n";
	if (p.getPlayerNum()==1)
	{
		std::cout<<"YOUR BOARD: \n";
		p1Board.printPublicBoard();
		std::cout<<"YOUR OPPONENT'S BOARD: \n";
		p2Board.printPrivateBoard();
		std::cout<<std::endl<<std::endl;
	}
	else
	{
		std::cout<<"YOUR BOARD: \n";
		p2Board.printPublicBoard();
		std::cout<<"YOUR OPPONENT'S BOARD: \n";
		p1Board.printPrivateBoard();
		std::cout<<std::endl<<std::endl;
	}


	return;
}


void Battle::initializeBoard(Board &b)
{
	int xEntry, yEntry, horizEntry, attemptCount;
	std::string entryTemp;
	for (int i=0; i<NUM_SHIPS; i++)
	{
		attemptCount=0;
		do 
		{
			b.printPublicBoard();
			if (attemptCount>0) 
				std::cout<<"INVALID ENTRY for that ship! Please try again. \n";

			std::cout<<"Please enter location [Letter][Number] for the "<<
						"top/left of your "<<SHIP_NAMES[i]<<" which is length "
						<<SHIP_LENGTHS[i]<<": \n";
			entryTemp=getSquare();		
			xEntry=static_cast<int>(entryTemp[0]);
			yEntry=static_cast<int>(entryTemp[1]);

			std::cout<<"Please enter 0 if the ship is oriented vertically, "
						<<"1 if it is oriented horizontally:\n";
			horizEntry=getInt(0,1);

			attemptCount++;
		} while (!b.placeShip(i, xEntry-LETTER_CHAR_OFFSET, 
						yEntry-NUMBER_CHAR_OFFSET, horizEntry));

	}

	std::cout<<"Your starting board: \n";
	b.printPublicBoard();

	return;
}


void Battle::initializeBoardAuto(Board &b, bool print)
{
	int xEntry, yEntry, horizEntry;

	for (int i=0; i<NUM_SHIPS; i++)
	{
		do 
		{
			xEntry=rand()%10;
			yEntry=rand()%10;
			horizEntry=rand()%2;
		} while (!b.placeShip(i, xEntry, yEntry, horizEntry));

	}

	if (print)
	{
		std::cout<<"Your starting board: \n";
		b.printPublicBoard();
	}


	return;
}

state Battle::BattleCondition()
{
	if (p1Board.getNumHits()==TOTAL_SHIP_SPACES)
		return P2_WIN;

	else if (p2Board.getNumHits()==TOTAL_SHIP_SPACES)
		return P1_WIN;

	else
		return UNFINISHED;
}


void Battle::getNextMove(Board &b)
{
	int attemptCount=0;
	int xEntry, yEntry;
	bool goodMove=false;
	std::string entryTemp;

	while (!goodMove)
	{
		if (attemptCount>0) 
			std::cout<<"That move has already been attempted. Try again. \n";

		std::cout<<"Please enter location [Letter][Number] of desired move:\n";
		entryTemp=getSquare();		
		xEntry=static_cast<int>(entryTemp[0]);
		yEntry=static_cast<int>(entryTemp[1]);

		if (b.getSpaceValue(xEntry-LETTER_CHAR_OFFSET, 
						yEntry-NUMBER_CHAR_OFFSET)!=isHIT
			&& b.getSpaceValue(xEntry-LETTER_CHAR_OFFSET, 
							yEntry-NUMBER_CHAR_OFFSET)!=isMISS)
		{
			b.recordHit(xEntry-LETTER_CHAR_OFFSET, yEntry-NUMBER_CHAR_OFFSET);
			goodMove=true;
		}
		attemptCount++;
	} 

	return;
}


// CHANGE  THISSS

void Battle::getNextMoveAuto2(Board &b)
{
	bool goodMove=false;
	int xEntry, yEntry;

	while (!goodMove)
	{
		xEntry=rand()%BOARD_DIM;
		yEntry=rand()%BOARD_DIM;

		if (b.getSpaceValue(xEntry, yEntry)!=isHIT
			&& b.getSpaceValue(xEntry, yEntry)!=isMISS)
		{
			b.recordHit(xEntry, yEntry);
			goodMove=true;
		}
	} 
	return;
}





void Battle::getNextMoveAuto(Board &b1, int remain)
{
	bool goodMove=false;
	int xEntry, yEntry;
	int N= b1.getNumHits();
	int R=BRANCH;
	if (remain>BOARD_DIM*BOARD_DIM-BOARD_DIM)
		{getNextMoveAuto2(b1);return ; }
		
	MCTS simulation = MCTS(R,N,remain);
	
	std::cout<<"Entering blackhole"<<std::endl;
	//std::cout.setstate(std::ios_base::failbit);
	simulation.SelectAction(b1, &xEntry, &yEntry);
	//std::cout.clear();
	
	b1.recordHit(yEntry, xEntry);
	
}

std::string Battle::getSquare()
{
	std::string retString;
	std::getline(std::cin, retString);
	bool isGoodInput=false;

	while (!isGoodInput)
	{
		if (retString.length()==2 && (retString[0]>=65 && retString[0]<=74) 
						&& (retString[1]>=48 && retString[1]<=57))
			isGoodInput=true;
		else
		{
			std::cout<<"Bad input! Please enter location [Letter][Number] of "
						<<"your desired move, with capital letters only:\n";
			std::getline(std::cin, retString);
		}
	}

	return retString;
}



void Battle::switchPlayers(std::string playerFrom, std::string playerTo)
{
	std::cout<<playerFrom<<", press ENTER to finish your turn!";
	std::cin.get();
	std::cout<<std::flush;
	std::cout<<std::string(100,'\n');
	std::cout<<playerTo<<", press ENTER to start your turn!";
	std::cin.get();
	std::cout<<std::flush;
	std::cout<<std::string(100,'\n');	

}
