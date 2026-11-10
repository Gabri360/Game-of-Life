#include "logic.h"
#include <math.h>

void init_state(int state[][HEIGHT])
{
	for(int i=0; i<WIDTH; i++) {
		for(int j=0;j<HEIGHT; j++) {
			state[i][j] = 0;
		}
	}
}

void copy_state(int state1[][HEIGHT], int state2[][HEIGHT])
{
	for(int i=0; i<WIDTH; i++) {
		for(int j=0;j<HEIGHT; j++) {
			state1[i][j] = state2[i][j];
		}
	}
}

void game_step(int state[][HEIGHT], int next_state[][HEIGHT])
{
	for(int i=0; i<WIDTH; i++) {
		int il = (i + WIDTH - 1) % WIDTH;
		int ir = (i + 1) % WIDTH;
		for(int j=0;j<HEIGHT; j++) {
			int jl = (j + HEIGHT - 1) % HEIGHT;
			int jr = (j + 1) % HEIGHT;
			int alive = state[il][jl] + state[il][j] + state[il][jr] + state[i][jl] + state[i][jr] + state[ir][jl] + state[ir][j] + state[ir][jr];
			if(state[i][j]) {
				if(alive<2 || alive>3) {
					next_state[i][j] = 0;
				}
				else {
					next_state[i][j] = 1;
				}
			}
			if(state[i][j] == 0){
				if(alive == 3) {
					next_state[i][j] = 1;
				}
				else {
					next_state[i][j] = 0;
				}
			}
		}
	}
}
