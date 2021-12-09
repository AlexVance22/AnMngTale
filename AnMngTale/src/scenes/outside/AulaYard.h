#pragma once

#include "core/Scene.h"


class AulaYard : public Scene
{
private:
	void impl(const float deltaTime) override;

public:
	AulaYard();
};