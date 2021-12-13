#pragma once

/*
class Sprite : public sf::Transformable, public sf::Drawable
{
private:
	sf::VertexArray m_va;

	const sf::Texture* m_texture = nullptr;
	const sf::Shader* m_shader = nullptr;

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
	Sprite(const sf::Texture & texture);

	void animate(uint32_t frames, uint32_t anims);

	void setFrameRate(float framerate);
	void setLoop(bool loop) { m_loop = loop; }
	void setIsPlaying(bool playing);
	void setFrame(uint32_t frame);
	void setAnimation(uint32_t anim);

	void setShader(const sf::Shader & shader) { m_shader = &shader; }

	bool isPlaying() { return m_playing && m_animated; }
	bool isAnimated() { return m_animated; }

	void setTexture(const sf::Texture& texture)
	{
		m_texture = &texture;

		const sf::Vector2f size = (sf::Vector2f)texture.getSize();

		m_va[1].position = sf::Vector2f(size.x, 0);
		m_va[2].position = size;
		m_va[3].position = sf::Vector2f(0, size.y);

		setTextureRect(sf::FloatRect(sf::Vector2f(0, 0), size));
	}
	void setTextureRect(sf::FloatRect rect)
	{
		m_va[0].texCoords = sf::Vector2f(rect.left,					rect.top);
		m_va[1].texCoords = sf::Vector2f(rect.left + rect.width,	rect.top);
		m_va[2].texCoords = sf::Vector2f(rect.left + rect.width,	rect.top + rect.height);
		m_va[3].texCoords = sf::Vector2f(rect.left,					rect.top + rect.height);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states)
	{
		states.texture = m_texture;
		states.shader = m_shader;
		states.transform *= getTransform();

		target.draw(m_va, states);
	}
};
*/

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

	const sf::Shader* m_shader = nullptr;

public:
	Sprite() = default;
	Sprite(const sf::Texture& texture);

	void animate(uint32_t frames, uint32_t anims);

	void setFrameRate(float framerate);
	void setLoop(bool loop) { m_loop = loop; }
	void setIsPlaying(bool playing);
	void setFrame(uint32_t frame);
	void setAnimation(uint32_t anim);

	void setShader(const sf::Shader& shader) { m_shader = &shader; }
	const sf::Shader* getShader() const { return m_shader; }

	bool isPlaying() { return m_playing && m_animated; }
	bool isAnimated() { return m_animated; }

	void update(const float deltaTime);
};