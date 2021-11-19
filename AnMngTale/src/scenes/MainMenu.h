#pragma once

#include "core/Scene.h"


class MainMenu : public Scene
{
private:
	void loadFlags();
	void dumpFlags();

	void handleEvents() override;
	void impl(const float deltaTime) override;

public:
	MainMenu();

private:
	bool m_isActiveSave[3] = { 0 };
};