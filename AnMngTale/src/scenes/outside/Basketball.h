#pragma once

#include "core/Scene.h"


class Basketball : public Scene
{
private:
	void impl(const float deltaTime) override;

public:
	Basketball();
};