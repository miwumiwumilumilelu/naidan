# Public headers are exposed through this interface target. Libraries in lib/
# link against Naidan::Headers instead of repeating include directories.
add_library(NaidanHeaders INTERFACE)
add_library(Naidan::Headers ALIAS NaidanHeaders)

target_include_directories(NaidanHeaders
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_compile_features(NaidanHeaders
  INTERFACE
    cxx_std_17
)
