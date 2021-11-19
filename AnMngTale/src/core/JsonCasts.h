#pragma once


template<typename N>
sf::Vector2<N> JsonToVec2(const rapidjson::Value& array)
{
	assert(false);
	return {};
}

template<>
sf::Vector2<float> JsonToVec2(const rapidjson::Value& array);

template<>
sf::Vector2<int32_t> JsonToVec2(const rapidjson::Value& array);

template<>
sf::Vector2<uint32_t> JsonToVec2(const rapidjson::Value& array);


b2Vec2 JsonToB2Vec(const rapidjson::Value& array);


sf::FloatRect JsonToRect(const rapidjson::Value& array);


sf::Color JsonToColor(const rapidjson::Value& array);