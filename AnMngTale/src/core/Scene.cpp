#include "PCH.h"
#include "Scene.h"

#include "core/Asserts.h"
#include "core/Profiler.h"

#include "gui/callbacks/CBCore.h"

#include "entities/Player.h"

#include "io/Deserialiser.h"
#include "io/JsonUtils.h"


#ifdef MNG_DEBUG

std::string f_to_string(float val, uint32_t prec)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(prec) << val;
	return stream.str();
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const sf::Vector2<T>& vec)
{
	return stream << '[' << vec.x << ", " << vec.y << ']';
}

#endif


sf::RenderWindow* Scene::p_window;
sf::RenderTexture Scene::m_fadeBuffer;


void Scene::reloadResources(bool clear)
{
	if (clear)
	{
		m_player = nullptr;
		m_physWorld.reset();

		m_textures.clear();
		m_shaders.clear();
		m_triggers.clear();

		m_backgroundSprites.clear();
		m_foregroundSprites.clear();
		m_entities.clear();
		m_particles.clear();
		m_scripts.clear();

		m_dialogue.stop();

#if MNG_DEBUG
		d_debugChainColliders.clear();
		d_debugBoxColliders.clear();
#endif
	}

	Deserialiser des(this);
	des.run();
}


void Scene::handleGui(const float deltaTime)
{
	m_overlays.top().update(deltaTime);

	if (m_overlays.top().hasNextScene())
	{
		auto next = m_overlays.top().getNextScene();
		m_progress = false;
		m_quit = true;
		fadeout();
		m_nextScene = std::move(next.get());
	}
	else if (m_overlays.top().getQuitAll())
	{
		while (!m_overlays.empty())
			m_overlays.pop();

		if (m_player)
			m_player->lock(m_overlays.getCachedLock());
	}
	else if (m_overlays.top().getQuitTop())
	{
		m_overlays.pop();

		if (m_player && m_overlays.empty())
			m_player->lock(m_overlays.getCachedLock());
	}
}

void Scene::handleGame(const float deltaTime)
{
	PROFILE_DEBUG_ONLY_BEGIN(false, MICROSECONDS);

	for (auto& a : m_backgroundSprites)
		a.update(deltaTime);
	for (auto& e : m_entities)
		e->update(deltaTime);
	for (auto& a : m_foregroundSprites)
		a.update(deltaTime);

	for (auto& s : m_scripts)
		s.update(deltaTime);

	m_physWorld->Step(deltaTime, 6, 2);

	m_dialogue.update(deltaTime);

	impl(deltaTime);

	PROFILE_DEBUG_ONLY_STEP();

	std::sort(m_entities.begin(), m_entities.end(), [](const Obj<Entity>& left, const Obj<Entity>& right)
		{
			return left->getPosition().y + left->getSize().y < right->getPosition().y + right->getSize().y;
		});

	m_camera.update(deltaTime);

	if (m_player)
	{
		const sf::FloatRect collider = m_player->getCollider();

		for (auto& [_, t] : m_triggers)
			t.triggerOnIntersects(collider);
	}

	PROFILE_DEBUG_ONLY_STEP();

#ifdef MNG_DEBUG
	const auto& times = __prof.data();
	d_tPhysics.setString("Physics:   " + f_to_string(times[0] * 0.001f, 3) + "ms");
	d_tUserlogic.setString("Logic:     " + f_to_string(times[1] * 0.001f, 3) + "ms");
#endif
}


void Scene::handleEvents()
{
	sf::Event event;
	while (p_window->pollEvent(event))
	{
		if (m_overlays.empty())
		{
			handleEventDefault(event);
			handleEventSpecial(event);
		}
		else
		{
			if (!m_overlays.top().isBlocking())
			{
				handleEventDefault(event);
				handleEventSpecial(event);
			}
		
			m_overlays.top().handleEvent(event);
		}
	}
}

void Scene::handleEventDefault(const sf::Event& event)
{
	m_dialogue.handleEvent(event);

	switch (event.type)
	{
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			if (m_overlays.empty())
				pushQuitMenu(&m_overlays, m_player ? m_player->isLocked() : false);
			break;
		case sf::Keyboard::E:
			if (m_overlays.empty())
				pushAgenda(&m_overlays);
			break;

		case sf::Keyboard::R:
#ifdef MNG_DEBUG
			reloadResources(true);
#endif
			break;
		}
		break;

	case sf::Event::MouseButtonPressed:
#ifdef MNG_DEBUG
			std::cout << p_window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_camera) << '\n';
#endif
		break;
	}
}

void Scene::handleEventSpecial(const sf::Event& event)
{

}


Scene::Scene(const std::string& scenename)
	: m_name(scenename), m_postfx(sf::Vector2u(1920, 1080))
	
#ifdef MNG_DEBUG
	, d_debugChainColliders(sf::LineStrip), d_debugBoxColliders(sf::Lines),
	d_framerate("Framerate: NA", d_debugFont, 18), d_tUpdate("Update:    NA", d_debugFont, 18),
	d_tRender("Render:    NA", d_debugFont, 18), d_tPhysics("Physics:   NA", d_debugFont, 18),
	d_tUserlogic("Logic:     NA", d_debugFont, 18)
#endif
{
	reloadResources();

	m_camera.moveToTarget();
	m_camera.update(0);

	m_postfx.loadShader("res/shaders/blur.frag", "blur");
	m_postfx.setEnabled("blur", false);

#ifdef MNG_DEBUG
	d_debugFont.loadFromFile("res/fonts/courierprime.ttf");
	d_framerate.setPosition(10, 10);
	d_tUpdate.setPosition(10, 40);
	d_tRender.setPosition(10, 60);
	d_tPhysics.setPosition(10, 90);
	d_tUserlogic.setPosition(10, 110);
	d_framerate.setFillColor(sf::Color(0, 0, 0));
	d_tUpdate.setFillColor(sf::Color(0, 0, 0));
	d_tRender.setFillColor(sf::Color(0, 0, 0));
	d_tPhysics.setFillColor(sf::Color(0, 0, 0));
	d_tUserlogic.setFillColor(sf::Color(0, 0, 0));
#endif
}

Scene::~Scene()
{
	if (m_progress)
	{
		rapidjson::Document doc;
		loadjson(doc, "config/state0.json");

		doc[m_name.c_str()].SetUint(m_state + 1);

		dumpjson(doc, "config/state0.json");
	}
}


bool Scene::quit() const
{
	return m_quit;
}

Obj<Scene>&& Scene::nextScene()
{
	return std::move(m_nextScene);
}


void Scene::fadeout()
{
	render(&m_fadeBuffer);
	m_fadeBuffer.display();
	sf::Sprite sp(m_fadeBuffer.getTexture());

	for (int i = 15; i >= 0; i--)
	{
		sp.setColor(sf::Color(16 * i, 16 * i, 16 * i, 255));
		p_window->draw(sp);
		p_window->display();
	}
}

void Scene::fadein()
{
	render(&m_fadeBuffer);
	m_fadeBuffer.display();
	sf::Sprite sp(m_fadeBuffer.getTexture());

	for (int i = 0; i < 16; i++)
	{
		sp.setColor(sf::Color(16 * i, 16 * i, 16 * i, 255));
		p_window->draw(sp);
		p_window->display();
	}
}


void Scene::update(const float deltaTime)
{
	PROFILE_DEBUG_ONLY_BEGIN(false, MICROSECONDS);

	for (auto& p : m_particles)
		p.update(deltaTime);

	handleEvents();

	m_postfx.setEnabled("blur", false);

	if (m_overlays.empty())
	{
		handleGame(deltaTime);

		PROFILE_DEBUG_ONLY_STEP();

#ifdef MNG_DEBUG
		d_tUpdate.setString("Update:    " + f_to_string(__prof.data()[0] * 0.001f, 3) + "ms");
#endif
	}
	else
	{
		if (m_player)
			m_player->lock(true);

		if (!m_overlays.top().isBlocking())
			handleGame(deltaTime);

		if (m_overlays.top().isBlurred())
			m_postfx.setEnabled("blur", true);

		handleGui(deltaTime);

		PROFILE_DEBUG_ONLY_STEP();

#ifdef MNG_DEBUG
		const std::string t = f_to_string(__prof.data()[0] * 0.001f, 3) + "ms";
		d_tUpdate.setString("Update:    " + t);

		if (!m_overlays.empty())
		{
			if (m_overlays.top().isBlocking())
			{
				d_tUserlogic.setString("Logic:     " + t);
				d_tPhysics.setString("Physics:   NA");
			}
		}
#endif
	}

#ifdef MNG_DEBUG
	d_framerate.setString("Framerate: " + std::to_string((uint32_t)(1.f / deltaTime)));
#endif
}

void Scene::render(sf::RenderTarget* target)
{
	PROFILE_DEBUG_ONLY_BEGIN(false, MICROSECONDS);

	if (!target)
		target = p_window;

	m_postfx.setView(m_camera);

	for (const auto& sp : m_backgroundSprites)
		m_postfx.draw(sp, sp.getShader());
	for (const auto& e : m_entities)
		m_postfx.draw(*e);
	for (const auto& sp : m_foregroundSprites)
		m_postfx.draw(sp, sp.getShader());

#ifdef MNG_DEBUG
	m_postfx.draw(d_debugChainColliders);
	for (const auto& rect : d_debugBoxColliders)
		m_postfx.draw(rect);
#endif

	m_postfx.setView(sf::View(sf::FloatRect(0, 0, 1920, 1080)));
	m_postfx.draw(m_dialogue);

	if (m_postfx.guiIncluded)
	{
		if (!m_overlays.empty())
			m_overlays.top().render(&m_postfx);

		m_postfx.render(target);
	}
	else
	{
		m_postfx.render(target);

		if (!m_overlays.empty())
			m_overlays.top().render(target);
	}

	target->setView(target->getDefaultView());

	for (const auto& p : m_particles)
		target->draw(p);

	PROFILE_DEBUG_ONLY_STEP();

#ifdef MNG_DEBUG
	d_tRender.setString("Render:    " + f_to_string(__prof.data()[0] * 0.001f, 3) + "ms");
	target->draw(d_framerate);
	target->draw(d_tUpdate);
	target->draw(d_tRender);
	target->draw(d_tPhysics);
	target->draw(d_tUserlogic);
#endif
}