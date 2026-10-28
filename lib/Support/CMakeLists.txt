# 创建NaidanSupport静态库，并将.cpp编译进这个库
add_naidan_library(NaidanSupport
    SourceManager.cpp
)
# 提供统一名称Naidan::Support，供模块链接
add_library(Naidan::Support ALIAS NaidanSupport)