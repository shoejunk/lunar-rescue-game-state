cmake_minimum_required(VERSION 3.14...3.22)

project(lunar_rescue_game_state_standalone LANGUAGES CXX)

if(MSVC)
	# I haven't figured out how to resolve this warning when importing std.core.
    add_compile_options("/wd5050")
endif()

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 3.0.0
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(NAME lunar_rescue_game_state SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# Add package: shoejunk/input
CPMAddPackage(
	NAME input
	GITHUB_REPOSITORY shoejunk/input
	#VERSION 1.5.0
	GIT_TAG main
)

# Add package: shoejunk/log
CPMAddPackage(
	NAME log
	GITHUB_REPOSITORY shoejunk/log
	#VERSION 1.1.0
	GIT_TAG main
)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 OUTPUT_NAME "lunar_rescue_game_state")

target_link_libraries(${PROJECT_NAME} lunar_rescue_game_state cxxopts input log)
