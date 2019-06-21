#pragma once

#include <math.h>

public enum class NOTE {
	C0 = 0, C0_S, D0, D0_S, E0, F0, F0_S, G0, G0_S, A0, A0_S, B0,
	C1, C1_S, D1, D1_S, E1, F1, F1_S, G1, G1_S, A1, A1_S, B1,
	C2, C2_S, D2, D2_S, E2, F2, F2_S, G2, G2_S, A2, A2_S, B2,
	C3, C3_S, D3, D3_S, E3, F3, F3_S, G3, G3_S, A3, A3_S, B3,
	C4, C4_S, D4, D4_S, E4, F4, F4_S, G4, G4_S, A4, A4_S, B4,
	C5, C5_S, D5, D5_S, E5, F5, F5_S, G5, G5_S, A5, A5_S, B5,
	C6, C6_S, D6, D6_S, E6, F6, F6_S, G6, G6_S, A6, A6_S, B6,
	C7, C7_S, D7, D7_S, E7, F7, F7_S, G7, G7_S, A7, A7_S, B7,
	C8, C8_S, D8, D8_S, E8, F8, F8_S, G8, G8_S, A8, A8_S, B8
};

public enum class WAVE_TYPE {
	SINE = 0,
	SAW,
	TRIANGLE,
	SQUARE,
	DRUM_KICK,
	DRUM_SNARE,
	REESE_BASS,
	THEREMIN,
	CELLO,
	TEST
};

const int MAGIC_IDENT = 0xacedfa;
const float VERSION = 1.20001;

double translateNote(NOTE note);

float map(float value, float min1, float max1, float min2, float max2);
