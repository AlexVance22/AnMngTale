#pragma once

#include "core/Scene.h"


class Basketball : public Scene
{
private:
	void loadFlags();
	void dumpFlags();

	void impl(const float deltaTime) override;

public:
	Basketball();
};