#pragma once

#include "core/Scene.h"


class Basketball : public Scene
{
private:
	void loadFlags();
	void manageFlags();
	void dumpFlags();

	void handleEvents() override;
	void impl(const float deltaTime) override;

public:
	Basketball();
};