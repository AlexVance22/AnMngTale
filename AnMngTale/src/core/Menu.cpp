#include "PCH.h"
#include "Menu.h"

#include "core/Asserts.h"
#include "core/Profiler.h"
#include "core/JsonCasts.h"

#include "gui/gui.h"

#include "global/AudioManager.h"


void loadjson(rapidjson::Document& doc, const std::string& filepath);


Menu::Menu(sf::RenderWindow* window, std::stack<Menu>* menus, const std::string& filepath) : p_window(window), p_menus(menus)
{
	loadFromFile(filepath);
}

Menu::Menu(sf::RenderWindow* window, std::stack<Menu>* menus)
	: p_window(window), p_menus(menus)
{};


void Menu::loadFromFile(const std::string& filepath)
{
	rapidjson::Document doc;
	loadjson(doc, filepath);

	const auto& vSizeData = doc["view"];
	const sf::Vector2f vSize = JsonToVec2<float>(vSizeData);
	m_view.setSize(vSize);
	m_view.setCenter(vSize * 0.5f);

	m_blocking = doc["blocking"].IsTrue();

	for (const auto& s : doc["sounds"].GetArray())
		AudioManager::addSound(s[0].GetString(), s[1].GetString());

	for (const auto& t : doc["textures"].GetArray())
		MNG_ASSERT_BASIC(m_textures[t[0].GetString()].loadFromFile(t[1].GetString()));

	for (const auto& f : doc["fonts"].GetArray())
		MNG_ASSERT_BASIC(m_fonts[f[0].GetString()].loadFromFile(f[1].GetString()));

	m_widgets = gui::Root::create();
	m_widgets->loadPreset(doc["root"], m_textures, m_fonts, doc["presets"]);
}


void Menu::handleEvent(const sf::Event& event)
{
	m_widgets->handleEvent(event);

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
			m_quit = true;
	}
}

void Menu::update(const float deltaTime)
{
	m_widgets->update(deltaTime);
}

void Menu::render(sf::RenderTarget* target)
{
	if (!target)
		target = p_window;

	target->setView(m_view);

	target->draw(*m_widgets);
}