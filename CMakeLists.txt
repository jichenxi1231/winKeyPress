# 设置最低CMake版本
cmake_minimum_required(VERSION 3.27)

# 设置项目名称
project(Test)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加头文件的搜索路径
include_directories(include)

# 添加库文件
add_library(winKeyPress SHARED src/winKeyPress.cpp)  # SHARED：用于创建共享库（动态链接库）

# 设置库文件的输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 添加可执行文件
add_executable(app
#        app app.cpp appH.hpp
        app.cpp

)

# 链接库到可执行文件
target_link_libraries(app PRIVATE winKeyPress)  # 私有


########################## 编译dll ##########################
## 设置最低 CMake 版本
#cmake_minimum_required(VERSION 3.10)
#
## 设置项目名称
#project(MyLibrary)
#
## 添加共享库目标
#add_library(MyLibrary SHARED app.cpp)
#
## 定义导出宏
#target_compile_definitions(MyLibrary PRIVATE MYLIB_EXPORTS)
#
## 设置头文件路径
#target_include_directories(MyLibrary
#        PUBLIC
#        ${CMAKE_CURRENT_SOURCE_DIR}
#)
#
## 设置输出路径（可选）
#set_target_properties(MyLibrary PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
#        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
#)

