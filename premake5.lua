include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Stormlight"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Stormlight/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Stormlight/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Stormlight/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Stormlight/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Stormlight/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Stormlight/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Stormlight/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Stormlight/vendor/ImGuizmo"
IncludeDir["Box2D"] = "%{wks.location}/Stormlight/vendor/Box2D/include"
IncludeDir["HazelAudio"] = "%{wks.location}/Stormlight/vendor/HazelAudio"

group "Dependencies"
	include "vendor/premake"
	include "Stormlight/vendor/GLFW"
	include "Stormlight/vendor/Glad"
	include "Stormlight/vendor/imgui"
	include "Stormlight/vendor/yaml-cpp"
	include "Stormlight/vendor/Box2D"
	include "Stormlight/vendor/HazelAudio/HazelAudio/vendor/OpenAL-Soft"
	include "Stormlight/vendor/HazelAudio/HazelAudio/vendor/libogg"
	include "Stormlight/vendor/HazelAudio/HazelAudio/vendor/Vorbis"
	include "Stormlight/vendor/HazelAudio"
group ""

include "Stormlight"
include "Sandbox"
include "Stormlight-Editor"