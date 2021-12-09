#pragma once

#include "core/Scene.h"


class MainMenu : public Scene
{
private:
	bool m_isActiveSave[3] = { 0 };

private:
	void impl(const float deltaTime) override;

public:
	MainMenu();
};