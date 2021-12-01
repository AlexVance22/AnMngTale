#pragma once


#ifdef MNG_DEBUG

	#define MNG_ASSERT_BASIC(condition) assert(condition)
	#define MNG_ASSERT_MSG(condition, message) if (!(condition)) { std::cerr << message << '\n'; assert(false); }
	#define MNG_ASSERT_SLIM(condition) assert(condition)

#else

	#define MNG_ASSERT_BASIC(condition) condition
	#define MNG_ASSERT_MSG(condition, message) condition
	#define MNG_ASSERT_SLIM(condition)

#endif