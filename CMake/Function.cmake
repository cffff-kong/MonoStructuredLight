
macro(set_windows_md_flag TARGET_NAME)
    if(WIN32 AND MSVC)
        set_property(TARGET ${TARGET_NAME} PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endmacro(set_windows_md_flag TARGET_NAME)

macro(resolve_executable_dependencies TARGET_NAME)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:${TARGET_NAME}>
        $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND_EXPAND_LISTS)
endmacro(resolve_executable_dependencies TARGET_NAME)

function(get_all_cmake_targets out_var current_dir)
    get_property(targets DIRECTORY ${current_dir} PROPERTY BUILDSYSTEM_TARGETS)
    get_property(subdirs DIRECTORY ${current_dir} PROPERTY SUBDIRECTORIES)

    foreach(subdir ${subdirs})
        get_all_cmake_targets(subdir_targets ${subdir})
        list(APPEND targets ${subdir_targets})
    endforeach()

    set(${out_var} ${targets} PARENT_SCOPE)
endfunction()

function(generate_dependency_graph OUTPUT_PATH GENERATE_EXTERNAL_LIB_DEPS TARGET_LISTS)
    file(WRITE ${OUTPUT_PATH} "digraph dependencies {\n")

    list(APPEND TARGETS ${TARGET_LISTS})

    foreach(item IN LISTS ARGN)
        list(APPEND TARGETS ${item})
    endforeach()

    foreach(target IN LISTS TARGETS)
        # 获取目标类型
        get_target_property(target_type ${target} TYPE)

        # 排除一些特殊的内置目标
        if(NOT "${target_type}" STREQUAL "INTERFACE_LIBRARY" AND
            NOT "${target_type}" STREQUAL "UTILITY")
            # 写入目标节点
            file(APPEND ${OUTPUT_PATH} "  \"${target}\" [label=\"${target}\" color=blue];\n")

            # 获取目标依赖
            get_target_property(target_deps ${target} LINK_LIBRARIES)

            foreach(dep IN LISTS target_deps)
                if(TARGET ${dep})
                    # 写入内部依赖关系
                    file(APPEND ${OUTPUT_PATH} "  \"${target}\" -> \"${dep}\";\n")
                elseif(NOT "${dep}" STREQUAL "target_deps-NOTFOUND" AND
                    ${GENERATE_EXTERNAL_LIB_DEPS})
                    # 写入外部有效依赖关系
                    file(APPEND ${OUTPUT_PATH} "  \"${target}\" -> \"${dep}\";\n")
                endif()
            endforeach()
        endif()
    endforeach()

    # 结束dot文件
    file(APPEND ${OUTPUT_PATH} "}\n")
endfunction()

function(export_and_install_target TARGET)
    get_target_property(target_type ${TARGET} TYPE)
    set(IS_EXECUTABLE FALSE)

    if(${target_type} STREQUAL "EXECUTABLE")
        set(IS_EXECUTABLE TRUE)
    endif()

    install(
        TARGETS ${TARGET}
        EXPORT ${TARGET}Targets
        RUNTIME DESTINATION "$<$<CONFIG:Debug>:bind>$<$<NOT:$<CONFIG:Debug>>:bin>"
        LIBRARY DESTINATION "$<$<CONFIG:Debug>:libd>$<$<NOT:$<CONFIG:Debug>>:lib>"
        ARCHIVE DESTINATION "$<$<CONFIG:Debug>:libd>$<$<NOT:$<CONFIG:Debug>>:lib>"
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Also install PDB files for debugging (optional).
    install(FILES $<TARGET_PDB_FILE:${TARGET}>
        DESTINATION "$<$<CONFIG:Debug>:bind>$<$<NOT:$<CONFIG:Debug>>:bin>" OPTIONAL)

    if(IS_EXECUTABLE)
        install(
            FILES $<TARGET_RUNTIME_DLLS:${TARGET}>
            DESTINATION "$<$<CONFIG:Debug>:bind>$<$<NOT:$<CONFIG:Debug>>:bin>"
        )
    endif(IS_EXECUTABLE)

    # Export config.
    install(EXPORT ${TARGET}Targets DESTINATION ${CMAKE_INSTALL_EXPORT_DIR})

    export(TARGETS ${TARGET} FILE ${TARGET}Targets.cmake)
endfunction(export_and_install_target)