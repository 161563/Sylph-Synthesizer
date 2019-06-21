#include "Music.h"

double translateNote(NOTE note) {
	return 440. * pow(pow(2., 1. / 12.), (double)note-57);
}

float map(float value, float min1, float max1, float min2, float max2)
{
	return min2 + (value - min1)*(max2 - min2) / (max1 - min1);
}
