#pragma once

class Menu;


void pushGame(std::stack<Menu>* menus, uint8_t save);

void pushFileMenu(sf::RenderWindow* window, std::stack<Menu>* menus, bool* saveData);

void pushOptionsMenu(sf::RenderWindow* window, std::stack<Menu>* menus);

void popGame(std::stack<Menu>* menus);