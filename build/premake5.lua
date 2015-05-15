solution "HeaderHeaven"
 
    ------------------------------------------------------------------
    -- setup common settings
    ------------------------------------------------------------------
	configurations 
	{ 
		"PriDeb",
        "PriRel",
		"PubRel"
	}

    vectorextensions "SSE2"
    floatingpoint "Fast"
    
    includedirs 
    { 
        "../code/",
        "../code/HeaderHeaven/include/"
	}
	
    location "projects"
    platforms { "x32", "x64" }
	
	configuration { "vs*"}
		buildoptions 
		{ 
			"/wd4512",
			"/wd4996",
			"/wd4018"
		}
        defines
        {
            "WIN32", "_WINDOWS"
        }
 
    ------------------------------------------------------------------
    -- setup the build configs
    ------------------------------------------------------------------
	configuration {"gmake"}
		linkoptions( "-lm -lpthread -pthread -g" ) 
		buildoptions {"-g --std=c++11" } 
	
    configuration "PriDeb"
        defines { "DEBUG", "_DEBUG" }
        flags { "Symbols" }
        targetsuffix "_d"
		
    configuration "PriRel"
        defines { "NDEBUG" }
        optimize "On"
        targetsuffix "_r"

	configuration "PubRel"
        defines { "NDEBUG", "PUBLIC_BUILD" }
        optimize "On"


    ------------------------------------------------------------------
    -- sdk projects
    ------------------------------------------------------------------
	group "Project"
		project "HeaderHeaven"
			kind "ConsoleApp"
			language "C++"
			targetdir "bin"
			files { "../code/HeaderHeaven/src/**.cpp", "../code/HeaderHeaven/include/**.h" }
            links { }