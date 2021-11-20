#pragma once


namespace particles
{
	struct Particle
	{
		sf::Vector2f velocity;

		void* userdata;
	};

	class System : public sf::Drawable
	{
	protected:
		std::vector<Particle> m_particles;
		std::vector<sf::Vertex> m_va;
		sf::FloatRect m_bounds;

	protected:
		void p_setPosition(size_t idx, sf::Vector2f position);
		sf::Vector2f p_getPosition(size_t idx) const;

		void p_setSize(size_t idx, sf::Vector2f size);
		sf::Vector2f p_getSize(size_t idx) const;

		void p_move(size_t idx, sf::Vector2f movement);
		sf::FloatRect p_getBounds(size_t idx) const;

		void p_erase(size_t idx);

	public:
		System() = default;
		System(size_t size);

		void setBoundary(sf::FloatRect boundary);

		virtual void append(void* userdata) = 0;
		virtual void update(const float deltaTime) = 0;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}