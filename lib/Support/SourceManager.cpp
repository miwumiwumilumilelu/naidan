#include "naidan/Support/SourceManager.h"
#include <fstream>
#include <iterator>
#include <utility>
#include <cassert>
#include <algorithm>

namespace naidan {

std::optional<FileId> SourceManager::loadFile(
                      const std::string& path,
                      std::string &errorMessage) {
    // 输入文件流对象并以二进制模式读取文件
    std::ifstream input(path, std::ios::binary);
    
    if (!input) {
        errorMessage = "naidan cannot open source file: " + path;
        return std::nullopt;
    }

    // 将打开文件数据加载到content中
    std::string content{
                std::istreambuf_iterator<char>{input},
                std::istreambuf_iterator<char>{}};
    
    if (input.bad()) {
        errorMessage = "naidan cannot read source file: " + path;
        return std::nullopt;
    }

    return addBuffer(path, std::move(content));
}

FileId SourceManager::addBuffer(
                      std::string name,
                      std::string content) {
    SourceFile file;
    file.name = std::move(name);
    file.content = std::move(content);

    // 第1行从偏移0开始
    // 然后记录每一行的偏移
    file.lineOffsets.push_back(0);

    for (std::uint32_t offset = 0; offset < file.content.size(); ++offset) {
        if (file.content[offset] == '\n')
            file.lineOffsets.push_back(offset + 1);
    }

    FileId fileId = static_cast<FileId>(files_.size());

    files_.push_back(std::move(file));
    return fileId;
}

// 获取fd对应的SourceManager
const SourceManager::SourceFile &SourceManager::getFile(FileId fileId) const {
    assert(fileId != InvalidFileId);
    assert(fileId < files_.size());

    return files_[fileId];
}

std::string_view SourceManager::getFileName(FileId fileId) const {
    return getFile(fileId).name;
}

std::string_view SourceManager::getBuffer(FileId fileId) const {
    return getFile(fileId).content;
}

std::string_view SourceManager::getText(SourceRange range) const {
    assert(range.begin.fileId == range.end.fileId);
    assert(range.begin.offset <= range.end.offset);

    std::string_view buffer = getBuffer(range.begin.fileId);
    assert(range.end.offset <= buffer.size());

    return buffer.substr(range.begin.offset, range.end.offset - range.begin.offset);
}

LineAndColumn SourceManager::getLineAndColumn(SourceLocation location) const {
    const SourceFile &file = getFile(location.fileId);
    assert(location.offset <= file.content.size());

    // 基于二分查找，[first, last)中找到第一个大于offset的元素的迭代器
    auto nextLine = std::upper_bound(file.lineOffsets.begin(),
                                     file.lineOffsets.end(),
                                     location.offset);
    // 当前行index
    std::size_t lineIndex = static_cast<std::size_t>(nextLine - file.lineOffsets.begin() - 1);
    return {static_cast<std::uint32_t>(lineIndex + 1), location.offset - file.lineOffsets[lineIndex] + 1};
}

}