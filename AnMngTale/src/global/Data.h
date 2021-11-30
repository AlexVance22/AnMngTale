#pragma once


struct Data
{
	static std::string language;
	static uint32_t activeFile;

	static rapidjson::Document dialogue;
	static rapidjson::Document labels;
};