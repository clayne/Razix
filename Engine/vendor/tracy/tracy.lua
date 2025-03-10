project "Tracy"
    kind "StaticLib"
    language "C++"
    cppdialect (engine_global_config.cpp_dialect)
    staticruntime "off"

    files
    {
        --"client/**.h",
        --"client/**.hpp",
        --"client/**.c",
        --"client/**.cpp",
        "Tracy.hpp",
        --"TracyC.h",
        "TracyClient.cpp"
        --"TracyD3D11.hpp",
        --"TracyD3D12.hpp",
        --"TracyLua.hpp",
        --"TracyOpenCL.hpp",
        --"TracyOpenGL.hpp",
        --"TracyVulkan.hpp"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect (engine_global_config.cpp_dialect)
        staticruntime "off"

    -- Config settings for Razix Engine project
    filter "configurations:Debug"
        defines { "RAZIX_DEBUG", "TRACY_ENABLE", "_DEBUG"}
        symbols "On"
        runtime "Debug"
        optimize "Off"

    filter "configurations:Release"
        defines { "RAZIX_RELEASE", "NDEBUG" }
        optimize "Speed"
        symbols "On"
        runtime "Release"

    filter "configurations:Distribution"
        defines { "RAZIX_DISTRIBUTION", "NDEBUG" }
        symbols "Off"
        optimize "Full"
        runtime "Release"
