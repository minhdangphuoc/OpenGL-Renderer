include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.23.0)

set(NAME MDEngineOGL)

message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

project(${NAME} VERSION 0.1.0)

# 2. Set GLFW_PATH in .env.cmake to target specific glfw
if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path specified in .env")
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  if (MSVC)
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2019") # 2.1 Update lib-vc2019 to use same version as your visual studio
  elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    message(STATUS "USING MINGW")
    set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64") # 2.1 make sure matches glfw mingw subdirectory
  endif()
else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_LIB glfw)
  message(STATUS "Found GLFW")
endif()
if (NOT GLFW_LIB)
  message(FATAL_ERROR "Could not find glfw library!")
else()
	message(STATUS "Using glfw lib at: ${GLFW_LIB}")
endif()

include_directories(external)

# 3. Set STB_INCLUDE_PATH in .env.cmake to target specific STB version
if (DEFINED STB_INCLUDE_PATH)
  set(STB_LIBRARIES "${STB_INCLUDE_PATH}") # 1.2 Make sure lib path is correct
  set(STB_FOUND "True")
endif()
if (NOT STB_FOUND)
	message(FATAL_ERROR "Could not find stb library!")
else()
	message(STATUS "Using stb lib at: ${STB_LIBRARIES}")
endif()

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
message(STATUS "Found OpenGL: $ENV{OpenGL_SDK}")

# ASSIMP
if (DEFINED ASSIMP_INCLUDE_PATH)
  set(ASSIMP_LIB "${ASSIMP_INCLUDE_PATH}") # 1.2 Make sure lib path is correct
  set(ASSIMP_FOUND "True")
else()
  find_package(assimp REQUIRED)
  set(ASSIMP_LIB assimp)
  message(STATUS "Found ASSIMP")
endif()
if (NOT ASSIMP_LIB)
	message(FATAL_ERROR "Could not find assimp library!")
else()
	message(STATUS "Using assimp lib at: ${ASSIMP_LIB}")
endif()

# Add Glad
if (DEFINED GLAD_INCLUDE_PATH)
  set(GLAD_LIBRARIES "${CMAKE_SOURCE_DIR}${GLAD_INCLUDE_PATH}") # 1.2 Make sure lib path is correct
  set(GLAD_FOUND "True")
else()
  find_package(glad REQUIRED)
  set(GLAD_LIBRARIES glad)
  message(STATUS "Found Glad")
endif()
if (NOT GLAD_FOUND)
	message(FATAL_ERROR "Could not find glad library!")
else()
	message(STATUS "Using glad lib at: ${GLAD_LIBRARIES}")
endif()


# Copy texture and shaders to build dir
find_path(TEXTURES_DIR_FOUND ${CMAKE_SOURCE_DIR}/build/Debug/textures)
if (NOT TEXTURES_DIR_FOUND)
  file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/Debug/textures)
endif()

find_path(SHADERS_DIR_FOUND ${CMAKE_SOURCE_DIR}/build/Debug/shaders)
if (NOT SHADERS_DIR_FOUND)
  file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/Debug/shaders)
endif()

find_path(MODELS_DIR_FOUND ${CMAKE_SOURCE_DIR}/build/Debug/shaders)
if (NOT MODELS_DIR_FOUND)
  file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/build/Debug/models)
endif()

#file( COPY ${PROJECT_SOURCE_DIR}/shaders/shader.frag.spv 
#      DESTINATION "${CMAKE_SOURCE_DIR}/build/Debug/shaders" )
#file( COPY ${PROJECT_SOURCE_DIR}/shaders/shader.vert.spv 
#     DESTINATION "${CMAKE_SOURCE_DIR}/build/Debug/shaders" )
#file( COPY ${PROJECT_SOURCE_DIR}/textures 
#      DESTINATION "${CMAKE_SOURCE_DIR}/build/Debug" )
#file( COPY ${PROJECT_SOURCE_DIR}/models 
#      DESTINATION "${CMAKE_SOURCE_DIR}/build/Debug" )
      
# If TINYOBJ_PATH not specified in .env.cmake, try fetching from git repo
if (NOT TINYOBJ_PATH)
  message(STATUS "TINYOBJ_PATH not specified in .env.cmake, using external/tinyobjloader")
  set(TINYOBJ_PATH external/tinyobjloader)
endif()

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c*)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")

  if (USE_MINGW)
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/include
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/lib
    )
  endif()

  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${TINYOBJ_PATH}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_PATH}
    ${STB_INCLUDE_PATH}
    ${GLAD_LIBRARIES}
    ${ASSIMP_LIB}
    ${OpenGL_LIBRARY}
    ${GLAD_LIBRARIES}
    ${GLAD_INCLUDE_PATH}
    )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${GLFW_LIB}
    ${STB_LIBRARIES}
    ${GLAD_LIBRARIES}
  )

  target_link_libraries(${PROJECT_NAME} glfw3 ${ASSIMP_LIB} ${OpenGL_LIBRARY})
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
      ${TINYOBJ_PATH}
      ${GLM_PATH}
      ${STB_INCLUDE_PATH}
      ${GLAD_LIBRARIES}
      ${GLAD_INCLUDE_PATH}
    )
    target_link_libraries(${PROJECT_NAME} glfw  ${ASSIMP_LIB} ${OpenGL_LIBRARY})
endif()