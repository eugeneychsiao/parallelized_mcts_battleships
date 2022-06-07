#include <string>
#include <math.h>  
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// game play constants (standard game of battleship)
const int NUM_SHIPS = 3;
const int BOARD_DIM = 9;
const int BOARD_SPACES = 81;
const int BRANCH=40;
const double DECAY=2;
const long SLOWDECAY=std::pow(2, 2); 
const int SHIP_LENGTHS[NUM_SHIPS] = {2,2,2};
const int TOTAL_SHIP_SPACES = 6;
const std::string SHIP_NAMES[NUM_SHIPS] = 
			{"Carrier","Battleship","Destroyer"};


const char isHIT = 'X';
const char isMISS = 'O';
const char isWATER = 'w';
const char isSAFESHIP = '_';
const char isUNKNOWN = '?';


const int LETTER_CHAR_OFFSET=65;
const int NUMBER_CHAR_OFFSET=48;


const int ASCII_INT_MIN=48;
const int ASCII_INT_MAX=57;




#endif
