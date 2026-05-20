cmake_minimum_required(VERSION 3.15)

project(labo02-animation)

# Build library that can read bwh file format
include(build-mocapreader)

# Setup language requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure path for .bvh folder
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/bvhfolder.h.in ${CMAKE_CURRENT_BINARY_DIR}/bvhfolder.h )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

#--------------------------------------------------
# Sous MAC, OpenGL est Deprecated, mais toujours
# fonctionnel, on veut éviter tous les warnings
# durant la compilation.
#--------------------------------------------------
if (APPLE)
  add_definitions( -DGL_SILENCE_DEPRECATION )
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add .cpp and .h files
set(HEADERS skeleton.h animationfile.h animationtimer.h )
set(SOURCE labo02-animation.cpp skeleton.cpp animationfile.cpp animationtimer.cpp )
add_executable(labo02-animation ${SOURCE} ${HEADERS})

target_link_libraries(labo02-animation labo01 MocapReaderLib polyscope)
