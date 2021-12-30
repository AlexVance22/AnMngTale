#pragma once

#include "core/Scene.h"


class Library : public Scene
{
private:
	void impl(const float deltaTime) override;

public:
	Library();
};