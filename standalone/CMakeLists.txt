cmake_minimum_required(VERSION 3.14...3.22)

project(lunar_rescue_game_standalone LANGUAGES CXX)

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

CPMAddPackage(NAME lunar_rescue_game SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# Add package: shoejunk/hash
CPMAddPackage(
	NAME hash
	GITHUB_REPOSITORY shoejunk/hash
	VERSION 6.0.1
)

# Add package: shoejunk/collide
CPMAddPackage(
	NAME collide
	GITHUB_REPOSITORY shoejunk/collide
	VERSION 0.1.1
)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 OUTPUT_NAME "lunar_rescue_game")

target_link_libraries(${PROJECT_NAME} lunar_rescue_game cxxopts hash collide)
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
