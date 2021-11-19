#pragma once

#include "Entity.h"


class Player : public Entity
{
private:

public:
	void update(const float deltaTime) override;
};