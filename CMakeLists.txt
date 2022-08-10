include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.23.0)

set(NAME MDEngineOGL)

option(GLFW_USE_HYBRID_HPG "Force use of high-performance GPU on hybrid systems" ON)

message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

project(${NAME} VERSION 0.2.6)

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

# Add imgui
set(IMGUI_LIBRARIES "${CMAKE_SOURCE_DIR}/deps/imgui") # 1.2 Make sure lib path is correct
message(STATUS "Found IMGUI")
message(STATUS "Using glad lib at: ${IMGUI_LIBRARIES}")

    # Add implot
    set(IMPLOT_LIBRARIES "${CMAKE_SOURCE_DIR}/deps/implot") # 1.2 Make sure lib path is correct
    message(STATUS "Found IMPLOT")
    message(STATUS "Using glad lib at: ${IMPLOT_LIBRARIES}")

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


file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c*)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")
  
  file( COPY ${PROJECT_SOURCE_DIR}/shaders/shader.frag
      DESTINATION "${CMAKE_SOURCE_DIR}/build/shaders" )
  file( COPY ${PROJECT_SOURCE_DIR}/shaders/shader.vert
      DESTINATION "${CMAKE_SOURCE_DIR}/build/shaders" )
  file( COPY ${PROJECT_SOURCE_DIR}/textures 
      DESTINATION "${CMAKE_SOURCE_DIR}/build" )
  file( COPY ${PROJECT_SOURCE_DIR}/models 
      DESTINATION "${CMAKE_SOURCE_DIR}/build" )

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
    ${PROJECT_SOURCE_DIR}/deps
    ${GLM_PATH}
    ${IMGUI_LIBRARIES}
    )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${GLFW_LIB}
  )

  target_link_libraries(${PROJECT_NAME} glfw3 ${OpenGL_LIBRARY})
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
      ${PROJECT_SOURCE_DIR}/deps
      ${GLM_PATH}
      ${IMGUI_LIBRARIES}
    )
    target_link_libraries(${PROJECT_NAME} glfw ${OpenGL_LIBRARY})
endif()
