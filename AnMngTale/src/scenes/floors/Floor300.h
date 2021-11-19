#pragma once

#include "core/Scene.h"


class Floor300 : public Scene
{
private:
	void loadFlags();
	void manageFlags();
	void dumpFlags();

	void handleEvents() override;
	void impl(const float deltaTime) override;

public:
	Floor300();
};