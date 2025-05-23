add_library(DLP4500SDK SHARED IMPORTED)

set_target_properties(DLP4500SDK PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_ROOT_DIR}/DLP4500/inc/"
    IMPORTED_IMPLIB "${PROJECT_ROOT_DIR}/DLP4500/lib/hidapi.lib"
    IMPORTED_LOCATION "${PROJECT_ROOT_DIR}/DLP4500/dll/hidapi.dll"  # 可选：如果你需要运行时复制 DLL
)
