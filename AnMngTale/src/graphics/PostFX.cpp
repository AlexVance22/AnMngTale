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
	m_shaders[name].shader.loadFromFile(filepath, sf::Shader::Fragment);
	m_shaders[name].shader.setUniform("texture", sf::Shader::CurrentTexture);
}

void PostFX::removeShader(const std::string& name)
{
	m_shaders.erase(name);
}

void PostFX::setEnabled(const std::string& name, bool enabled)
{
	m_shaders[name].enabled = enabled;
}

void PostFX::setEnabled(bool enabled)
{
	m_enabled = enabled;
}


sf::Shader& PostFX::getShader(const std::string& name)
{
	return m_shaders[name].shader;
}

const sf::Shader& PostFX::getShader(const std::string& name) const
{
	return m_shaders.at(name).shader;
}


void PostFX::render(sf::RenderTarget* target)
{
	display();
	bool buffer = false;

	if (m_enabled)
	{
		setView(sf::View(sf::FloatRect(0, 0, (float)m_resolution.x, (float)m_resolution.y)));

		for (const auto& [_, effect] : m_shaders)
		{
			if (effect.enabled)
			{
				if (buffer)
				{
					draw(sf::Sprite(m_buffer.getTexture()), &effect.shader);
					display();
					m_buffer.clear();
					buffer = false;
				}
				else
				{
					m_buffer.draw(sf::Sprite(getTexture()), &effect.shader);
					m_buffer.display();
					clear();
					buffer = true;
				}
			}
		}
	}

	if (buffer)
		target->draw(sf::Sprite(m_buffer.getTexture()));
	else
		target->draw(sf::Sprite(getTexture()));
}