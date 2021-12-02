#include "PCH.h"
#include "Menu.h"

#include "core/Scene.h"
#include "core/Asserts.h"
#include "core/Profiler.h"

#include "gui/gui.h"
#include "io/JsonUtils.h"
#include "global/AudioManager.h"


Menu::Menu()
{

}

Menu::Menu(const std::string& filepath)
{
	loadFromFile(filepath);
}


void Menu::loadFromFile(const std::string& filepath)
{
	rapidjson::Document doc;
	loadjson(doc, filepath);

	const sf::Vector2f vSize = JsonToVec2<float>(doc["view"]);
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
	m_widgets->load(doc["root"], m_textures, m_fonts, doc["presets"]);
}


bool Menu::hasNextScene() const
{
	return m_hasNextScene;
}

std::future<Obj<Scene>>&& Menu::getNextScene()
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
	target->setView(m_view);
	target->draw(*m_widgets);
}