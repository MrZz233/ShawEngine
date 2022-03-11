include "./vendor/premake/premake_customization/solution_items.lua"

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

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/ShawEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/ShawEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/ShawEngine/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/ShawEngine/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/ShawEngine/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/ShawEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/ShawEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/ShawEngine/vendor/ImGuizmo"

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