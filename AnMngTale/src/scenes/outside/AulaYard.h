#pragma once

#include "core/Scene.h"


class AulaYard : public Scene
{
private:
	void loadFlags();
	void dumpFlags();

	void impl(const float deltaTime) override;

public:
	AulaYard();
};