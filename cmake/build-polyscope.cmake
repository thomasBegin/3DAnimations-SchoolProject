include(FetchContent)

FetchContent_Declare(
  polyscope
  GIT_REPOSITORY https://github.com/nmwsharp/polyscope.git
  GIT_TAG        v1.3.0
  GIT_PROGRESS TRUE
)

set(POLYSCOPE_BACKEND_OPENGL_MOCK "OFF" CACHE BOOL "Enable openGL_mock backend")

FetchContent_MakeAvailable(polyscope)