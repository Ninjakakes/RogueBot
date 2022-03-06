# CMakeList.txt : CMake project for RogueBot, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (RogueBot "main.cpp"  "Engine.h" "Engine.cpp" )

find_package(libtcod CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(RogueBot PRIVATE SDL2::SDL2 SDL2::SDL2main libtcod::libtcod)

configure_file(LN_EGA8x8.png LN_EGA8x8.png COPYONLY)
