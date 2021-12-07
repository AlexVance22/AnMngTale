#pragma once

#include "Entity.h"


class Player : public Entity
{
private:

public:
	Player();

	void update(const float deltaTime) override;
};