#pragma once


class Callback
{
private:
	std::function<void()> m_func;

	template <typename Func, typename... Args>
	static void invokeFunc(Func&& func, Args&&... args)
	{
		std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
	}

public:
	Callback() : m_func([]() {})
	{}

	template <typename Func, typename... Args, typename std::enable_if_t<std::is_convertible<Func, std::function<void(const Args&...)>>::value>* = nullptr>
	void bind(const Func& func, const Args&... args)
	{
		if constexpr (sizeof...(Args) == 0)
			m_func = func;
		else
			m_func = [=]() { invokeFunc(func, args...); };
	}

	void operator()() { m_func(); }
};