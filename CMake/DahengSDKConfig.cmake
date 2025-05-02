add_library(DahengSDK SHARED IMPORTED)
set_target_properties(
    DahengSDK PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_ROOT_DIR}/GalaxySDK/Samples/C++ SDK/inc/"
    IMPORTED_IMPLIB "${PROJECT_ROOT_DIR}/GalaxySDK/Samples/C++ SDK/lib//x64/*.lib"
)
#file(GLOB_RECURSE DahengSDKDlls "${PROJECT_ROOT_DIR}/GalaxySDK/Samples/C++ SDK/bin/*.dll")