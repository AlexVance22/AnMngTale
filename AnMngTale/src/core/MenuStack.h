#pragma once

#include "Menu.h"


class MenuStack
{
private:
	TextureMap m_sharedTextures;
	FontMap m_sharedFonts;

	std::stack<Menu> m_stack;

	bool m_cachePlayerLocked = false;

public:
	MenuStack() = default;

	void push(const std::string& filepath, bool playerLocked = false);
	void pop();
	bool empty() const;
	Menu& top();

	bool getCachedLock() const;
};