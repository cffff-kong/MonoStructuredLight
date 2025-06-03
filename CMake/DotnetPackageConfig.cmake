find_program(NUGET nuget)
if (NOT NUGET)
    message(FATAL_ERROR "Unable to find the nuget CLI tool. Please install it from https://www.nuget.org/downloads and ensure it has been added to the PATH")
endif()

execute_process(COMMAND
    ${NUGET} restore packages.config -OutputDirectory build/DotnetPackages
    WORKING_DIRECTORY ${PROJECT_ROOT_DIR}
    RESULT_VARIABLE ret)
if (NOT ret EQUAL 0)
    message(FATAL_ERROR "Failed to install nuget package")
endif()


set(SCOTTPLOT_LIB_PATH "${PROJECT_ROOT_DIR}/build/DotnetPackages/ScottPlot.4.1.59/lib/net462")
set(SCOTTPLOT_WINFORM_LIB_PATH "${PROJECT_ROOT_DIR}/build/DotnetPackages/ScottPlot.WinForms.4.1.59/lib/net461")
set(NPOI_LIB_PATH "${PROJECT_ROOT_DIR}/build/DotnetPackages/NPOI.2.5.6/lib/net45")
set(Json_LIB_PATH "${PROJECT_ROOT_DIR}/build/DotnetPackages/Newtonsoft.Json.13.0.1/lib/net45")
set(Aliyun_LIB_PATH "${PROJECT_ROOT_DIR}/build/DotnetPackages/Aliyun.OSS.SDK.2.13.0/lib/net45")
set(ACTIVIZ_NET_X64_PATH "${PROJECT_ROOT_DIR}/build/DotnetPackages/Activiz.NET.x64.5.8.0/lib/net20")
set(WINDOWS_API_CODE_PACK_SHELL_PATH "${PROJECT_ROOT_DIR}/build/DotnetPackages/WindowsAPICodePack-Shell.1.1.1/lib")
set(WINDOWS_API_CODE_PACK_CORE_PATH "${PROJECT_ROOT_DIR}/build/DotnetPackages/WindowsAPICodePack-Core.1.1.1/lib")