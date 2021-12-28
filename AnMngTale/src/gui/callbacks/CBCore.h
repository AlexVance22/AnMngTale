#pragma once

class MenuStack;


void pushQuitMenu(MenuStack* menus, bool playerLocked);

void pushOptionsMenu(MenuStack* menus);

void pushFileMenu(MenuStack* menus, bool load, bool* saveData);


void pushAgenda(MenuStack* menus, bool playerLocked);