#pragma once

#include "core/Scene.h"


class AulaYard : public Scene
{
private:
	void loadFlags();
	void manageFlags();
	void dumpFlags();

	void impl(const float deltaTime) override;

public:
	AulaYard();
};