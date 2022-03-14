include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"
workspace "ShawEngine"
	architecture "x86_64"
	startproject "Shaw-Editor"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}	

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "ShawEngine/vendor/GLFW"	--将GLFW中的premake复制过来
	include "ShawEngine/vendor/Glad"
	include "ShawEngine/vendor/imgui"
	include "ShawEngine/vendor/yaml-cpp"
group ""

include "ShawEngine"
include "AppStart"
include "Shaw-Editor"