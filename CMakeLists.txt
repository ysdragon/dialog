cmake_minimum_required(VERSION 3.16)
project(RingDialog LANGUAGES C)

include(FetchContent)
find_package(Git QUIET)

# Get the Ring source directory from environment or use default
if(NOT DEFINED ENV{RING})
	set(RING "${CMAKE_CURRENT_SOURCE_DIR}/../..")
	message(STATUS "RING not defined, using default: ${RING}")
else()
	set(RING $ENV{RING})
	message(STATUS "Using RING from environment: ${RING}")
endif()

# Set paths
set(RING_INCLUDE "${RING}/language/include")
set(RING_LIB "${RING}/lib")
set(RING_BIN "${RING}/bin")

# Add osdialog, fetching from GitHub if not present
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/osdialog/osdialog.h")
    message(STATUS "osdialog source not found, fetching from GitHub...")
    FetchContent_Declare(
        osdialog
        GIT_REPOSITORY https://github.com/AndrewBelt/osdialog.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(osdialog)
    set(OSDIALOG_SRC_DIR ${osdialog_SOURCE_DIR})
    set(OSDIALOG_INCLUDE_DIR ${osdialog_SOURCE_DIR})
else()
    message(STATUS "osdialog source found locally.")
    set(OSDIALOG_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/osdialog")
    set(OSDIALOG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/osdialog")
endif()

# Apply patch for MSVC if needed
if(MSVC AND GIT_FOUND)
    message(STATUS "MSVC detected, applying patch to osdialog.")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} apply --ignore-space-change --ignore-whitespace ${CMAKE_CURRENT_SOURCE_DIR}/src/patches/fix_msvc.patch
        WORKING_DIRECTORY ${OSDIALOG_SRC_DIR}
        RESULT_VARIABLE PATCH_RESULT
    )
    if(PATCH_RESULT)
        message(WARNING "osdialog patch could not be applied: ${PATCH_RESULT}")
    endif()
endif()

set(RING_DIALOG_C_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ring_dialog.c"
    "${OSDIALOG_SRC_DIR}/osdialog.c"
)

# Add OS-specific osdialog source files
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND RING_DIALOG_C_SOURCES "${OSDIALOG_SRC_DIR}/osdialog_mac.m")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND RING_DIALOG_C_SOURCES "${OSDIALOG_SRC_DIR}/osdialog_win.c")
endif()

# Determine OS and Architecture specific paths
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(OS_DIR "linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    set(OS_DIR "freebsd")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OS_DIR "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(OS_DIR "macos")
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

# Determine architecture
if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND DEFINED CMAKE_GENERATOR_PLATFORM)
    if(CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
        set(ARCH_DIR "i386")
    elseif(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
        set(ARCH_DIR "amd64")
    elseif(CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64")
        set(ARCH_DIR "arm64")
    else()
        message(WARNING "Unsupported CMAKE_GENERATOR_PLATFORM: ${CMAKE_GENERATOR_PLATFORM}, falling back to CMAKE_SYSTEM_PROCESSOR")
    endif()
endif()

# Fallback to CMAKE_SYSTEM_PROCESSOR if ARCH_DIR is not set
if(NOT DEFINED ARCH_DIR)
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" CMAKE_SYSTEM_PROCESSOR_LC)
    if(CMAKE_SYSTEM_PROCESSOR_LC MATCHES "x86_64|amd64")
        set(ARCH_DIR "amd64")
    elseif(CMAKE_SYSTEM_PROCESSOR_LC MATCHES "aarch64|arm64")
        set(ARCH_DIR "arm64")
    elseif(CMAKE_SYSTEM_PROCESSOR_LC MATCHES "riscv64")
        set(ARCH_DIR "riscv64")
    elseif(CMAKE_SYSTEM_PROCESSOR_LC MATCHES "i386|i686|x86")
        set(ARCH_DIR "i386")
    else()
        message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
endif()

# Set the destination directory for the built library
set(LIB_DEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/${OS_DIR}/${ARCH_DIR}")


# Linux backend configuration
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    find_package(PkgConfig REQUIRED)
    find_program(ZENITY_CMD zenity)

    if(ZENITY_CMD)
        message(STATUS "Found Zenity, using osdialog_zenity.")
        list(APPEND RING_DIALOG_C_SOURCES "${OSDIALOG_SRC_DIR}/osdialog_zenity.c")
    else()
        message(STATUS "Zenity not found, falling back to GTK4.")
        pkg_check_modules(GTK4 QUIET gtk4)
        if (GTK4_FOUND)
            message(STATUS "Found GTK4, using osdialog_gtk.")
            list(APPEND RING_DIALOG_C_SOURCES "${OSDIALOG_SRC_DIR}/osdialog_gtk.c")
            target_compile_definitions(ring_dialog PRIVATE ${GTK4_DEFINITIONS})
            target_include_directories(ring_dialog PRIVATE ${GTK4_INCLUDE_DIRS})
            # We will link this later
            target_compile_definitions(ring_dialog PRIVATE OSDIALOG_GTK)
        else()
            message(FATAL_ERROR "Neither Zenity nor GTK4 found. Please install one of them.")
        endif()
    endif()
endif()

# Create the Ring dialog extension shared library
add_library(ring_dialog SHARED
	${RING_DIALOG_C_SOURCES}
)

# Include directories
target_include_directories(ring_dialog PRIVATE
	${RING_INCLUDE}
	${OSDIALOG_INCLUDE_DIR}
)

# Determine Ring library based on OS
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(RING_LIB_NAME "${RING_LIB}/ringstatic.lib")
else()
	set(RING_LIB_NAME "${RING_LIB}/libring${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

# Link libraries
target_link_libraries(ring_dialog PRIVATE
    ${RING_LIB_NAME}
)

# OS-specific linking
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND GTK4_FOUND)
    target_link_libraries(ring_dialog PRIVATE ${GTK4_LIBRARIES})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(ring_dialog PRIVATE "-framework AppKit")
endif()


# Set target properties for output name and prefix
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set_target_properties(ring_dialog PROPERTIES
		OUTPUT_NAME "ring_dialog"
	)
else()
	set_target_properties(ring_dialog PROPERTIES
		PREFIX "lib"
		OUTPUT_NAME "ring_dialog"
	)
endif()

# Move the built library to the OS/architecture-specific directory after build
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(
        TARGET ring_dialog
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${LIB_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RING_BIN}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ring_dialog> ${RING_BIN}/
        COMMAND ${CMAKE_COMMAND} -E rename $<TARGET_FILE:ring_dialog> ${LIB_DEST_DIR}/$<TARGET_FILE_NAME:ring_dialog>
        COMMENT "Moving library to ${LIB_DEST_DIR} and copying to ${RING_BIN}"
        VERBATIM
    )
else()
    add_custom_command(
        TARGET ring_dialog
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${LIB_DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E rename $<TARGET_FILE:ring_dialog> ${LIB_DEST_DIR}/$<TARGET_FILE_NAME:ring_dialog>
        COMMENT "Moving built library to ${LIB_DEST_DIR}"
        VERBATIM
    )
endif()

# Install rule
get_filename_component(NORMALIZED_RING_LIB "${RING_LIB}" ABSOLUTE)
install(TARGETS ring_dialog
	LIBRARY DESTINATION ${NORMALIZED_RING_LIB}
)

# Print info message
message(STATUS "Ring Dialog Extension Configuration:")
message(STATUS "  - Ring Include Dir: ${RING_INCLUDE}")
message(STATUS "  - Ring Library Dir: ${RING_LIB}")