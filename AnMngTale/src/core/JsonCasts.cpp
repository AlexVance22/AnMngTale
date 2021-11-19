#include "PCH.h"
#include "JsonCasts.h"


template<>
sf::Vector2<float> JsonToVec2(const rapidjson::Value& array)
{
	return sf::Vector2f(array[0].GetFloat(), array[1].GetFloat());
}

template<>
sf::Vector2<int32_t> JsonToVec2(const rapidjson::Value& array)
{
	return sf::Vector2i(array[0].GetInt(), array[1].GetInt());
}

template<>
sf::Vector2<uint32_t> JsonToVec2(const rapidjson::Value& array)
{
	return sf::Vector2u(array[0].GetUint(), array[1].GetUint());
}


b2Vec2 JsonToB2Vec(const rapidjson::Value& array)
{
	return b2Vec2(array[0].GetFloat(), array[1].GetFloat());
}


sf::FloatRect JsonToRect(const rapidjson::Value& array)
{
	return sf::FloatRect(array[0].GetFloat(), array[1].GetFloat(), array[2].GetFloat(), array[3].GetFloat());
}


sf::Color JsonToColor(const rapidjson::Value& array)
{
	return sf::Color(array[0].GetUint(), array[1].GetUint(), array[2].GetUint(), array[3].GetUint());
}