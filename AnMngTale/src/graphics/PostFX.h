#pragma once


class PostFX : public sf::RenderTexture
{
private:
	struct Effect
	{
		sf::Shader shader;
		bool enabled = true;
	};

	std::unordered_map<std::string, Effect> m_shaders;
	sf::RenderTexture m_buffer;
	const sf::Vector2u m_resolution;
	bool m_enabled = true;

public:
	bool guiIncluded = false;

public:
	PostFX(sf::Vector2u resolution);

	void loadShader(const std::string& filepath, const std::string& name);
	void removeShader(const std::string& name);
	void setEnabled(const std::string& name, bool enabled);
	void setEnabled(bool enabled);

	sf::Shader& getShader(const std::string& name);
	const sf::Shader& getShader(const std::string& name) const;

	void render(sf::RenderTarget* target);
};