#include "PCH.h"
#include "CBCore.h"

#include "core/Menu.h"
#include "gui/gui.h"

#include "global/AudioManager.h"
#include "global/Data.h"

#include "scenes/outside/Courtyard.h"
#include "scenes/MainMenu.h"


void pushGame(std::stack<Menu>* menus, uint8_t save)
{
	menus->top().setNextScene<Courtyard>();
}

void pushFileMenu(sf::RenderWindow* window, std::stack<Menu>* menus, bool* saveData)
{
	menus->emplace(window, menus, "res/menus/titlescreen/files.json");

	auto f1 = menus->top().getWidget<gui::Button>("0");
	auto f2 = menus->top().getWidget<gui::Button>("1");
	auto f3 = menus->top().getWidget<gui::Button>("2");

	f1->onClick.bind(&pushGame, menus, 0);
	f2->onClick.bind(&pushGame, menus, 1);
	f3->onClick.bind(&pushGame, menus, 2);

	if (saveData)
	{
		f1->setEnabled(saveData[0]);
		f2->setEnabled(saveData[1]);
		f3->setEnabled(saveData[2]);
	}
}

void pushOptionsMenu(sf::RenderWindow* window, std::stack<Menu>* menus)
{
	menus->emplace(window, menus, "res/menus/titlescreen/options.json");

	auto gvol = menus->top().getWidget<gui::Slider>("global-vol");
	auto mvol = menus->top().getWidget<gui::Slider>("music-vol");
	auto svol = menus->top().getWidget<gui::Slider>("sound-vol");

	gvol->setScaleTarget(Data::masterVolume, 100.f);
	mvol->setScaleTarget(Data::musicVolume, 100.f);
	svol->setScaleTarget(Data::soundVolume, 100.f);

	gvol->onValueChange.bind([]() {	sf::Listener::setGlobalVolume(Data::masterVolume); });
	mvol->onValueChange.bind([]() { AudioManager::track.setVolume(Data::musicVolume); });
	svol->onValueChange.bind([]() { AudioManager::setEffectVolume(Data::soundVolume); });

	auto lang = menus->top().getWidget<gui::Button>("language");
	auto back = menus->top().getWidget<gui::Button>("back");

	lang->onClick.bind([]() {});
	lang->setEnabled(false);

	back->onClick.bind(&Menu::scheduleQuit, &menus->top());
}

void popGame(std::stack<Menu>* menus)
{
	menus->top().setNextScene<MainMenu>();
}