workspace "CAutomata"
        configurations {"Debug", "Release", "Dist"}
        architecture "x64"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
macro_prefix = "CA"
project "CAutomata"
        location "CAutomata"
        kind "ConsoleApp"
        language "C"
        targetdir ("out/bin/" .. outputdir .. "/%{prj.name}")
        objdir    ("out/int/" .. outputdir .. "/%{prj.name}")
        includedirs {
           "%{prj.name}/vendor/raylib/include",
           "%{prj.name}/vendor/stb",
           "%{prj.name}/vendor/gifenc/include",
        }
        libdirs {
           "%{prj.name}/vendor/raylib/lib",
           "%{prj.name}/vendor/gifenc/lib"
        }
        links {
           "raylib",
           "gifenc"
        }
        files {
           "%{prj.name}/src/**.h",
           "%{prj.name}/src/**.c",
        }
        filter "configurations:Debug"
            defines {
                macro_prefix .. "_TARGET_DEBUG"
            }
            symbols "On"
            optimize "Off"
        filter "configurations:Release"
            defines {
                macro_prefix .. "_TARGET_RELEASE"
            }
            symbols "On"
            optimize "On"
        filter "configurations:Dist"
            defines {
                macro_prefix .. "_TARGET_DIST"
            }
            symbols "Off"
            optimize "On"
        filter "system:Windows"
            defines {
                macro_prefix .. "_PLATFORM_WINDOWS"
            }
            links {
                "winmm"
            }
        filter "system:Linux"
            defines {
                macro_prefix .. "_PLATFORM_LINUX"
            }
        
        filter "system:Android"
            defines {
                macro_prefix .. "_PLATFORM_ANDROID"
            }
