#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <box2d/box2d.h>

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <iomanip>
#include <random>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <stack>

#include <functional>
#include <optional>
#include <future>
#include <chrono>

#include <cstdlib>
#include <cstdint>
#include <cstdio>

#include <fstream>
#include <fstream>
#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


template<typename T>
using Obj = std::unique_ptr<T>;