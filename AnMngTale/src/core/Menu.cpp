#include "PCH.h"
#include "Menu.h"

#include "core/Scene.h"
#include "core/Asserts.h"
#include "core/Profiler.h"
#include "core/JsonCasts.h"

#include "gui/gui.h"

#include "global/AudioManager.h"


void loadjson(rapidjson::Document& doc, const std::string& filepath);


Menu::Menu(sf::RenderWindow* window, std::stack<Menu>* overlays, const std::string& filepath) : p_window(window), p_overlays(overlays)
{
	loadFromFile(filepath);
}

Menu::Menu(sf::RenderWindow* window, std::stack<Menu>* overlays) : p_window(window), p_overlays(overlays)
{

}


void Menu::loadFromFile(const std::string& filepath)
{
	rapidjson::Document doc;
	loadjson(doc, filepath);

	const auto& vSizeData = doc["view"];
	const sf::Vector2f vSize = JsonToVec2<float>(vSizeData);
	m_view.setSize(vSize);
	m_view.setCenter(vSize * 0.5f);

	m_blocking = doc["blocking"].IsTrue();
	m_blurred = doc["blurred"].IsTrue();

	for (const auto& s : doc["sounds"].GetArray())
		AudioManager::addSound(s[0].GetString(), s[1].GetString());

	for (const auto& t : doc["textures"].GetArray())
		MNG_ASSERT_BASIC(m_textures[t[0].GetString()].loadFromFile(t[1].GetString()));

	for (const auto& f : doc["fonts"].GetArray())
		MNG_ASSERT_BASIC(m_fonts[f[0].GetString()].loadFromFile(f[1].GetString()));

	m_widgets = gui::Root::create();
	m_widgets->loadPreset(doc["root"], m_textures, m_fonts, doc["presets"]);
}


Obj<Scene>&& Menu::getNextScene()
{
	return std::move(m_nextScene);
}


void Menu::scheduleQuit()
{
	m_quitTop = true;
}

bool Menu::getQuitTop()
{
	return m_quitTop;
}

bool Menu::getQuitAll()
{
	return m_quitAll;
}


bool Menu::isBlocking()
{ 
	return m_blocking;
}

bool Menu::isBlurred()
{
	return m_blurred;
}


void Menu::handleEvent(const sf::Event& event)
{
	m_widgets->handleEvent(event);

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
			m_quitTop = true;
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