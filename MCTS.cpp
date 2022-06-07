#include "Battle.hpp"
#include "MCTS.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <algorithm>
#include<cilk/cilk.h>
#include<cilk/cilk_api.h>
#include <random>
#include <tbb/mutex.h>

void MCTS::findMax(int* bx, int* by)
{
    int minElement = INT_MAX;
    for (int i = 0; i < BOARD_DIM; i++) {
        for (int j = 0; j < BOARD_DIM; j++) {
            if (Values[i][j] < minElement) {
                minElement = Values[i][j];
                *bx = i;
                *by= j;
            }
        }
    }
    std::cout<<*bx<<"  "<<*by<<" "<<minElement<<std::endl;

}

double doubleRand(double min, double max) {
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

void MCTS::findMaxArray(int* bx, int* by, double Array[BOARD_DIM][BOARD_DIM])
{
    double maxElement = 0;
    for (int i = 0; i < BOARD_DIM; i++) {
        for (int j = 0; j < BOARD_DIM; j++) {
            if (Array[i][j] > maxElement) {
                maxElement = Array[i][j];
                *bx = i;
                *by= j;
            }
        }
    }

}


void MCTS::SelectAction(Board b, int* x, int* y)
{
	std::cout<<"Entering simulator"<<std::endl;
	Simulator(b);	
	std::cout<<"Computing best move"<<std::endl;
	findMax(x, y);
}


void MCTS::Simulator(Board b)
{
	time_t timer;
	time(&timer);
	int xEntry, yEntry;
	int numfinish=TOTAL_SHIP_SPACES-NumHits;
	double Probabilities[BOARD_DIM][BOARD_DIM]={0};
	int seen[BOARD_DIM][BOARD_DIM]={0};
	bool goodmove=false;
	Foo coor=b.ComputeProbability(numfinish,r, rem, Probabilities);
	xEntry=coor.value1;
	yEntry=coor.value2;
	coor.xValues.clear();
	coor.yValues.clear();
	coor.ProbValues.clear();
	std::cout<<"Computed Probabilities round 1 "<<coor.val<<std::endl;
	
	if (coor.val>=1)
		{Values[xEntry][yEntry]=0; return;}
	
	if (coor.val<=0)
	{
		while (!goodmove)
		{
			xEntry=rand()%BOARD_DIM;
			yEntry=rand()%BOARD_DIM;

			if (b.getSpaceValue(xEntry, yEntry)!=isHIT
				&& b.getSpaceValue(xEntry, yEntry)!=isMISS)
			{
				Values[xEntry][yEntry]=0;
				goodmove=true;
			}
		} 
	return;
	}
	
	if (r>rem)
		r=rem;
		
	int* xEntries = new int[r];
	int* yEntries = new int[r];
	Board b_copy[r];
	int useless=0;
	
	double Probabilities_copy[BOARD_DIM][BOARD_DIM]={0};
	
	for (int i=0;i<r;i++)
	{
		while (!goodmove)
		{
			findMaxArray(&xEntry,&yEntry, Probabilities);
			std::cout<<"x: "<<xEntry<<" y: "<<yEntry<<" "<<b.getSpaceValue(yEntry, xEntry)<<Probabilities[xEntry][yEntry]<<" "<<numfinish<<std::endl;
			if (b.getSpaceValue(yEntry, xEntry)!=isHIT && b.getSpaceValue(yEntry, xEntry)!=isMISS&& seen[xEntry][yEntry]==0)
			{
				goodmove=true;
				seen[xEntry][yEntry]=1;
				if(Probabilities[xEntry][yEntry]>0)
					useless=1;
				
				Probabilities_copy[xEntry][yEntry]=Probabilities[xEntry][yEntry];
				Probabilities[xEntry][yEntry]=-1;
			}
			else if (Probabilities[xEntry][yEntry]==-1||Probabilities[xEntry][yEntry]==0)
			{
				while (!goodmove)
				{
					xEntry=rand()%BOARD_DIM;
					yEntry=rand()%BOARD_DIM;
					if (b.getSpaceValue(yEntry, xEntry)!=isHIT && b.getSpaceValue(yEntry, xEntry)!=isMISS&& seen[xEntry][yEntry]==0)
					{
						goodmove=true;
						seen[xEntry][yEntry]=1;
						Probabilities_copy[xEntry][yEntry]=0;
						
					}
					
				}
				
				
			}
			
			
		}
		b_copy[i]=Board(b);
		xEntries[i]=xEntry;
		yEntries[i]=yEntry;
		goodmove=false;
	}

	//#pragma grainsize = 5;
	cilk_for (int i=0; i<r; i++)
	{
	
		std::cout<<"Inside first while loop on "<<i<<std::endl;
		double p, temp;
		int dummy=BOARD_DIM*BOARD_DIM;
		p=Probabilities_copy[xEntries[i]][yEntries[i]];
		temp=(double) rand_r(seeds[i])/ (double) RAND_MAX;
		//temp=0.01;
		//std::cout<<"x: "<<xEntries[i]<<" y: "<<yEntries[i]<<std::endl;
		//std::cout<<"Mainloop, temp: "<<temp<<" p: "<<p<<std::endl;
		
		if (p==0&&useless==0)
			continue;
		if (temp<p)
		{
			b_copy[i].simulate(yEntries[i], xEntries[i], isHIT);
			
			if (numfinish-1>0)
			{
				//time_t timer1;
				//time(&timer1);
				//std::cout<<"Got a hit, round 1.. entering rollout "<<rem<<std::endl;
				dummy=Rollout(b_copy[i], numfinish-1, rem-1, r, 0, i);
				
				//time_t end1;
				//time(&end1);
				//std::cout<<"Rollout time: "<<difftime(end1,timer1)<<std::endl;

			}
		
		}
		else
		{
			//std::cout<<"Got a miss, round 1.. entering rollout "<<rem<<std::endl;
			b_copy[i].simulate(yEntries[i], xEntries[i], isMISS);
			//time_t timer2;
			//time(&timer2);
			dummy=Rollout(b_copy[i], numfinish, rem-1, r, 0,i );
			//time_t end2;
			//time(&end2);
			//std::cout<<"Rollout time: "<<difftime(end2,timer2)<<std::endl;

		}

		Values[xEntries[i]][yEntries[i]]=1+dummy;
		
		

		std::cout<<"Exit first while loop on "<<i<<" time taken: "<<std::endl;
	}
	delete [] xEntries;
	delete [] yEntries;
	coor.xValues.clear();
	coor.yValues.clear();
	coor.ProbValues.clear();
	
	time_t end;
	time(&end);
	std::cout<<difftime(end,timer)<<std::endl;

}


int MCTS::Rollout(Board b, int numfinish, int remain, int rn, int num, int see)
{
	//std::cout<<"In rollout "<<numfinish<<" "<<remain<<std::endl;
	using namespace std;
	//std::cout<<"count: "<<++count<<std::endl;
	if (numfinish<=0)
	{
		//std::cout<<" remain: "<<remain<<" numfinish: "<<numfinish<<std::endl;
		return 0;
	}
	if (remain<numfinish)
	{
		//std::cout<<" remain: "<<remain<<" numfinish: "<<numfinish<<std::endl;
		return BOARD_DIM*BOARD_DIM;
	}
	//double Probabilities[BOARD_DIM][BOARD_DIM]={0};
	int xEntry, yEntry;
	
	if (rn<2)
	{
		rn=2;
		if (num>SLOWDECAY)
			rn=1;
	}
	if (remain<rn)
		rn=remain;
	
	Foo coor=b.ComputeProbability2( numfinish,rn, remain, seeds[see]);
	xEntry=coor.value1;
	yEntry=coor.value2;
	
	//std::cout<<"Probability"<<xEntry<<yEntry<<std::endl;
	

	double p,temp;
	bool goodmove=false;

	
	if(coor.val>=1)
		goodmove=true;
	
	
	
	int rn_copy=rn;
	
	if (goodmove)
	{
		rn_copy=1;
		coor.xValues[0]=xEntry;
		coor.yValues[0]=xEntry;
		coor.ProbValues[0]=coor.val;
		
	}
	

	//std::cout<<" In rollout "<<remain<<std::endl;
	
	
	double* depth = new double[rn_copy];
	int min_depth=BOARD_DIM*BOARD_DIM;
	float avg=0;
	int flag=0;

	for (int i=0;i<rn_copy;i++)
	{
		Board b_copy = Board(b);
		xEntry=coor.xValues[i];
		yEntry=coor.yValues[i];
		p=coor.ProbValues[i];
		//temp=0.1;
		temp=(double) rand_r(seeds[see])/ (double) RAND_MAX;
		//std::cout<<" temp: "<<temp<<" p: "<<p<<std::endl;
		
		//std::cout<<" x "<<xEntry<<" y "<<yEntry<<std::endl;
		if (temp<p)
		{
			b_copy.simulate(yEntry, xEntry, isHIT);
			if (numfinish-1==0)
			{
				//std::cout<<" remain: "<<remain<<" numfinish: "<<numfinish<<std::endl;
				flag=1;
			}
			else
				depth[i]=1+Rollout(b_copy, numfinish-1, remain-1, (int)rn/DECAY, num+1, see);
		}
		else
		{
			b_copy.simulate(yEntry, xEntry, isMISS);
			depth[i]=1+Rollout(b_copy, numfinish, remain-1, (int)rn/DECAY, num+1, see);
		}
		
	}

	if (flag==1)
		return 1;
	for (int i=0;i<rn_copy;i++)
	{
		if(depth[i]<min_depth)
			min_depth=depth[i];
		avg+=depth[i];
	}
	avg/=rn_copy;
	
	coor.xValues.clear();
	coor.yValues.clear();
	coor.ProbValues.clear();
	//delete [] depth;
	//delete [] xEntries;
	//delete [] yEntries;
	
	//std::cout<<" remain: "<<remain<<" numfinish: "<<numfinish<<" mindepth: "<<min_depth<<std::endl;
	return (int) avg;
	
		
}

