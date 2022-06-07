#include "Battle.hpp"
#include "Player.hpp"
#include "utils.hpp"
#include <iostream>
#include<cilk/cilk.h>
#include<cilk/cilk_api.h>


int main(int argc, char* argv[])
{
	srand(time(NULL));

	bool continueGame=true;
	__cilkrts_set_param("nworkers", argv[1]);

	while (continueGame)
	{
		continueGame = printMenu();
	}
	return 0;

}
