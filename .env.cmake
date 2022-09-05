if (WIN32)
    set(GLFW_PATH /usr/local/include/GLFW)
endif()

set(GLM_PATH ./deps/glm)
set(GLAD_INCLUDE_PATH ./deps/glad)
set(STB_INCLUDE_PATH  ./deps/stb)
set(TINYOBJ_PATH  ./deps/tinyobjloader)
# Set MINGW_PATH if using mingwBuild.bat and not VisualStudio20XX
# set(MINGW_PATH "C:/Program Files (x86)/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32")

# Optional set TINYOBJ_PATH to target specific version, otherwise defaults to external/tinyobjloader
# set(TINYOBJ_PATH X:/dev/Libraries/tinyobjloader)