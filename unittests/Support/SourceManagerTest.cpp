#include "naidan/Support/SourceManager.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

#define CHECK(condition)                                                       \
  do {                                                                         \
    if (!(condition)) {                                                        \
      std::cerr << __FILE__ << ':' << __LINE__                                 \
                << ": CHECK failed: " << #condition << '\n';                   \
      return 1;                                                                \
    }                                                                          \
  } while (false)

int main() {
  //------------------测试内存文件写入--------------------
  naidan::SourceManager sourceManager;

  naidan::FileId fileId =
      sourceManager.addBuffer("test.sy", "int a;\nreturn a;\n");

  CHECK(fileId == 0);
  CHECK(sourceManager.getFileName(fileId) == "test.sy");
  CHECK(sourceManager.getBuffer(fileId) == "int a;\nreturn a;\n");

  naidan::SourceRange range{{fileId, 0}, {fileId, 3}};

  CHECK(sourceManager.getText(range) == "int");

  naidan::LineAndColumn position = sourceManager.getLineAndColumn({fileId, 9});

  CHECK(position.line == 2);
  CHECK(position.column == 3);

  //------------------测试边界位置--------------------
  naidan::FileId boundaryFileId =
      sourceManager.addBuffer("boundary.sy", "a\n\nxyz");

  // 连续 addBuffer 时 FileId 应递增。
  CHECK(boundaryFileId == fileId + 1);

  // offset 为 0。
  position = sourceManager.getLineAndColumn({boundaryFileId, 0});
  CHECK(position.line == 1);
  CHECK(position.column == 1);

  // offset 正好位于换行符。
  position = sourceManager.getLineAndColumn({boundaryFileId, 1});
  CHECK(position.line == 1);
  CHECK(position.column == 2);

  // offset 等于文件长度，且文件末尾没有换行符。
  position = sourceManager.getLineAndColumn({boundaryFileId, 6});
  CHECK(position.line == 3);
  CHECK(position.column == 4);

  //------------------测试磁盘文件加载--------------------
  std::filesystem::path testPath =
      std::filesystem::temp_directory_path() / "naidan-source-manager-test.sy";

  {
    std::ofstream output(testPath, std::ios::binary | std::ios::trunc);

    CHECK(output.is_open());

    output << "int main() {\n"
              "  return 0;\n"
              "}\n";

    CHECK(output.good());
  }

  std::string errorMessage;

  auto loadedFileId = sourceManager.loadFile(testPath.string(), errorMessage);

  CHECK(loadedFileId.has_value());

  if (loadedFileId.has_value()) {
    CHECK(sourceManager.getBuffer(*loadedFileId).find("int main") !=
          std::string_view::npos);
  }

  //------------------测试失败路径--------------------
  std::error_code removeError;
  CHECK(std::filesystem::remove(testPath, removeError));
  CHECK(!removeError);

  std::string missingFileError;

  auto missingFileId =
      sourceManager.loadFile(testPath.string(), missingFileError);

  CHECK(!missingFileId.has_value());
  CHECK(!missingFileError.empty());

  return 0;
}

#undef CHECK