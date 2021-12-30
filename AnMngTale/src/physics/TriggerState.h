#pragma once

#include "gui/backend/Callback.h"


struct TriggerState
{
	Callback onEnter;
	Callback onExit;

	bool used = false;
	bool inside = false;
	bool autoreset = false;

	void reset()
	{
		used = false;
	}
};