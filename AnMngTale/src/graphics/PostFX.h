#pragma once


class PostFX : public sf::RenderTexture
{
private:
	std::unordered_map<std::string, sf::Shader> m_shaders;
	sf::RenderTexture m_buffer;
	const sf::Vector2u m_resolution;

public:
	bool guiIncluded = false;

public:
	PostFX(sf::Vector2u resolution);

	void loadShader(const std::string& filepath, const std::string& name);
	void removeShader(const std::string& name);

	sf::Shader& getShader(const std::string& name);
	const sf::Shader& getShader(const std::string& name) const;

	void render(sf::RenderTarget* target);
};