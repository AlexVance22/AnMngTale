#pragma once

class MenuStack;


void pushQuitMenu(MenuStack* menus, bool playerLocked);

void pushOptionsMenu(MenuStack* menus);

void pushFileMenu(MenuStack* menus, bool* saveData);


void pushAgenda(MenuStack* menus);

void pushDialogue(MenuStack* menus, const std::vector<std::string>& text);