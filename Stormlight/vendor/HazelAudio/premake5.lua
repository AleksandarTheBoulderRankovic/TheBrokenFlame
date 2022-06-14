project "HazelAudio"
	location "HazelAudio"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"AL_LIBTYPE_STATIC"
	}

	includedirs
	{
		"%{prj.name}/src",
		"HazelAudio/vendor/OpenAL-Soft/include",
		"HazelAudio/vendor/OpenAL-Soft/src",
		"HazelAudio/vendor/OpenAL-Soft/src/common",
		"HazelAudio/vendor/libogg/include",
		"HazelAudio/vendor/Vorbis/include",
		"HazelAudio/vendor/minimp3"
	}

	links
	{
		"OpenAL-Soft",
		"Vorbis"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "SL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SL_DIST"
		runtime "Release"
		optimize "on"