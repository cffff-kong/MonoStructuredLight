set(TARGET_NAME DLPOperation)
# 配置资源文件
add_library(${TARGET_NAME} SHARED)
# add_executable(${TEST_NAME}
#     ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp  # 你 EXE 的入口文件
# )
# # EXE 链接到 DLL
# target_link_libraries(${TEST_NAME} PRIVATE ${TARGET_NAME})
file(GLOB CURRENT_DIR_HEADERS "*.h")
file(GLOB CURRENT_DIR_SRCS "*.cpp")
target_sources(${TARGET_NAME} PRIVATE
    ${CURRENT_DIR_HEADERS}
    ${CURRENT_DIR_SRCS}
)

target_include_directories(${TARGET_NAME}
    PUBLIC
    $<INSTALL_INTERFACE:include>
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(${TARGET_NAME} PUBLIC 
DLP4500SDK
)
set_windows_md_flag(${TARGET_NAME})

