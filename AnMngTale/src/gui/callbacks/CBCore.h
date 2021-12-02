#pragma once

class Menu;


void pushQuitMenu(sf::RenderWindow* window, std::stack<Menu>* menus);

void pushOptionsMenu(sf::RenderWindow* window, std::stack<Menu>* menus);

void pushFileMenu(sf::RenderWindow* window, std::stack<Menu>* menus, bool* saveData);


void pushAgenda(sf::RenderWindow* window, std::stack<Menu>* menus);

void pushDialogue(sf::RenderWindow* window, std::stack<Menu>* menus, const std::vector<std::string>& text);