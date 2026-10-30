add_executable(SourceManagerTest
  SourceManagerTest.cpp
)

target_link_libraries(SourceManagerTest
  PRIVATE
    Naidan::Support
)

add_test(
  NAME SourceManagerTest
  COMMAND SourceManagerTest
)

set_tests_properties(SourceManagerTest
  PROPERTIES
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)