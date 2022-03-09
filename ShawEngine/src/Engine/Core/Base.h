#pragma once

#include <memory>
#include "Engine/Core/PlatformDetection.h"


#ifdef SE_DEBUG
	#define SE_ENABLE_ASSERTS
#endif

#ifdef SE_ENABLE_ASSERTS
	#define SE_CLIENT_ASSERT(x, ...) { if(!(x)) { SE_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define SE_CORE_ASSERT(x, ...) { if(!(x)) { SE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SE_CLIENT_ASSERT(x, ...)
	#define SE_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1<<x)

#define SE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
namespace ShawEngine {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		//std::forward<Args>(args)... 表示将args中的元素全部扩展成std::forward<Args>类型的变量
		//例如  CreateScope<Class_A>(float,char,double)
		//会返回 unique_ptr<Class_A>(forward(float),forward(char),forward(double));
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}