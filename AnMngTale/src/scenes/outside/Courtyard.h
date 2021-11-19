#pragma once

#include "core/Scene.h"


class Courtyard : public Scene
{
private:
	void loadFlags();
	void dumpFlags();

	void handleEvents() override;
	void impl(const float deltaTime) override;

public:
	Courtyard();
};