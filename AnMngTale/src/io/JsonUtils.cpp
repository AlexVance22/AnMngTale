#include "PCH.h"
#include "JsonUtils.h"

#include "core/Asserts.h"


const char* freadall(const std::string& filepath)
{
	std::ifstream f(filepath, std::ios::binary);
	MNG_ASSERT_SLIM(f.is_open());
	f.seekg(0, std::ios::end);
	size_t size = f.tellg();
	f.seekg(0, std::ios::beg);
	char* buf = new char[size + 1];
	f.read(buf, size);
	f.close();
	buf[size] = '\0';

	return buf;
}

void loadjson(rapidjson::Document& doc, const std::string& filepath)
{
	const char* source = freadall(filepath);
	MNG_ASSERT_BASIC(!doc.Parse(source).HasParseError());
	delete[] source;
}

void dumpjson(const rapidjson::Document& doc, const std::string& filepath)
{
	std::ofstream stream(filepath, std::ios::trunc);
	MNG_ASSERT_SLIM(stream.is_open());
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	doc.Accept(writer);
	stream.write(sb.GetString(), sb.GetLength());
	stream.close();
}


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


b2Vec2 JsonToB2Vec(const rapidjson::Value& array, float scale)
{
	return b2Vec2(array[0].GetFloat() * scale, array[1].GetFloat() * scale);
}


sf::FloatRect JsonToRect(const rapidjson::Value& array)
{
	return sf::FloatRect(array[0].GetFloat(), array[1].GetFloat(), array[2].GetFloat(), array[3].GetFloat());
}


sf::Color JsonToColor(const rapidjson::Value& array)
{
	return sf::Color(array[0].GetUint(), array[1].GetUint(), array[2].GetUint(), array[3].GetUint());
}