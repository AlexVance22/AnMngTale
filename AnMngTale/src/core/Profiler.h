#pragma once


class Profiler
{
private:
	std::chrono::steady_clock::time_point m_begin;
	std::vector<uint64_t> m_results;
	bool m_tracking = true;
	bool m_print;

public:
	enum class Prec { SECONDS, MILLISECONDS, MICROSECONDS };
	Prec precision = Prec::MILLISECONDS;

public:
	Profiler(bool print = true);
	~Profiler();

	const std::vector<uint64_t>& data() const { return m_results; }

	void step();

	void pause();
	void resume();
};


#ifdef MNG_DEBUG
	#define PROFILE_DEBUG_ONLY_BEGIN(print, prec) Profiler __prof(print); __prof.precision = Profiler::Prec::prec
	#define PROFILE_DEBUG_ONLY_STEP() __prof.step()
#else
	#define PROFILE_DEBUG_ONLY_BEGIN(print, prec)
	#define PROFILE_DEBUG_ONLY_STEP()
#endif