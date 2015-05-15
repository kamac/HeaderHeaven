group "Library"
      project "libjpeg"
            kind "StaticLib"
            language "C"
            targetdir "lib"
            files { "../code/libjpeg/include/*.h", "../code/libjpeg/src/*.c", "../code/libjpeg/src/*.asm" }
            configuration { "vs*" }
                  excludes { "../code/libjpeg/src/jmemmac.c", "../code/libjpeg/src/jmemdos.c" }

      project "freetype"
            kind "StaticLib"
            language "C"
            targetdir "lib"
            files { "../code/freetype/src/autofit/autofit.c", "../code/freetype/src/bdf/bdf.c", "../code/freetype/src/cff/cff.c",
                  "../code/freetype/src/base/ftbase.c", "../code/freetype/src/base/ftbitmap.c", "../code/freetype/src/cache/ftcache.c",
                  "../code/freetype/src/windows/ftdebug.c", "../code/freetype/src/base/ftfstype.c", "../code/freetype/src/base/ftgasp.c",
                  "../code/freetype/src/base/ftglyph.c", "../code/freetype/src/gzip/ftgzip.c", "../code/freetype/src/base/ftinit.c",
                  "../code/freetype/src/lzw/ftlzw.c", "../code/freetype/src/base/ftstroke.c", "../code/freetype/src/base/ftsystem.c",
                  "../code/freetype/src/smooth/smooth.c", "../code/freetype/src/base/ftbbox.c", "../code/freetype/src/base/ftgxval.c",
                  "../code/freetype/src/base/ftlcdfil.c", "../code/freetype/src/base/ftmm.c", "../code/freetype/src/base/ftotval.c",
                  "../code/freetype/src/base/ftpatent.c", "../code/freetype/src/base/ftpfr.c", "../code/freetype/src/base/ftsynth.c",
                  "../code/freetype/src/base/fttype1.c", "../code/freetype/src/base/ftwinfnt.c", "../code/freetype/src/base/ftxf86.c",
                  "../code/freetype/src/pcf/pcf.c", "../code/freetype/src/pfr/pfr.c", "../code/freetype/src/psaux/psaux.c",
                  "../code/freetype/src/pshinter/pshinter.c", "../code/freetype/src/psnames/psmodule.c", "../code/freetype/src/raster/raster.c",
                  "../code/freetype/src/sfnt/sfnt.c", "../code/freetype/src/truetype/truetype.c", "../code/freetype/src/type1/type1.c",
                  "../code/freetype/src/cid/type1cid.c", "../code/freetype/src/type42/type42.c", "../code/freetype/src/winfonts/winfnt.c",

                  "../code/freetype/include/ft2build.h", "../code/freetype/include/config/ftconfig.h", "../code/freetype/include/config/ftheader.h",
                  "../code/freetype/include/config/ftmodule.h", "../code/freetype/include/config/ftoption.h", "../code/freetype/include/config/ftstdlib.h" }

            excludes { "../code/freetype/src/tools/**" }
            flags { "noPCH" }
            defines { "FT2_BUILD_LIBRARY", "_LIB" }
            configuration { "PriDeb" }
                  defines { "FT_DEBUG_LEVEL_ERROR", "FT_DEBUG_LEVEL_TRACE" }
            configuration { "vs*" }
                  excludes { "../code/freetype/src/base/ftmac.c" }

      project "glew"
            kind "StaticLib"
            language "C"
            targetdir "lib"
            files { "../code/glew/include/GL/*.h", "../code/glew/src/*.c" }
            defines { "GLEW_STATIC" }

      --[[project "sndfile"
            kind "StaticLib"
            language "C"
            targetdir "lib"
            files { "../code/sndfile/include/*.h", "../code/sndfile/src/**.c" }
            configuration { "vs*" }
                  defines { "CPU_IS_LITTLE_ENDIAN=1", "OS_IS_WIN32=1" }
                  files { "../code/sndfile/include/windows/*.h" }
                  includedirs { "../code/sndfile/include/windows/" }--]]

      --[[project "sfml-audio"
            kind "SharedLib"
            language "C++"
            targetdir "lib"
            includedirs { "../code/SFML/src/ " }
            files { "../code/SFML/include/SFML/Audio/**.hpp", "../code/SFML/src/SFML/Audio/**.cpp" }
            defines { "SFML_AUDIO_EXPORTS", "_CRT_SECURE_NO_DEPRECATE" }
            links { "sfml-system", "openal32", "sndfile", "winmm" }--]]

      project "sfml-graphics"
            kind "StaticLib"
            language "C++"
            targetdir "lib"
            includedirs { "../code/SFML/src/ " }
            files { "../code/SFML/include/SFML/Graphics/**.hpp", "../code/SFML/src/SFML/Graphics/**.cpp" }
            defines { "GLEW_STATIC", "_CRT_SECURE_NO_DEPRECATE", "STBI_FAILURE_USERMSG", "SFML_STATIC" }
            configuration { "PriDeb" }
                  defines { "FT_DEBUG_LEVEL_ERROR", "FT_DEBUG_LEVEL_TRACE" }
            links { "sfml-window", "sfml-system", "glew", "opengl32", "freetype", "libjpeg" }

      project "sfml-main"
            kind "StaticLib"
            language "C++"
            targetdir "lib"
            includedirs { "../code/SFML/src/ " }
            files { "../code/SFML/src/SFML/Main/MainWin32.cpp" }
            defines { "_CRT_SECURE_NO_DEPRECATE", "SFML_STATIC"  }

      project "sfml-network"
            kind "StaticLib"
            language "C++"
            targetdir "lib"
            includedirs { "../code/SFML/src/ " }
            files { "../code/SFML/include/SFML/Network/**.hpp", "../code/SFML/src/SFML/Network/*.cpp" }
            defines { "_CRT_SECURE_NO_DEPRECATE", "SFML_STATIC"  }
            configuration "vs*"
                  files { "../code/SFML/src/SFML/Network/Win32/*.cpp" }
            links { "sfml-system", "ws2_32" }

      project "sfml-system"
            kind "StaticLib"
            language "C++"
            targetdir "lib"
            includedirs { "../code/SFML/src/ " }
            files { "../code/SFML/include/SFML/System/**.hpp", "../code/SFML/src/SFML/System/*.cpp" }
            defines { "_CRT_SECURE_NO_DEPRECATE", "SFML_STATIC"  }
            configuration "vs*"
                  files { "../code/SFML/src/SFML/System/Win32/*.cpp" }
                  links { "winmm" }

      project "sfml-window"
            kind "StaticLib"
            language "C++"
            targetdir "lib"
            includedirs { "../code/SFML/src/ " }
            files { "../code/SFML/include/SFML/Window/**.hpp", "../code/SFML/src/SFML/Window/*.cpp" }
            defines { "_CRT_SECURE_NO_DEPRECATE", "SFML_STATIC" }
            configuration "vs*"
                  files { "../code/SFML/src/SFML/Window/Win32/*.cpp" }
                  excludes
                  {
                        "../code/SFML/src/SFML/Window/EGLCheck.cpp", "../code/SFML/include/SFML/Window/EGLCheck.hpp",
                        "../code/SFML/src/SFML/Window/EGLContext.cpp", "../code/SFML/include/SFML/Window/EGLContext.hpp"
                  }
                  flags { "Unicode" }
                  links { "opengl32", "winmm" }
            links { "sfml-system" }