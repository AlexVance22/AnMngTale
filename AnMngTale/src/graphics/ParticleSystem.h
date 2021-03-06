#pragma once

#include "Noise.h"


class ParticleSystem : public sf::Drawable
{
public:
	enum class Mode { None = -1, Fireflies, Snow, Fountain };

private:
	struct Particle
	{
		sf::Vector2u noiseCoord;

		sf::Vector2f velocity;
		float angle;
	};

	sf::FloatRect m_bounds;
	sf::Vector2f m_size;
	sf::Vector2f m_dsize;
	bool m_randSize = false;

	std::vector<sf::Vertex> m_va;
	const sf::Texture* m_tex = nullptr;
	sf::Vector2f m_texsize;
	sf::Color m_color;

	Mode m_mode = Mode::None;

	std::vector<Particle> m_particles;

	Noise m_noise;

	size_t m_minParticles = 0;

	float m_genprog = 0.f;
	float m_gentime = 1.f;

	sf::Vector2f m_gravityDir;
	float m_gravityStr = 0.f;
	float m_dragStr = 0.f;
	sf::Vector2f m_emitter;
	float m_startVel = 1.f;

	bool m_shrink = false;
	float m_shrinkprog = 0.f;
	float m_shrinktime = 0.f;

private:
	void p_setPosition(size_t idx, sf::Vector2f position);
	sf::Vector2f p_getPosition(size_t idx) const;

	void p_setSize(size_t idx, sf::Vector2f size);
	sf::Vector2f p_getSize(size_t idx) const;

	void p_setColor(size_t idx, sf::Color color);
	void p_setTextureSize(size_t idx, sf::Vector2f size);

	void p_move(size_t idx, sf::Vector2f movement);
	sf::FloatRect p_getBounds(size_t idx) const;

	void p_erase(size_t idx);
	
private:
	void shrink(const float deltaTime);
	void cull();

	void doFireflies(const float deltaTime);
	void doSnow(const float deltaTime);
	void doFountain(const float deltaTime);

public:
	ParticleSystem() = default;
	ParticleSystem(sf::FloatRect rect);

	void setColor(sf::Color color);
	void setTexture(const sf::Texture& tex);

	void setSize(sf::Vector2f size);
	void setSize(sf::Vector2f minRand, sf::Vector2f maxRand);

	void setMode(Mode mode);
	void setBoundary(sf::FloatRect boundary);

	void setMinParticles(uint32_t minimum);

	void setGenRate(float ratePerSec);

	void setEmitter(sf::Vector2f pos);
	void setStartVel(float startVel);
	void setGravityStr(float force);
	void setGravityDir(float degrees);
	void setDragStr(float force);

	void setShrinkRate(float sizePerSec);

	void append(int idx = -1);
	void generate(size_t count);

	void update(const float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};