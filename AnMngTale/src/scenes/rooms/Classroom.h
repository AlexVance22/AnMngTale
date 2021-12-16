#pragma once

#include "core/Scene.h"


class Classroom : public Scene
{
private:
	void impl(const float deltaTime) override;

public:
	Classroom();
};