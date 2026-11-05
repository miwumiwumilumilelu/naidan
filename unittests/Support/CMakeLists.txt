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
