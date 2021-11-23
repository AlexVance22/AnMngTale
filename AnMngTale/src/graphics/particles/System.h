#pragma once


namespace particles
{
	class System : public sf::Drawable
	{
	protected:
		sf::FloatRect m_bounds;
		sf::Vector2f m_size;
		sf::Vector2f m_dsize;
		bool m_randSize = false;

		std::vector<sf::Vertex> m_va;
		const sf::Texture* m_tex = nullptr;
		sf::Vector2f m_texsize;
		sf::Color m_color;

	protected:
		void p_setPosition(size_t idx, sf::Vector2f position);
		sf::Vector2f p_getPosition(size_t idx) const;

		void p_setSize(size_t idx, sf::Vector2f size);
		sf::Vector2f p_getSize(size_t idx) const;

		void p_setColor(size_t idx, sf::Color color);
		void p_setTextureSize(size_t idx, sf::Vector2f size);

		void p_move(size_t idx, sf::Vector2f movement);
		sf::FloatRect p_getBounds(size_t idx) const;

		void p_erase(size_t idx);
		
		virtual void cull();

		size_t emplace(int idx = -1);

	public:
		System() = default;
		System(sf::FloatRect rect);

		void setColor(sf::Color color);
		void setTexture(const sf::Texture& tex);

		void setSize(sf::Vector2f size);
		void setSize(sf::Vector2f minRand, sf::Vector2f maxRand);
		void setBoundary(sf::FloatRect boundary);

		virtual void append(int idx = -1) = 0;
		void generate(size_t count);
		virtual void update(const float deltaTime) = 0;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}