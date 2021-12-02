#include "PCH.h"
#include "MenuStack.h"

#include "Scene.h"


void MenuStack::push(const std::string& filepath, bool playerLocked)
{
	if (m_stack.empty())
		m_cachePlayerLocked = playerLocked;

	m_stack.emplace(filepath);
}

void MenuStack::pop()
{
	m_stack.pop();
}

bool MenuStack::empty() const
{
	return m_stack.empty();
}

Menu& MenuStack::top()
{
	return m_stack.top();
}


bool MenuStack::getCachedLock() const
{
	return m_cachePlayerLocked;
}