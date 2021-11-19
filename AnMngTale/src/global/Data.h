#pragma once


struct Data
{
	static float masterVolume;
	static float musicVolume;
	static float soundVolume;

	static std::string language;
	static uint32_t activeFile;

	static rapidjson::Document dialogue;
	static rapidjson::Document labels;
};