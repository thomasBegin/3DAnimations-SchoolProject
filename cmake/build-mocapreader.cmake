include(FetchContent)

FetchContent_Declare(
  mocapreader
  GIT_REPOSITORY https://github.com/ETS-vis-interactive/MocapReader.git
  #GIT_TAG        e9ec8a1a9861cf578d9c6e85a6420080aa715c03
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(mocapreader)
FetchContent_GetProperties(mocapreader SOURCE_DIR mocapreader_SRC_DIR BINARY_DIR mocapreader_BIN_DIR)

include_directories(${mocapreader_SRC_DIR})