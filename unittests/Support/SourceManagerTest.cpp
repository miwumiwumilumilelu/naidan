#include "naidan/Support/SourceManager.h"

#include <cassert>

int main() {
//------------------测试文件写入--------------------
  naidan::SourceManager sourceManager;

  naidan::FileId fileId = sourceManager.addBuffer(
      "test.sy",
      "int a;\nreturn a;\n");

  assert(fileId == 0);
  assert(sourceManager.getFileName(fileId) == "test.sy");
  assert(sourceManager.getBuffer(fileId) ==
         "int a;\nreturn a;\n");

  naidan::SourceRange range{
      {fileId, 0},
      {fileId, 3}
  };

  assert(sourceManager.getText(range) == "int");

  naidan::LineAndColumn position =
      sourceManager.getLineAndColumn({fileId, 9});

  assert(position.line == 2);
  assert(position.column == 3);

//------------------测试文件加载--------------------
std::string errorMessage;

auto loadedFileId = sourceManager.loadFile(
    "functional/00_main.sy",
    errorMessage);

assert(loadedFileId.has_value());
assert(sourceManager.getBuffer(*loadedFileId).find("int main") !=
       std::string_view::npos);

//------------------测试失败路径--------------------
std::string missingFileError;

auto missingFileId = sourceManager.loadFile(
    "functional/does_not_exist.sy",
    missingFileError);

assert(!missingFileId.has_value());
assert(!missingFileError.empty());

  return 0;
}