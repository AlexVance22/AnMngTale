#include "PCH.h"
#include "Data.h"


float Data::masterVolume = 100;
float Data::musicVolume = 100;
float Data::soundVolume = 100;

std::string Data::language;
uint32_t Data::activeFile;

rapidjson::Document Data::dialogue;
rapidjson::Document Data::labels;