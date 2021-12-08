#pragma once

#include "core/Scene.h"


class Courtyard : public Scene
{
private:
	void impl(const float deltaTime) override;

public:
	Courtyard();
};