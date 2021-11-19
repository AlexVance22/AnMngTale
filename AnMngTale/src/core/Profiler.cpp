#include "PCH.h"
#include "Profiler.h"

using namespace std::chrono;
using namespace std::chrono_literals;


Profiler::Profiler(bool print) : m_print(print)
{
	m_begin = high_resolution_clock::now();
	m_results.push_back(0);
}

Profiler::~Profiler()
{
	if (m_tracking)
	{
		switch (precision)
		{
		case Prec::SECONDS:
			m_results[m_results.size() - 1] += duration_cast<seconds>(high_resolution_clock::now() - m_begin).count();
			break;
		case Prec::MILLISECONDS:
			m_results[m_results.size() - 1] += duration_cast<milliseconds>(high_resolution_clock::now() - m_begin).count();
			break;
		case Prec::MICROSECONDS:
			m_results[m_results.size() - 1] += duration_cast<microseconds>(high_resolution_clock::now() - m_begin).count();
			break;
		}
	}

	if (m_print)
	{
		for (uint64_t step : m_results)
		{
			switch (precision)
			{
			case Prec::SECONDS:
				std::cout << step << "\tseconds\n";
				break;
			case Prec::MILLISECONDS:
				std::cout << step << "\tmilliseconds\n";
				break;
			case Prec::MICROSECONDS:
				std::cout << step << "\tmicroseconds\n";
				break;
			}
		}

		std::cout << '\n';
	}
}


void Profiler::step()
{
	if (m_tracking)
	{
		switch (precision)
		{
		case Prec::SECONDS:
			m_results[m_results.size() - 1] += duration_cast<seconds>(high_resolution_clock::now() - m_begin).count();
			break;
		case Prec::MILLISECONDS:
			m_results[m_results.size() - 1] += duration_cast<milliseconds>(high_resolution_clock::now() - m_begin).count();
			break;
		case Prec::MICROSECONDS:
			m_results[m_results.size() - 1] += duration_cast<microseconds>(high_resolution_clock::now() - m_begin).count();
			break;
		}
		m_begin = high_resolution_clock::now();
	}

	m_results.push_back(0);
}


void Profiler::pause()
{
	switch (precision)
	{
	case Prec::SECONDS:
		m_results[m_results.size() - 1] += duration_cast<seconds>(high_resolution_clock::now() - m_begin).count();
		break;
	case Prec::MILLISECONDS:
		m_results[m_results.size() - 1] += duration_cast<milliseconds>(high_resolution_clock::now() - m_begin).count();
		break;
	case Prec::MICROSECONDS:
		m_results[m_results.size() - 1] += duration_cast<microseconds>(high_resolution_clock::now() - m_begin).count();
		break;
	}
	m_tracking = false;
}

void Profiler::resume()
{
	m_begin = high_resolution_clock::now();
	m_tracking = true;
}