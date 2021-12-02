#include "PCH.h"
#include "MainMenu.h"

#include "gui/gui.h"
#include "gui/callbacks/CBCore.h"

#include "io/JsonUtils.h"


void MainMenu::loadFlags()
{
	for (size_t i = 0; i < 3; i++)
	{
		std::string filepath = "config/saves/save" + std::to_string(i) + "/core.json";
		rapidjson::Document doc;
		loadjson(doc, filepath.c_str());
		m_isActiveSave[i] = doc["active"].IsTrue();
	}
}

void MainMenu::dumpFlags()
{

}


void MainMenu::impl(const float deltaTime)
{
	m_quit = true;
}


MainMenu::MainMenu() : Scene("mainmenu")
{
	loadFlags();

	m_overlays.emplace(p_window, &m_overlays, "res/menus/titlescreen/title.json");
	m_overlays.top().getWidget<gui::Button>("start")->onClick.bind(&pushFileMenu, p_window, &m_overlays, nullptr);
	if (!(m_isActiveSave[0] || m_isActiveSave[1] || m_isActiveSave[2]))
		m_overlays.top().getWidget<gui::Button>("load")->onClick.bind(&pushFileMenu, p_window, &m_overlays, &m_isActiveSave[0]);
	else
		m_overlays.top().getWidget<gui::Button>("load")->setEnabled(false);
	m_overlays.top().getWidget<gui::Button>("options")->onClick.bind(&pushOptionsMenu, p_window, &m_overlays);
	m_overlays.top().getWidget<gui::Button>("quit")->onClick.bind(&sf::RenderWindow::close, p_window);
}