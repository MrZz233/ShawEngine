#pragma once

#include "Engine/Core/PlatformDetection.h"
#ifdef SE_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <array>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Engine/Core/Base.h"
#include "Engine/Core/Log.h"
#include "Engine/Debug/Instrumentor.h"

#ifdef SE_PLATFORM_WINDOWS
	#include <windows.h>
#endif
