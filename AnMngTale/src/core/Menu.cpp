#include "PCH.h"
#include "Menu.h"

#include "core/Scene.h"
#include "core/Asserts.h"
#include "core/MenuStack.h"

#include "gui/gui.h"
#include "io/JsonUtils.h"
#include "global/AudioManager.h"


Menu::Menu()
{

}

Menu::Menu(const std::string& filepath, MenuStack* stack)
{
	loadFromFile(filepath, stack);
}


void Menu::loadFromFile(const std::string& filepath, MenuStack* stack)
{
	p_container = stack;

	rapidjson::Document doc;
	loadjson(doc, filepath);

	m_blocking = doc["blocking"].IsTrue();
	m_blurred = doc["blurred"].IsTrue();

	for (const auto& k : doc["quitkeys"].GetArray())
		m_quitKeys.insert((sf::Keyboard::Key)k.GetInt());

	for (const auto& t : doc["textures"].GetArray())
	{
		//if (t["shared"].IsFalse() || p_container->empty())
			MNG_ASSERT_BASIC(m_textures[t["name"].GetString()].loadFromFile(t["file"].GetString()));
	}

	for (const auto& s : doc["sounds"].GetArray())
	{
		if (s["shared"].IsFalse() || p_container->empty())
			AudioManager::addSound(s["name"].GetString(), s["file"].GetString());
	}

	for (const auto& f : doc["fonts"].GetArray())
	{
		//if (f["shared"].IsFalse() || p_container->empty())
			MNG_ASSERT_BASIC(m_fonts[f["name"].GetString()].loadFromFile(f["file"].GetString()));
	}

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
		//sf::Keyboard::Escape 36
		//sf::Keyboard::E 4

		if (m_quitKeys.find(event.key.code) != m_quitKeys.end())
			m_quitTop = true;
	}
}

void Menu::update(const float deltaTime)
{
	m_widgets->update(deltaTime);
}

void Menu::render(sf::RenderTarget* target)
{
	target->draw(*m_widgets);
}