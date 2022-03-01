workspace "ShawEngine"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "ShawEngine/vendor/GLFW/include"

include "ShawEngine/vendor/GLFW"		--将GLFW中的premake复制过来

project "ShawEngine"
	location "ShawEngine"
	kind "SharedLib"
	language "C++"

	targetdir("bin/"..outputdir.."/%{prj.name}")
	objdir("bin-int/"..outputdir.."/%{prj.name}")
	
	pchheader "sepch.h"
	pchsource "ShawEngine/src/sepch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"SE_PLATFORM_WINDOWS",
			"SE_BUILD_DLL",
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/AppStart")
		}

	filter "configurations:Debug"
		defines "SE_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "SE_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "SE_DIST"
		symbols "On"

project "AppStart"
	location "AppStart"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/"..outputdir.."/%{prj.name}")
	objdir("bin-int/"..outputdir.."/%{prj.name}")
	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"ShawEngine/vendor/spdlog/include",
		"ShawEngine/src"
	}

	links{
		"ShawEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"SE_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "SE_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "SE_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "SE_DIST"
		symbols "On"