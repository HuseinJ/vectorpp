cmake_minimum_required(VERSION 3.14)
project(vectorpp
    VERSION 1.0.0
    DESCRIPTION "Template for C++ library built with CMake"
    LANGUAGES CXX)

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

# VECTORPP_SHARED_LIBS option (undefined by default) can be used to force shared/static build
option(VECTORPP_BUILD_TESTS "Build vectorpp tests" OFF)
option(VECTORPP_BUILD_EXAMPLES "Build vectorpp examples" OFF)
option(VECTORPP_BUILD_DOCS "Build vectorpp documentation" OFF)
option(VECTORPP_INSTALL "Generate target for installing vectorpp" ${is_top_level})
set_if_undefined(VECTORPP_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/vectorpp" CACHE STRING
    "Install path for vectorpp package-related CMake files")

if(DEFINED VECTORPP_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${VECTORPP_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(vectorpp) # initialized below
add_library(vectorpp::vectorpp ALIAS vectorpp)

#----------------------------------------------------------------------------------------------------------------------
# vectorpp dependencies
#----------------------------------------------------------------------------------------------------------------------

# Search for your dependencies here

#----------------------------------------------------------------------------------------------------------------------
# vectorpp sources
#----------------------------------------------------------------------------------------------------------------------

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif()

generate_export_header(vectorpp EXPORT_FILE_NAME include/vectorpp/${export_file_name})

set(sources
    include/vectorpp/export.h
    include/vectorpp/vectorpp.h
    src/vectorpp.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#----------------------------------------------------------------------------------------------------------------------
# vectorpp target
#----------------------------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

target_sources(vectorpp PRIVATE ${sources})
target_compile_definitions(vectorpp PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:VECTORPP_STATIC_DEFINE>")

target_include_directories(vectorpp
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

set_target_properties(vectorpp PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION})

if(VECTORPP_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/vectorpp-config.cmake.in vectorpp-config.cmake
        INSTALL_DESTINATION "${VECTORPP_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(vectorpp-config-version.cmake
        COMPATIBILITY SameMajorVersion)

    install(TARGETS vectorpp EXPORT vectorpp_export
        RUNTIME COMPONENT vectorpp
        LIBRARY COMPONENT vectorpp NAMELINK_COMPONENT vectorpp-dev
        ARCHIVE COMPONENT vectorpp-dev
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
        TYPE INCLUDE
        COMPONENT vectorpp-dev)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/vectorpp/${export_file_name}"
        COMPONENT vectorpp-dev
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/vectorpp")

    set(targets_file "vectorpp-shared-targets.cmake")

    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "vectorpp-static-targets.cmake")
    endif()

    install(EXPORT vectorpp_export
        COMPONENT vectorpp-dev
        FILE "${targets_file}"
        DESTINATION "${VECTORPP_INSTALL_CMAKEDIR}"
        NAMESPACE vectorpp::)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/vectorpp-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/vectorpp-config-version.cmake"
        COMPONENT vectorpp-dev
        DESTINATION "${VECTORPP_INSTALL_CMAKEDIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:vectorpp>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
            set(pdb_file "$<TARGET_FILE_DIR:vectorpp>/$<TARGET_FILE_PREFIX:vectorpp>$<TARGET_FILE_BASE_NAME:vectorpp>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
            COMPONENT vectorpp-dev
            CONFIGURATIONS Debug RelWithDebInfo
            DESTINATION "${pdb_file_destination}"
            OPTIONAL)
    endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# other targets
#----------------------------------------------------------------------------------------------------------------------

if(VECTORPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(VECTORPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(VECTORPP_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif()
