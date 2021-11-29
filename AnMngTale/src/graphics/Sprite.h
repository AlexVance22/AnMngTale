#pragma once


class Sprite : public sf::Sprite
{
private:
	uint32_t m_animCount = 1;
	uint32_t m_animSize = 1;
	
	sf::Vector2u m_cellSize;
	sf::Vector2u m_currentImage;

	float m_currentTime = 0;
	float m_frameTime = 0;
	bool m_animated = false;
	bool m_playing = false;
	bool m_loop = false;

public:
	Sprite() = default;
	Sprite(const sf::Texture& texture);

	void animate(uint32_t frames, uint32_t anims);

	void setFrameRate(float framerate);
	void setLoop(bool loop) { m_loop = loop; }
	void setIsPlaying(bool playing);
	void setFrame(uint32_t frame);
	void setAnimation(uint32_t anim);

	bool isPlaying() { return m_playing && m_animated; }
	bool isAnimated() { return m_animated; }

	void update(const float deltaTime);
};