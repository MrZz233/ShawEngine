workspace "ShawEngine"
	architecture "x64"
	startproject "AppStart"
	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "ShawEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "ShawEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "ShawEngine/vendor/imgui"
IncludeDir["glm"] = "ShawEngine/vendor/glm"

group "Dependencies"
	include "ShawEngine/vendor/GLFW"	--将GLFW中的premake复制过来
	include "ShawEngine/vendor/Glad"
	include "ShawEngine/vendor/imgui"
group ""

project "ShawEngine"
	location "ShawEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir("bin/"..outputdir.."/%{prj.name}")
	objdir("bin-int/"..outputdir.."/%{prj.name}")
	
	pchheader "sepch.h"
	pchsource "ShawEngine/src/sepch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",	
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"SE_PLATFORM_WINDOWS",
			"SE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands{
			--("{COPY} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/AppStart")
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/AppStart/\"")
		}

	filter "configurations:Debug"
		defines "SE_DEBUG"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "SE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "SE_DIST"
		runtime "Release"
		optimize "On"

project "AppStart"
	location "AppStart"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir("bin/"..outputdir.."/%{prj.name}")
	objdir("bin-int/"..outputdir.."/%{prj.name}")
	
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"ShawEngine/vendor/spdlog/include",
		"ShawEngine/src",
		"ShawEngine/vendor",
		"%{IncludeDir.glm}"
	}

	links{
		"ShawEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"SE_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "SE_DEBUG"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		defines "SE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "SE_DIST"
		runtime "Release"
		optimize "On"