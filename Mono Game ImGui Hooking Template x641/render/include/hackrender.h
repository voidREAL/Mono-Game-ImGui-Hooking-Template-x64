#pragma once
#include "../include.h"

struct MiscState {
	bool isBunnyHopOn;
};

class HackRender {
public:
	struct State {
		MiscState misc;
	}state;
	static void render();

	void miscRender();
	void debugRender();
};

extern HackRender hackRender;