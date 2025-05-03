set(TARGET_NAME MSLGUI)

#包含一些CSharp相关的配置与方法
include(CSharpUtilities)
#指定.net版本
SET(CMAKE_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2")
#配置源文件
file(GLOB PROPERTY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Properties/*.*)
file(GLOB CS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cs*)
file(GLOB RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.resx*)
#生成执行文件，将所有源文件添加到项目中
add_executable(${TARGET_NAME}
    App.config
    ${PROPERTY_FILES}
    ${CS_FILES}
    ${RESOURCE_FILES}
)
#不这样设置生成的是个控制台
set_property(TARGET ${TARGET_NAME} PROPERTY WIN32_EXECUTABLE TRUE)

# 设置C#项目的ApplicationIcon图标
#set_property(TARGET ${TARGET_NAME} PROPERTY VS_GLOBAL_ApplicationIcon "${PROJECT_ROOT_DIR}/ico/VisionResProcessing.ico")

#设置源文件的依赖关系：会自动解析，对于Designer、resx文件，自动搜索同名但结尾为cs的文件，建立依赖关系！！！
csharp_set_windows_forms_properties(
    ${CS_FILES}
    ${PROPERTY_FILES}
    ${RESOURCE_FILES}
)
#属性配置
set_target_properties(${TARGET_NAME} PROPERTIES
    #根命名空间
    VS_GLOBAL_RootNamespace ${TARGET_NAME}
    VS_GLOBAL_WarningLevel "4"
    VS_GLOBAL_DefineConstants "DEBUG;TRACE"
    VS_GLOBAL_FileAlignment "512"
    VS_GLOBAL_Deterministic "true"
    VS_GLOBAL_AutoGenerateBindingRedirects "true"
    #设置允许不安全的代码块
    VS_GLOBAL_ALLOWunsafeBlocks "true"
    VS_GLOBAL_DocumentationFile "${TARGET_NAME}.xml"
    #设置链接器语言
    LINKER_LANGUAGE "CSharp"
)

target_link_libraries(${TARGET_NAME} 
ClrImport
)

#链接第三方CSharp库和.Net库
set_property(TARGET ${TARGET_NAME} PROPERTY VS_DOTNET_REFERENCES
   "Microsoft.CSharp"
   "Microsoft.VisualBasic"
   "System"
   "System.Core"
   "System.Data"
   "System.Deployment"
   "System.Drawing"
   "System.Net.Http"
   "System.Windows.Forms"
   "System.Windows.Forms.DataVisualization"
   "System.Numerics"
   "System.Numerics.Vectors"
   "System.Data.DataSetExtensions"
   "System.Xml"
   "System.Xml.Linq"
   "System.Buffers"
   "System.Memory"
   "System.ValueTuple"
   "System.IO.Compression"
   "System.IO.Compression.FileSystem"
    ${FSLEGUI_EXTERNAL_REFRENCES}
)

# 指定CSharp模块的目标生成平台
if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    set(DOTNET_GENERATOR_PLATFORM "x64")
elseif(CMAKE_GENERATOR_PLATFORM STREQUAL "Win32" OR CMAKE_GENERATOR_PLATFORM STREQUAL "")
    set(DOTNET_GENERATOR_PLATFORM "x86")
else()
    message(FATAL_ERROR "Generator platform is set to '${CMAKE_GENERATOR_PLATFORM}', which is not supported by managed projects")
endif()

target_compile_options(${TARGET_NAME} PRIVATE "/platform:${DOTNET_GENERATOR_PLATFORM}")

set_windows_md_flag(${TARGET_NAME})

# 设置C#项目的pdb文件生成，便于调试
set(CMAKE_CSharp_FLAGS_RELEASE "/debug:pdbonly")

resolve_executable_dependencies(${TARGET_NAME})


# # 声明该模块要被导出发布
# set_property(TARGET ${TARGET_NAME} PROPERTY "TARGET_EXPORTED" TRUE)

# # 执行基本的install 动作
# export_and_install_target(${TARGET_NAME})

# # 设置C#项目的pdb文件生成，便于调试（可打断点）
# set(CMAKE_CSharp_FLAGS_RELEASE "/debug:pdbonly")

# # xml文件移动
# install(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/${TARGET_NAME}.xml" DESTINATION "$<$<CONFIG:Debug>:bind>$<$<NOT:$<CONFIG:Debug>>:bin>")