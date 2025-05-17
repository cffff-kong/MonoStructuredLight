set(TARGET_NAME ClrImport)
# 配置资源文件
add_library(${TARGET_NAME} SHARED)

file(GLOB CURRENT_DIR_HEADERS "*.h")
file(GLOB CURRENT_DIR_SRCS "*.cpp")
target_sources(${TARGET_NAME} PRIVATE
    ${OTHER_DIR_HEADERS}
    ${CURRENT_DIR_HEADERS}
    ${CURRENT_DIR_SRCS}
    ${encryption_DIR_HEADERS}
    ${encryption_DIR_SRCS}
)

# Opencv
set(OpenCV_DIR "D:/opencv4.7/opencv/build/x64/vc16/lib") #OpenCVConfig路径
find_package(OpenCV REQUIRED)
target_include_directories(${TARGET_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})


target_include_directories(${TARGET_NAME}
    PUBLIC
    $<INSTALL_INTERFACE:include>
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(${TARGET_NAME} PUBLIC 
${OpenCV_LIBS}
CameraOperation
DLPOperation
SSLReconstruction
)
set_windows_md_flag(${TARGET_NAME})

target_link_options(${TARGET_NAME}
    PUBLIC $<$<OR:$<CONFIG:>,$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:/DEBUG /ASSEMBLYDEBUG>
)
# Define the assembly to be mixed-mode (managed + unmanaged).
set_property(TARGET ${TARGET_NAME} PROPERTY COMMON_LANGUAGE_RUNTIME "")
 
# Set CLR assembly properties.
set_target_properties(${TARGET_NAME} PROPERTIES
    VS_DOTNET_REFERENCES "System;System.Core"
    VS_GLOBAL_ROOTNAMESPACE ${TARGET_NAME}
    VS_GLOBAL_EnableUnmanagedDebugging "true"
)