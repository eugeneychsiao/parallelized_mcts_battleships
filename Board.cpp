#include "Board.hpp"
#include <iostream>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <vector>


// default board constructor builds a 2D array filled with water, 
// and fills a vector with the standard ship objects
Board::Board()
{
	for (int i=0; i<BOARD_DIM; i++)
		for (int j=0; j<BOARD_DIM; j++)
			gameBoard[i][j]=isWATER;

	for (int i=0; i<NUM_SHIPS; i++)
		shipVec.push_back(Ship(SHIP_LENGTHS[i], SHIP_NAMES[i]));
}

// copy constructor - copies old board square by square
Board::Board(const Board &oldBoard)
{
	for (int i=0; i<BOARD_DIM; i++)
		for (int j=0; j<BOARD_DIM; j++)
			gameBoard[i][j]=oldBoard.gameBoard[i][j];
	shipVec = oldBoard.shipVec;
}

// copy assignment operator - copies old board square by square
Board& Board::operator=(const Board &right)
{
	if (this!=&right)
	{
		for (int i=0; i<BOARD_DIM; i++)
			for (int j=0; j<BOARD_DIM; j++)
				gameBoard[i][j]=right.gameBoard[i][j];
		shipVec = right.shipVec;
	}

	return *this;

}

Foo Board::ComputeProbability( int numfinish, int r, int remain, double value[BOARD_DIM][BOARD_DIM])
{
	double bonus;
	double ship;
	double emptynbr=0;
	double exception[BOARD_DIM][BOARD_DIM]={0};
	int x =-1;
	int y=-1;
	for (int i=0; i<BOARD_DIM; i++)
	{
		for (int j=0; j<BOARD_DIM; j++)
		{
			if (gameBoard[i][j]==isHIT)
			{
				bonus=0;
				ship=0;
				if (i>=1)
				{
					if (gameBoard[i-1][j]!=isMISS&&gameBoard[i-1][j]!=isHIT){bonus+=1;}
					
					if (gameBoard[i-1][j]==isHIT)
					{ship=1;
						if (i>=2)
						{
							if (gameBoard[i-2][j]==isHIT)
								ship=0;
						}
						if (j>=1)
						{
							if (gameBoard[i-1][j-1]==isHIT)
								ship=0;
						}
						if (j<BOARD_DIM-1)
						{
							if (gameBoard[i-1][j+1]==isHIT)
								ship=0;
						}
						
						
					}
					
				}
				if (j>=1)
				{
					if (gameBoard[i][j-1]!=isMISS&&gameBoard[i][j-1]!=isHIT)
					{
						bonus+=1;
					}
					if (gameBoard[i][j-1]==isHIT)
						{ship=1;
						if (i>=1)
						{
							if (gameBoard[i-1][j-1]==isHIT)
								ship=0;
						}
						if (j>=2)
						{
							if (gameBoard[i][j-2]==isHIT)
								ship=0;
						}
						if (i<BOARD_DIM-1)
						{
							if (gameBoard[i+1][j+1]==isHIT)
								ship=0;
						}
						
						
					}
				}
				if (i<BOARD_DIM-1)
				{
					if (gameBoard[i+1][j]!=isMISS&&gameBoard[i+1][j]!=isHIT)
					{
						bonus+=1;
					}
					if (gameBoard[i+1][j]==isHIT)
					{ship=1;
						if (i<BOARD_DIM-2)
						{
							if (gameBoard[i+2][j]==isHIT)
								ship=0;
						}
						if (j>=1)
						{
							if (gameBoard[i+1][j-1]==isHIT)
								ship=0;
						}
						if (j<BOARD_DIM-2)
						{
							if (gameBoard[i+1][j+2]==isHIT)
								ship=0;
						}
						
						
					}
				}
				if (j<BOARD_DIM-1)
				{
					if (gameBoard[i][j+1]!=isMISS&&gameBoard[i][j+1]!=isHIT)
					{
						bonus+=1;
					}
					if (gameBoard[i][j+1]==isHIT)
					{ship=1;
						if (i<BOARD_DIM-1)
						{
							if (gameBoard[i+1][j+1]==isHIT)
								ship=0;
						}
						if (i>=1)
						{
							if (gameBoard[i-1][j+1]==isHIT)
								ship=0;
						}
						if (j<BOARD_DIM-2)
						{
							if (gameBoard[i][j+2]==isHIT)
								ship=0;
						}
						
						
					}
				}
				
				if (ship==1)
					continue;
				else if (bonus==0)
					continue;
				else if (bonus==1||numfinish==1)
				{
					if (i>=1){
						value[i-1][j]+=numfinish; exception[i-1][j]=1;
						}				
					if (j>=1){
						value[i][j-1]+=numfinish;	exception[i][j-1]=1;}
					if (i<BOARD_DIM-1){
						value[i+1][j]+=numfinish;	exception[i+1][j]=1;}
					if (j<BOARD_DIM-1){
						value[i][j+1]+=numfinish;	exception[i][j+1]=1;}
										
				}
				else
				{
					if (i>=1)
						value[i-1][j]+=numfinish/bonus;					
					if (j>=1)
						value[i][j-1]+=numfinish/bonus;	
					if (i<BOARD_DIM-1)
						value[i+1][j]+=numfinish/bonus;	
					if (j<BOARD_DIM-1)
						value[i][j+1]+=numfinish/bonus;	
				}
				
				
			}
			
			else if (gameBoard[i][j]==isMISS)
				continue;
				
			else
			{
				if (numfinish==1)
					break;
				if (i>=1)
					value[i-1][j]+=(double) numfinish/((double) remain);					
				if (j>=1)
					value[i][j-1]+=(double) numfinish/((double) remain);		
				if (i<BOARD_DIM-1)
					value[i+1][j]+=(double) numfinish/((double) remain);		
				if (j<BOARD_DIM-1)
					value[i][j+1]+=(double) numfinish/((double) remain);	
			}
			
			if (i>=1)
				if (gameBoard[i-1][j]!=isMISS||gameBoard[i-1][j]!=isHIT)
					emptynbr+=1;
			if (j>=1)
				if (gameBoard[i][j-1]!=isMISS||gameBoard[i][j-1]!=isHIT)
					emptynbr+=1;
			if (i<BOARD_DIM-1)
				if (gameBoard[i+1][j]!=isMISS||gameBoard[i+1][j]!=isHIT)
					emptynbr+=1;
			if (j<BOARD_DIM-1)
				if (gameBoard[i][j+1]!=isMISS||gameBoard[i][j+1]!=isHIT)
					emptynbr+=1;
					
		}
		
	}
	
	emptynbr/=BOARD_DIM*BOARD_DIM;

	double max=0;
	
	for (int i=0; i<BOARD_DIM; i++)
	{
		for (int j=0; j<BOARD_DIM; j++)
		{
			if (gameBoard[i][j]==isHIT||gameBoard[i][j]==isMISS)
				value[i][j]=0;
			else if (exception[i][j]!=1)
			{
				value[i][j]/=emptynbr;
			}

			if (value[i][j]>max)
				{max=value[i][j];x=i;y=j;}
		}
		
	}
	using namespace std;
	
	
	Foo result;
	result.value1=x;
	result.value2=y;
	result.val=max;
	return result;
}

class rand_x { 
    unsigned int* seed = new unsigned int;
public:
    rand_x(unsigned int init) {*seed=init;}

    /*int operator()(int limit) {
        int divisor = RAND_MAX/(limit+1);
        int retval;

        do { 
            retval = rand_r(seed) / divisor;
            std::cout<<"Return Value"<<retval<<std::endl;
        } while (retval > limit);

        return retval;
    }  */
     int operator()(int limit) {return rand_r(seed) % limit;}   
};



Foo Board::ComputeProbability2( int numfinish, int r, int remain, unsigned int* see)
{
	double bonus;
	double ship;
	double emptynbr=0;
	double value[BOARD_DIM][BOARD_DIM]={0};
	double exception[BOARD_DIM][BOARD_DIM]={0};
	int x =-1;
	int y=-1;
	for (int i=0; i<BOARD_DIM; i++)
	{
		for (int j=0; j<BOARD_DIM; j++)
		{
			if (gameBoard[i][j]==isHIT)
			{
				bonus=0;
				ship=0;
				if (i>=1)
				{
					if (gameBoard[i-1][j]!=isMISS){bonus+=1;}
					
					if (gameBoard[i-1][j]==isHIT){ship=1;}
					
				}
				if (j>=1)
				{
					if (gameBoard[i][j-1]!=isMISS)
					{
						bonus+=1;
					}
					if (gameBoard[i][j-1]==isHIT)
						{ship=1; }
				}
				if (i<BOARD_DIM-1)
				{
					if (gameBoard[i+1][j]!=isMISS)
					{
						bonus+=1;
					}
					if (gameBoard[i+1][j]==isHIT)
						{ship=1;}
				}
				if (j<BOARD_DIM-1)
				{
					if (gameBoard[i][j+1]!=isMISS)
					{
						bonus+=1;
					}
					if (gameBoard[i][j+1]==isHIT)
						{ship=1;}
				}
				
				if (ship==1&&numfinish>1)
					continue;
				else if (bonus==0)
					continue;
				else if (bonus==1||numfinish==1)
				{
					if (i>=1){
						value[i-1][j]+=numfinish/bonus; exception[i-1][j]=1;
						}				
					if (j>=1){
						value[i][j-1]+=numfinish/bonus;	exception[i][j-1]=1;}
					if (i<BOARD_DIM-1){
						value[i+1][j]+=numfinish/bonus;	exception[i+1][j]=1;}
					if (j<BOARD_DIM-1){
						value[i][j+1]+=numfinish/bonus;	exception[i][j+1]=1;}
										
				}
				else
				{
					if (i>=1)
						value[i-1][j]+=numfinish/bonus;					
					if (j>=1)
						value[i][j-1]+=numfinish/bonus;	
					if (i<BOARD_DIM-1)
						value[i+1][j]+=numfinish/bonus;	
					if (j<BOARD_DIM-1)
						value[i][j+1]+=numfinish/bonus;	
				}
				
				
			}
			
			else if (gameBoard[i][j]==isMISS)
				continue;
				
			else
			{
				if (numfinish==1)
					break;
				if (i>=1)
					value[i-1][j]+=(double) numfinish/((double) remain);					
				if (j>=1)
					value[i][j-1]+=(double) numfinish/((double) remain);		
				if (i<BOARD_DIM-1)
					value[i+1][j]+=(double) numfinish/((double) remain);		
				if (j<BOARD_DIM-1)
					value[i][j+1]+=(double) numfinish/((double) remain);	
			}
			
			if (i>=1)
				if (gameBoard[i-1][j]!=isMISS||gameBoard[i-1][j]!=isHIT)
					emptynbr+=1;
			if (j>=1)
				if (gameBoard[i][j-1]!=isMISS||gameBoard[i][j-1]!=isHIT)
					emptynbr+=1;
			if (i<BOARD_DIM-1)
				if (gameBoard[i+1][j]!=isMISS||gameBoard[i+1][j]!=isHIT)
					emptynbr+=1;
			if (j<BOARD_DIM-1)
				if (gameBoard[i][j+1]!=isMISS||gameBoard[i][j+1]!=isHIT)
					emptynbr+=1;
					
		}
		
	}
	
	emptynbr/=BOARD_DIM*BOARD_DIM;
	
	double max=0;
	int* xAvailabilities= new int[remain];
	int* yAvailabilities= new int[remain];
	int* indicesShuffled= new int[remain];
	int k=0;
	
	for (int i=0; i<BOARD_DIM; i++)
	{
		for (int j=0; j<BOARD_DIM; j++)
		{
			if (gameBoard[i][j]==isHIT||gameBoard[i][j]==isMISS)
				value[i][j]=0;
			else if (exception[i][j]!=1)
			{
				value[i][j]/=emptynbr;
			}
			if (gameBoard[i][j]!=isHIT&&gameBoard[i][j]!=isMISS&&k<remain)
			{
				xAvailabilities[k]=i;
				yAvailabilities[k]=j;
				indicesShuffled[k]=k;
				k++;
			}

			if (value[i][j]>max)
				{max=value[i][j];x=i;y=j;}
		}
		
	}
	using namespace std;
	
	//srand(unsigned(time(0)));
	//std::cout<<"See:"<<*see<<std::endl;
	std::random_shuffle(indicesShuffled,indicesShuffled+remain, rand_x(*see));
	
	Foo result;
	result.value1=x;
	result.value2=y;
	result.val=max;
	//std::cout<<"Probability "<<x<<y<<" "<<k<<std::endl;
	for (int i=0;i<r;i++)
	{	
		//std::cout<<"x value "<<xAvailabilities[indicesShuffled[i]]<<yAvailabilities[indicesShuffled[i]]<<" "<<indicesShuffled[i]<<std::endl;
		result.xValues.push_back(xAvailabilities[indicesShuffled[i]]);
		result.yValues.push_back(yAvailabilities[indicesShuffled[i]]);
		//std::cout<<"indices pushed "<<std::endl;
		result.ProbValues.push_back(value[xAvailabilities[indicesShuffled[i]]][yAvailabilities[indicesShuffled[i]]]);
		//std::cout<<"done"<<std::endl;
	}
	//std::cout<<"happy? "<<x<<y<<std::endl;
	delete [] xAvailabilities;
	delete [] yAvailabilities;
	delete [] indicesShuffled;
	//std::cout<<"deleted "<<std::endl;
	return result;
}
/*
void Board::ComputeProbability(double Probabilities[BOARD_DIM][BOARD_DIM], int numfinish, int remain)
{
	double val[BOARD_DIM][BOARD_DIM]={0};
	double denom=1;
	
	for (int i=0; i<BOARD_DIM; i++)
	{
		for (int j=0; j<BOARD_DIM; j++)
		{
			if (gameBoard[i][j]==isHIT||gameBoard[i][j]==isMISS)
				continue;
			if (i>=1)
			{
				if (gameBoard[i-1][j]==isHIT)
				{
					val[i][j]+=4;
				}
				else if (gameBoard[i-1][j]==isMISS)
					val[i][j]+=0;
				else
				{
					val[i][j]+=1;
				}
			}
			if (j>=1)
			{
				if (gameBoard[i][j-1]==isHIT)
				{
					val[i][j]+=4;
				}
				else if (gameBoard[i][j-1]==isMISS)
					val[i][j]+=0;
				else
				{
					val[i][j]+=1;
				}
			}
			if (i<BOARD_DIM-1)
			{
				if (gameBoard[i+1][j]==isHIT)
				{
					val[i][j]+=4;
				}
				else if (gameBoard[i+1][j]==isMISS)
					val[i][j]+=0;
				else
				{
					val[i][j]+=1;
				}
			}
			if (j<BOARD_DIM-1)
			{
				if (gameBoard[i][j+1]==isHIT)
				{
					val[i][j]+=4;
				}
				else if (gameBoard[i][j+1]==isMISS)
					val[i][j]+=0;
				else
				{
					val[i][j]+=1;
				}
			}
			
			denom+=val[i][j];
			
		}
		
	}
	
	for (int i=0; i<BOARD_DIM; i++)
	{
		for (int j=0; j<BOARD_DIM; j++)
		{
			Probabilities[i][j]=val[i][j]*numfinish/denom;
		}
		
	}
	
	
}*/

// return the number of hits on the board at the moment
int Board::getNumHits()
{
	int count=0;

	for (int i=0; i<BOARD_DIM; i++)
		for (int j=0; j<BOARD_DIM; j++)
			if (gameBoard[i][j]==isHIT)
				count++;

	return count;
}

// method to print the private version of the board 
// (player can only see hits/misses) 
void Board::printPrivateBoard()
{
	std::cout<<"  A B C D E F G H I J\n";
	for (int i=0; i<BOARD_DIM; i++)
	{
		std::cout<<i<<" ";
		for (int j=0; j<BOARD_DIM; j++)
		{
			if (gameBoard[i][j]==isHIT || gameBoard[i][j]==isMISS)
				std::cout<<gameBoard[i][j]<<" ";
			else // obfuscate non-hit/miss entries
				std::cout<<isUNKNOWN<<" ";
		}	
		std::cout<<std::endl;
	}
}

// method to print the board that the player can see completely 
// (usually, the player's own board)
void Board::printPublicBoard()
{
	std::cout<<"  A B C D E F G H I J\n";
	for (int i=0; i<BOARD_DIM; i++)
	{
		std::cout<<i<<" ";
		for (int j=0; j<BOARD_DIM; j++)
		{
			std::cout<<gameBoard[i][j]<<" ";
		}	
		std::cout<<std::endl;
	}

}

//return the value of a space
char Board::getSpaceValue(int x, int y)
{
	return gameBoard[y][x];
}


bool Board::recordHit(int x, int y)
{
	for (int i=0; i<NUM_SHIPS; i++)
	{
		if (shipVec[i].recordHit(x, y))
		{
			gameBoard[y][x]=isHIT; //record the hit on the board
			//tell the user that they sunk a ship
			if (shipVec[i].isShipSunk()) 
				std::cout<<"You sunk the "<<shipVec[i].getName()<<"!\n";
			return true;
		}
	}
	gameBoard[y][x]=isMISS;
	return false;
}

void Board::simulate(int x, int y, char val)
{
	gameBoard[y][x]=val;
}



// function to place ship on board
bool Board::placeShip(int shipNum, int x, int y, bool isHorizontal)
{
	//if x or y is out of bounds, return false
	if (x>=BOARD_DIM || y>=BOARD_DIM)
		return false;

	//if ship has already been placed, return false
	if (shipVec[shipNum].getX()>=0 && shipVec[shipNum].getY()>=0)
		return false;

	//loop through the positions required for the ship
	for (int i=0; i<shipVec[shipNum].getSize(); i++)
	{
		//if any of the desired spaces are filled, return false
		if ((isHorizontal && gameBoard[y][x+i]!=isWATER) || 
							(!isHorizontal && gameBoard[y+i][x]!=isWATER))
			return false;
		//if any of the desired spaces are out of bounds, return false
		if ((isHorizontal && (x+i)>=BOARD_DIM) || 
							(!isHorizontal && (y+i)>=BOARD_DIM))
			return false;
	}

	//if the for loop finishes, and all the positions are empty, 
	//1. place the Ship in the desired position on the board
	for (int i=0; i<shipVec[shipNum].getSize(); i++)
	{
		if (isHorizontal)
			gameBoard[y][x+i]=isSAFESHIP;
		else 
			gameBoard[y+i][x]=isSAFESHIP;
	}

	//2. set the x/y parameters for the Ship object 
	shipVec[shipNum].setPosition(x, y, isHorizontal);

	//... and return true
	return true;
}
