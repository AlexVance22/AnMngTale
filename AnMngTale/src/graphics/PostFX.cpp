#include "PCH.h"
#include "PostFX.h"

#include "core/Profiler.h"


PostFX::PostFX(sf::Vector2u resolution) : m_resolution(resolution)
{
	create(resolution.x, resolution.y);
	m_buffer.create(resolution.x, resolution.y);
}


void PostFX::loadShader(const std::string& filepath, const std::string& name)
{
	m_shaders[name].loadFromFile(filepath, sf::Shader::Fragment);
	m_shaders[name].setUniform("texture", sf::Shader::CurrentTexture);
}

void PostFX::removeShader(const std::string& name)
{
	m_shaders.erase(name);
}


sf::Shader& PostFX::getShader(const std::string& name)
{
	return m_shaders[name];
}

const sf::Shader& PostFX::getShader(const std::string& name) const
{
	return m_shaders.at(name);
}


void PostFX::render(sf::RenderTarget* target)
{
	display();
	bool buffer = false;

	setView(sf::View(sf::FloatRect(0, 0, (float)m_resolution.x, (float)m_resolution.y)));

	for (const auto& [_, shader] : m_shaders)
	{
		if (buffer)
		{
			draw(sf::Sprite(m_buffer.getTexture()), &shader);
			display();
			m_buffer.clear();
			buffer = false;
		}
		else
		{
			m_buffer.draw(sf::Sprite(getTexture()), &shader);
			m_buffer.display();
			clear();
			buffer = true;
		}
	}

	if (buffer)
		target->draw(sf::Sprite(m_buffer.getTexture()));
	else
		target->draw(sf::Sprite(getTexture()));
}