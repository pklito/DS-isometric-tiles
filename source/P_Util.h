/*
 * util.h
 *
 *  Created on: Oct 24, 2023
 *      Author: nds
 */
#pragma once
#include <nds.h>

inline float clamp_float(float value, float min, float max);

inline int clamp(int value, int min, int max);

inline int coords(int x, int y, int w);

inline int sign(int x);

inline int round_float(float b);

int rng();

void set_seed(int new_seed);
