cmake_minimum_required (VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE "D:/develop/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_NAME ylox)

project (${PROJECT_NAME})

# find_package example
# find_package(Qt5 COMPONENTS Core Gui Widgets)

set(HEADERS
    "src/stdafx.h"
    "src/tools.h"
    "src/ast.h"
    "src/scanner.h"
    "src/parser.h"
    "src/ast_print.h"
)

set(SOURCES
    "src/main.cpp"
    "src/scanner.cpp"
    "src/parser.cpp"
    "src/ast_print.cpp"
)

# 将源代码添加到此项目的可执行文件。
add_executable (${PROJECT_NAME}  
    ${HEADERS}
    ${SOURCES}
    )

target_precompile_headers(
    ${PROJECT_NAME} PRIVATE "src/stdafx.h"
    )

# link example
# target_link_libraries(${PROJECT_NAME} 
# 	Qt5::Core 
# )

# 包含子项目。
# add_subdirectory("event_test")

