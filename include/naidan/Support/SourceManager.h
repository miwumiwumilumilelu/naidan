#ifndef NAIDAN_SUPPORT_SOURCE_MANAGER_H
#define NAIDAN_SUPPORT_SOURCE_MANAGER_H

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <string_view>

namespace naidan {

using FileId = std::uint32_t;
inline constexpr FileId InvalidFileId = static_cast<FileId>(-1);

struct SourceLocation {
    FileId fileId = InvalidFileId;
    std::uint32_t offset = 0;
    
    bool isValid() const {
        return fileId != InvalidFileId;
    }
};

struct SourceRange {
    SourceLocation begin;
    SourceLocation end;
};

struct LineAndColumn {
    std::uint32_t line = 1;
    std::uint32_t column = 1;
};

class SourceManager {
public:
    std::optional<FileId> loadFile(
                          const std::string &path,
                          std::string &errorMessage);
    FileId addBuffer(
           std::string name,
           std::string content);
    std::string_view getFileName(FileId fileId) const;
    std::string_view getBuffer(FileId fileId) const;
    std::string_view getText(SourceRange range) const;
    LineAndColumn getLineAndColumn(SourceLocation location) const;

private:
    struct SourceFile {
        std::string name;
        std::string content;
        std::vector<std::uint32_t> lineOffsets;
    };

    const SourceFile &getFile(FileId fileId) const;

    std::vector<SourceFile> files_;
};

} // namespace naidan

#endif
