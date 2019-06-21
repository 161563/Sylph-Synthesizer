#include "Music.h"

double getNote(NOTE note) {
	return 440. * pow(pow(2., 1. / 12.), (double)note);
}