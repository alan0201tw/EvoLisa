workspace "EvoLisa"
    architecture "x64"
    startproject "EvoLisa"

    configurations
    {
        -- only debug for this hobby project
        "Debug",
        "Release"
    }

-- variables
    -- cfg - configuration
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "EvoLisa"
    location "."
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "include/**.hpp",
        "src/**.cpp"
    }

    includedirs
    {
        "include",
        "vendor/include"
    }

    libdirs
    {
		"vendor/lib"
    }

    filter "system:linux"
        cppdialect "C++17"
        systemversion "latest"
        toolset "clang"
        links
        {
            "GL",
            "glfw",
            "dl"
        }

    -- everything under this filter only applies to windows
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS",
            "_USE_MATH_DEFINES"
        }

        linkoptions
        {
            "opengl32.lib glfw3.lib"
        }

    filter { "configurations:Debug" }
        symbols "On"
    
    filter { "configurations:Release" }
        optimize "On"