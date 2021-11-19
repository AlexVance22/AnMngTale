#include "PCH.h"
#include "Sprite.h"


Sprite::Sprite(const sf::Texture& texture) : sf::Sprite(texture) {}


void Sprite::animate(uint32_t frames, uint32_t anims)
{
	sf::Vector2u size = getTexture()->getSize();
	m_animSize = frames;
	m_animCount = anims;
	m_cellSize = { size.x / m_animSize, size.y / m_animCount };

	m_animated = true;

	setTextureRect(sf::IntRect(0, 0, m_cellSize.x, m_cellSize.y));
}

void Sprite::setFrameRate(float framerate)
{
	if (framerate == 0)
		m_playing = false;
	else
		m_frameTime = 1.0f / framerate;
}

void Sprite::setIsPlaying(bool playing)
{
	m_playing = m_animated * playing;
}

void Sprite::setFrame(uint32_t frame)
{
	m_currentImage.x = frame;

	if (m_animated)
		setTextureRect(sf::IntRect(m_currentImage.x * m_cellSize.x, m_currentImage.y * m_cellSize.y, m_cellSize.x, m_cellSize.y));
}

void Sprite::setAnimation(uint32_t anim)
{
	m_currentImage.y = anim;

	if (m_animated)	
		setTextureRect(sf::IntRect(m_currentImage.x * m_cellSize.x, m_currentImage.y * m_cellSize.y, m_cellSize.x, m_cellSize.y));
}


void Sprite::update(const float deltaTime)
{
	if (m_playing && m_animated)
	{
		if (m_currentTime >= m_frameTime)
		{
			m_currentTime -= m_frameTime;
			m_currentImage.x++;
			if (m_currentImage.x == m_animSize)
			{
				if (m_loop)
					m_currentImage.x = 0;
				else
				{
					m_playing = false;
					m_currentImage.x--;
				}
			}

			setTextureRect(sf::IntRect(m_currentImage.x * m_cellSize.x, m_currentImage.y * m_cellSize.y, m_cellSize.x, m_cellSize.y));
		}
		else
			m_currentTime += deltaTime;
	}
}