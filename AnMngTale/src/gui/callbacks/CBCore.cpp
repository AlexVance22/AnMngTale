#include "PCH.h"
#include "CBCore.h"

#include "core/MenuStack.h"
#include "gui/gui.h"

#include "global/AudioManager.h"

#include "scenes/MainMenu.h"
#include "scenes/outside/Courtyard.h"


void gameStart(MenuStack* menus, uint8_t save)
{
	menus->top().setNextScene<Courtyard>();
}

void gameEnd(MenuStack* menus)
{
	menus->top().setNextScene<MainMenu>();
}


void pushQuitMenu(MenuStack* menus, bool playerLocked)
{
	menus->push("res/menus/quit.json", playerLocked);

	menus->top().getWidget<gui::Button>("resume")->onClick.bind(&Menu::scheduleQuit, &menus->top());
	menus->top().getWidget<gui::Button>("options")->onClick.bind(&pushOptionsMenu, menus);
	menus->top().getWidget<gui::Button>("quit")->onClick.bind(&gameEnd, menus);
}

void pushOptionsMenu(MenuStack* menus)
{
	menus->push("res/menus/titlescreen/options.json");

	auto gvol = menus->top().getWidget<gui::Slider>("global-vol");
	auto mvol = menus->top().getWidget<gui::Slider>("music-vol");
	auto evol = menus->top().getWidget<gui::Slider>("effect-vol");
	//auto svol = menus->top().getWidget<gui::Slider>("sound-vol");

	gvol->setRawValue(AudioManager::getGlobalVolume() * 0.01f);
	mvol->setRawValue(AudioManager::getMusicVolume() * 0.01f);
	evol->setRawValue(AudioManager::getEffectVolume() * 0.01f);
	//svol->setRawValue(AudioManager::getSoundVolume() * 0.01f);

	gvol->onValueChange.bind([](gui::Slider* ptr) { AudioManager::setGlobalVolume(ptr->getRawValue() * 100.f); }, gvol.get());
	mvol->onValueChange.bind([](gui::Slider* ptr) { AudioManager::setMusicVolume(ptr->getRawValue() * 100.f); }, mvol.get());
	evol->onValueChange.bind([](gui::Slider* ptr) { AudioManager::setEffectVolume(ptr->getRawValue() * 100.f); }, evol.get());
	//svol->onValueChange.bind([](gui::Slider* ptr) { AudioManager::setSoundVolume(ptr->getRawValue() * 100.f); }, svol.get());

	auto lang = menus->top().getWidget<gui::Button>("language");
	auto back = menus->top().getWidget<gui::Button>("back");

	lang->onClick.bind([]() {});
	lang->setEnabled(false);

	back->onClick.bind(&Menu::scheduleQuit, &menus->top());
}

void pushFileMenu(MenuStack* menus, bool* saveData)
{
	menus->push("res/menus/titlescreen/files.json");

	auto f1 = menus->top().getWidget<gui::Button>("0");
	auto f2 = menus->top().getWidget<gui::Button>("1");
	auto f3 = menus->top().getWidget<gui::Button>("2");

	f1->onClick.bind(&gameStart, menus, 0);
	f2->onClick.bind(&gameStart, menus, 1);
	f3->onClick.bind(&gameStart, menus, 2);

	if (saveData)
	{
		f1->setEnabled(saveData[0]);
		f2->setEnabled(saveData[1]);
		f3->setEnabled(saveData[2]);
	}
}


void pushAgenda(MenuStack* menus)
{
	menus->push("res/menus/agenda.json");
}

void pushDialogue(MenuStack* menus, const std::vector<std::string>& text)
{
	menus->push("res/menus/dialayer.json");

	auto d = menus->top().getWidget<gui::Dialogue>("popup");

	d->setText(text);
	d->setPlaySpeed(40.f);
	d->onFinish.bind(&Menu::scheduleQuit, &menus->top());
}