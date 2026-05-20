cmake_minimum_required(VERSION 3.15)
project(GTI320-labo02-animation)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include( build-google-test )
include( build-polyscope )
  
add_subdirectory(labo01-lib-mat)
add_subdirectory(labo02-animation)