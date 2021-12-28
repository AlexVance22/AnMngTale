#include "PCH.h"
#include "CBCore.h"

#include "core/MenuStack.h"
#include "core/Asserts.h"
#include "gui/gui.h"

#include "global/AudioManager.h"
#include "global/Agenda.h"

#include "scenes/MainMenu.h"

#include "io/Deserialiser.h"
#include "io/JsonUtils.h"

#include "LoadScene.h"


void gameStart(MenuStack* menus, uint8_t save)
{
	std::string filepath = "config/state" + std::to_string(save) + ".json";

	std::ofstream target(filepath);
	MNG_ASSERT_SLIM(target.is_open());
	std::ifstream base("config/statedef.json");
	MNG_ASSERT_SLIM(base.is_open());

	for (std::string line; std::getline(base, line);)
		target << line;

	target.close();

	Deserialiser::activeFile = save;

	rapidjson::Document doc;
	loadjson(doc, filepath.c_str());
	doc["begun"].SetBool(true);
	dumpjson(doc, filepath.c_str());

	getSceneFromID(menus, 1);
}

void gameLoad(MenuStack* menus, uint8_t save)
{
	Deserialiser::activeFile = save;

	std::string filepath = "config/state" + std::to_string(save) + ".json";
	rapidjson::Document doc;
	loadjson(doc, filepath.c_str());
	getSceneFromID(menus, doc["scene"].GetUint());
}

void gameEnd(MenuStack* menus)
{
	menus->top().setNextScene<MainMenu>();
}


void pushWarning(MenuStack* menus, uint8_t save)
{
	menus->push("res/menus/titlescreen/warning.json");

	auto no = menus->top().getWidget<gui::Button>("no");
	auto yes = menus->top().getWidget<gui::Button>("yes");

	no->onClick.bind(&Menu::scheduleQuit, &menus->top());
	yes->onClick.bind(&gameStart, menus, save);
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

void pushFileMenu(MenuStack* menus, bool load, bool* saveData)
{
	menus->push("res/menus/titlescreen/files.json");

	auto f1 = menus->top().getWidget<gui::Button>("0");
	auto f2 = menus->top().getWidget<gui::Button>("1");
	auto f3 = menus->top().getWidget<gui::Button>("2");

	if (load)
	{
		f1->onClick.bind(&gameStart, menus, 0);
		f2->onClick.bind(&gameStart, menus, 1);
		f3->onClick.bind(&gameStart, menus, 2);

		f1->setEnabled(saveData[0]);
		f2->setEnabled(saveData[1]);
		f3->setEnabled(saveData[2]);
	}
	else
	{
		if (saveData[0])
			f1->onClick.bind(&pushWarning, menus, 0);
		else
			f1->onClick.bind(&gameStart, menus, 0);

		if (saveData[1])
			f2->onClick.bind(&pushWarning, menus, 1);
		else
			f2->onClick.bind(&gameStart, menus, 1);

		if (saveData[2])
			f3->onClick.bind(&pushWarning, menus, 2);
		else
			f3->onClick.bind(&gameStart, menus, 2);
	}
}


void pushAgenda(MenuStack* menus, bool playerLocked)
{
	menus->push("res/menus/agenda.json", playerLocked);

	uint32_t count = 0;

	for (const auto& [name, quest] : Agenda::getActiveQuests())
	{
		auto label = gui::Label::create();
		label->setString(name);
		label->setFont(menus->top().m_fonts["cambria"]);
		label->setCharacterSize(25);
		label->setLayout(sf::Vector2i(400, 150 + 100 * count), sf::Vector2i(400, 100));
		label->setTextPosition(sf::Vector2f(0, 0));
		label->setTextColor(sf::Color::Black);
		label->setStyle(sf::Text::Bold);

		auto desc = gui::Label::create();
		desc->setString(quest.description);
		desc->setFont(menus->top().m_fonts["cambria"]);
		desc->setCharacterSize(25);
		desc->setLayout(sf::Vector2i(400, 190 + 100 * count), sf::Vector2i(400, 100));
		desc->setTextPosition(sf::Vector2f(0, 0));
		desc->setTextColor(sf::Color::Black);

		auto lst = gui::cast<gui::Container>(menus->top().m_widgets->get("list"));
		lst->add(label, "label" + std::to_string(count));
		lst->add(desc, "desc" + std::to_string(count));

		count++;
	}
}