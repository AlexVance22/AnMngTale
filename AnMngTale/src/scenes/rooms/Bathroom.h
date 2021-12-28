#pragma once

#include "core/Scene.h"


class Bathroom : public Scene
{
private:
	void impl(const float deltaTime) override;

public:
	Bathroom();
};