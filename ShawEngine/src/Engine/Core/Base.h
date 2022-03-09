#pragma once

#include <memory>
#include "Engine/Core/PlatformDetection.h"

#ifdef SE_DEBUG
	#if defined(SE_PLATFORM_WINDOWS)
		#define SE_DEBUGBREAK() __debugbreak()
	#elif defined(SE_PLATFORM_LINUX)
		#include <signal.h>
		#define SE_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
		#define SE_ENABLE_ASSERTS
	#else
		#define SE_DEBUGBREAK()
#endif


#define SE_EXPAND_MACRO(x) x
#define SE_STRINGIFY_MACRO(x) #x

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

#include "Engine/Core/Log.h"
#include "Engine/Core/Assert.h"