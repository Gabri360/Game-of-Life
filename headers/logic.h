#ifndef LOGIC_H
#define LOGIC_H

#include "const.h"

void init_state(int state[][HEIGHT]);

void copy_state(int state1[][HEIGHT], int state2[][HEIGHT]);

void game_step(int state[][HEIGHT], int next_state[][HEIGHT]);

#endif
