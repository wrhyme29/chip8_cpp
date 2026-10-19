include(FetchContent)
FetchContent_Declare(
  googletest
  # tag v1.18.x
  URL https://github.com/google/googletest/archive/063de7e9578f82b369302001269680b4b1553359.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

FetchContent_MakeAvailable(googletest)
