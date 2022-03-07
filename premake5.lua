workspace "ShawEngine"
	architecture "x86_64"
	startproject "AppStart"
	configurations{
		"Debug",
		"Release",
		"Dist"
	}
	flags
	{
		"MultiProcessorCompile"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "ShawEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "ShawEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "ShawEngine/vendor/imgui"
IncludeDir["glm"] = "ShawEngine/vendor/glm"
IncludeDir["stb_image"] = "ShawEngine/vendor/stb_image"

group "Dependencies"
	include "ShawEngine/vendor/GLFW"	--将GLFW中的premake复制过来
	include "ShawEngine/vendor/Glad"
	include "ShawEngine/vendor/imgui"
group ""

project "ShawEngine"
	location "ShawEngine"
	kind "StaticLib"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/"..outputdir.."/%{prj.name}")
	objdir("bin-int/"..outputdir.."/%{prj.name}")
	
	pchheader "sepch.h"
	pchsource "ShawEngine/src/sepch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",	
	}

	filter "system:windows"
		systemversion "latest"

		--postbuildcommands{
			--("{COPY} %{cfg.buildtarget.relpath} ../bin/"..outputdir.."/AppStart")
			--("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/AppStart/\"")
		--}

	filter "configurations:Debug"
		defines "SE_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "SE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SE_DIST"
		runtime "Release"
		optimize "on"

project "AppStart"
	location "AppStart"
	kind "ConsoleApp"
	cppdialect "C++17"
	staticruntime "on"

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
		systemversion "latest"

	filter "configurations:Debug"
		defines "SE_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "SE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SE_DIST"
		runtime "Release"
		optimize "on"

project "Shaw-Editor"
	location "Shaw-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"ShawEngine/vendor/spdlog/include",
		"ShawEngine/src",
		"ShawEngine/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"ShawEngine"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "SE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SE_DIST"
		runtime "Release"
		optimize "on"