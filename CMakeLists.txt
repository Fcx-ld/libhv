cmake_minimum_required(VERSION 3.6)

project(hv VERSION 1.3.2)

option(BUILD_SHARED "build shared library" ON)
option(BUILD_STATIC "build static library" ON)

option(BUILD_EXAMPLES "build examples" ON)
option(BUILD_UNITTEST "build unittest" OFF)

# see config.ini
option(WITH_PROTOCOL "compile protocol" OFF)

option(WITH_EVPP "compile evpp" ON)
option(WITH_HTTP "compile http" ON)
option(WITH_HTTP_SERVER "compile http/server" ON)
option(WITH_HTTP_CLIENT "compile http/client" ON)
option(WITH_MQTT "compile mqtt" OFF)

option(ENABLE_UDS "Unix Domain Socket" OFF)
option(USE_MULTIMAP "MultiMap" OFF)

option(WITH_CURL "with curl library (deprecated)" OFF)
option(WITH_NGHTTP2 "with nghttp2 library" OFF)

option(WITH_OPENSSL "with openssl library" OFF)
option(WITH_GNUTLS  "with gnutls library"  OFF)
option(WITH_MBEDTLS "with mbedtls library" OFF)

option(WITH_KCP "compile event/kcp" OFF)

option(STUDY "study project" ON)

if(WIN32 OR MINGW)
    option(WITH_WEPOLL "compile event/wepoll -> use iocp" ON)
    option(ENABLE_WINDUMP "Windows MiniDumpWriteDump" OFF)
    option(BUILD_FOR_MT "build for /MT" OFF)
    if(BUILD_FOR_MT)
        #[[在CMake脚本中，这行命令的作用是为调试配置（Debug）设置C编译器的标志。具体来说：

CMAKE_C_FLAGS_DEBUG 是一个CMake预定义的变量，它包含了用于调试构建类型的C编译器的命令行选项。

"/MTd" 是一个编译器标志，专门用于Microsoft Visual C++编译器，它指示编译器生成调试信息，并使用静态链接的CRT（C Runtime）库。/MTd标志与多线程调试版本的CRT库链接，而/MDd则与动态链接的CRT库链接。

${CMAKE_C_FLAGS_DEBUG} 表示当前为调试配置设置的C编译器标志的值。

set(...) 命令用于设置变量的值。这里它将现有的 CMAKE_C_FLAGS_DEBUG 值与 /MTd 标志连接起来，用空格分隔。

此命令通常用于Windows平台，当你希望在Debug模式下使用静态链接的CRT库时。使用静态链接的CRT库可以避免DLL地狱（依赖于特定版本的DLL），并且有时可以提供更清晰的错误信息，因为所有必要的CRT代码都包含在最终的可执行文件中。

请注意，如果你的项目中同时使用了 /MT 和 /MD 标志（分别用于Release和Debug配置），你需要确保所有项目文件都使用相同的CRT库类型，以避免运行时错误]]
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    endif()
endif()
# message(STATUS ...) 命令输出当前的CMake变量值，有助于调试和确认配置状态
message(STATUS "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
# STREQUAL 是一个内置的条件运算符，用于比较两个字符串是否完全相同。
if(NOT "${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    set(BUILD_EXAMPLES OFF)
endif()

if(IOS)
    set(BUILD_SHARED OFF)
    set(BUILD_EXAMPLES OFF)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(utils)
include(vars)

# see configure
# Checks for header files
check_header("stdbool.h")
check_header("stdint.h")
check_header("stdatomic.h")
check_header("sys/types.h")
check_header("sys/stat.h")
check_header("sys/time.h")
check_header("fcntl.h")
check_header("pthread.h")
check_header("endian.h")
check_header("sys/endian.h")

# Checks for functions
if(NOT MSVC)
    set(CMAKE_REQUIRED_LIBRARIES "-pthread")
endif()
check_function("gettid" "unistd.h")
check_function("strlcpy" "string.h")
check_function("strlcat" "string.h")
check_function("clock_gettime" "time.h")
check_function("gettimeofday" "sys/time.h")
check_function("pthread_spin_lock" "pthread.h")
check_function("pthread_mutex_timedlock" "pthread.h")
check_function("sem_timedwait" "semaphore.h")
check_function("pipe" "unistd.h")
check_function("socketpair" "sys/socket.h")
check_function("eventfd" "sys/eventfd.h")
check_function("setproctitle" "unistd.h")
# configure_file 是 CMake 中的一个命令，用于将一个源文件复制到另一个位置，并在这个过程中执行变量替换。
# CMake 会查找 hconfig.h.in 文件中的特殊标记（例如 @VAR@），这些标记对应于 CMake 变量 VAR 的值。CMake 将自动替换这些标记为它们的实际值。
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/hconfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/hconfig.h)

# see Makefile.in
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 这行代码会告诉系统 INCDIR 变量应该指向 include 目录
set(INCDIR include)
set(SRCDIR src)
set(LIBDIR lib)
set(BINDIR bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BINDIR})
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
#[[include_directories 来添加头文件的搜索路径。指定编译器在编译过程中搜索头文件的路径。
使用 link_directories 来添加库文件的搜索路径。链接器在链接过程中搜索库文件的路径。]]
set(INCDIRS . include 3rd/include)
set(LIBDIRS . lib 3rd/lib)
include_directories(${INCDIRS} ${SRCDIR})
link_directories(${LIBDIRS})
# 当你使用 add_definitions(-DDEBUG) 时，你正在向编译器添加一个定义 -DDEBUG。这通常意味着你想要启用调试模式。
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DDEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

if(ENABLE_UDS)
    add_definitions(-DENABLE_UDS)
endif()

if(USE_MULTIMAP)
    add_definitions(-DUSE_MULTIMAP)
endif()

if(WITH_CURL)
    add_definitions(-DWITH_CURL)
    set(LIBS ${LIBS} curl)
    if(WIN32 OR MINGW)
        set(LIBS ${LIBS} wldap32 advapi32 crypt32)
    endif()
endif()

if(WITH_NGHTTP2)
    add_definitions(-DWITH_NGHTTP2)
    set(LIBS ${LIBS} nghttp2)
endif()

if(WITH_OPENSSL)
    add_definitions(-DWITH_OPENSSL)
    find_package(OpenSSL)
    if(OpenSSL_FOUND)
        set(LIBS ${LIBS} OpenSSL::SSL OpenSSL::Crypto)
    else()
        set(LIBS ${LIBS} ssl crypto)
    endif()
endif()

if(WITH_GNUTLS)
    add_definitions(-DWITH_GNUTLS)
    set(LIBS ${LIBS} gnutls)
endif()

if(WITH_MBEDTLS)
    add_definitions(-DWITH_MBEDTLS)
    set(LIBS ${LIBS} mbedtls mbedx509 mbedcrypto)
endif()

if(WIN32 OR MINGW)
    add_definitions(-DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0600)
    set(LIBS ${LIBS} secur32 crypt32 winmm iphlpapi ws2_32)
    if(ENABLE_WINDUMP)
        add_definitions(-DENABLE_WINDUMP)
        set(LIBS ${LIBS} dbghelp)
    endif()
endif()

if(ANDROID)
    set(LIBS ${LIBS} log)
elseif(UNIX AND NOT MINGW)
    set(LIBS ${LIBS} pthread m dl)
    if(CMAKE_COMPILER_IS_GNUCC)
        set(LIBS ${LIBS} rt)
    endif()
endif()

if(APPLE)
    set(LIBS ${LIBS} "-framework CoreFoundation" "-framework Security")
endif()

# see Makefile
set(ALL_SRCDIRS . base ssl event event/kcp util cpputil evpp protocol http http/client http/server mqtt)
set(CORE_SRCDIRS . base ssl event)
if(WIN32 OR MINGW)
    if(WITH_WEPOLL)
        set(CORE_SRCDIRS ${CORE_SRCDIRS} event/wepoll)
    endif()
endif()
if(WITH_KCP)
    set(CORE_SRCDIRS ${CORE_SRCDIRS} event/kcp)
endif()
set(LIBHV_SRCDIRS ${CORE_SRCDIRS} util)
set(LIBHV_HEADERS hv.h hconfig.h hexport.h)
set(LIBHV_HEADERS ${LIBHV_HEADERS} ${BASE_HEADERS} ${SSL_HEADERS} ${EVENT_HEADERS} ${UTIL_HEADERS})

if(WITH_PROTOCOL)
    set(LIBHV_HEADERS ${LIBHV_HEADERS} ${PROTOCOL_HEADERS})
    set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} protocol)
endif()

if(WITH_EVPP)
    set(LIBHV_HEADERS ${LIBHV_HEADERS} ${CPPUTIL_HEADERS} ${EVPP_HEADERS})
    set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} cpputil evpp)
    if(WITH_HTTP)
        set(LIBHV_HEADERS ${LIBHV_HEADERS} ${HTTP_HEADERS})
        set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} http)
        if(WITH_NGHTTP2)
            set(LIBHV_HEADERS ${LIBHV_HEADERS} ${HTTP2_HEADERS})
        endif()
        if(WITH_HTTP_SERVER)
            set(LIBHV_HEADERS ${LIBHV_HEADERS} ${HTTP_SERVER_HEADERS})
            set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} http/server)
        endif()
        if(WITH_HTTP_CLIENT)
            set(LIBHV_HEADERS ${LIBHV_HEADERS} ${HTTP_CLIENT_HEADERS})
            set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} http/client)
        endif()
    endif()

    if(CMAKE_SYSTEM_NAME MATCHES "Linux" AND CMAKE_COMPILER_IS_GNUCC)
        set(LIBS ${LIBS} stdc++)
    endif()
endif()

if(WITH_MQTT)
    set(LIBHV_HEADERS ${LIBHV_HEADERS} ${MQTT_HEADERS})
    set(LIBHV_SRCDIRS ${LIBHV_SRCDIRS} mqtt)
endif()

list_source_directories(LIBHV_SRCS ${LIBHV_SRCDIRS})

file(INSTALL ${LIBHV_HEADERS} DESTINATION include/hv)
file(INSTALL ${LIBHV_HEADERS} DESTINATION ${PROJECT_SOURCE_DIR}/include/hv)

if(BUILD_SHARED)
    add_library(hv SHARED ${LIBHV_SRCS})
    target_compile_definitions(hv PRIVATE HV_DYNAMICLIB)
    target_include_directories(hv PRIVATE ${LIBHV_SRCDIRS}
        INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
    target_link_libraries(hv ${LIBS})
    install(TARGETS hv
        EXPORT libhvConfig
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
    add_custom_target(libhv DEPENDS hv)
endif()

if(BUILD_STATIC)
    add_library(hv_static STATIC ${LIBHV_SRCS})
    target_compile_definitions(hv_static PUBLIC HV_STATICLIB)
    target_include_directories(hv_static PRIVATE ${LIBHV_SRCDIRS}
        INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
    target_link_libraries(hv_static ${LIBS})
    install(TARGETS hv_static
        EXPORT libhvConfig
        ARCHIVE DESTINATION lib)
    add_custom_target(libhv_static DEPENDS hv_static)
endif()

install(FILES ${LIBHV_HEADERS} DESTINATION include/hv)
install(EXPORT libhvConfig DESTINATION lib/cmake/libhv)
#[[定义变量: 定义了一个名为 HV_LIBRARIES 的变量，其值为 hv。这里 hv 可能代表一个库的名称，例如一个动态链接库（DLL）或静态库（LIB），具体取决于项目环境和库的实现。

缓存变量: 使用 CACHE 关键字将 HV_LIBRARIES 变量设置为一个缓存变量。缓存变量的值在CMake配置过程中被保存，并且在后续的配置中保持不变，除非明确地改变它。这有助于加快构建过程，因为CMake不需要每次都重新计算这些值。

内部文档: INTERNAL 表示这个变量是供项目内部使用的，不会暴露给外部用户。这通常用于避免用户在配置项目时不小心修改这些变量。

文档字符串: 通过双引号提供的字符串 "link hv libraries" 为变量提供了一个文档字符串，这有助于在CMake GUI或通过 cmake -LH 命令查看时提供额外的上下文信息。]]
if(BUILD_SHARED)
    set(HV_LIBRARIES hv CACHE INTERNAL "link hv libraries")
else()
    add_definitions(-DHV_STATICLIB)
    set(HV_LIBRARIES hv_static ${LIBS} CACHE INTERNAL "link hv libraries")
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
    # for httpd -c etc/httpd.conf
    file(INSTALL etc DESTINATION ${CMAKE_BINARY_DIR})
    file(INSTALL etc DESTINATION ${CMAKE_BINARY_DIR}/bin)
    file(INSTALL etc DESTINATION ${CMAKE_BINARY_DIR}/examples)
endif()

if(STUDY)
 add_subdirectory(study)
endif()

if(BUILD_UNITTEST)
    add_subdirectory(unittest)
endif()

# CPack settings
set(CPACK_PACKAGE_NAME "libhv")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A high-performance C/C++ network library")
set(CPACK_PACKAGE_VENDOR "libhv")
set(CPACK_PACKAGE_CONTACT "ithewei <ithewei@163.com>")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_HOST_SYSTEM_PROCESSOR}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Specify the package generators
set(CPACK_GENERATOR "TGZ;DEB;RPM")

# Enable CPack debug output
set(CPACK_PACKAGE_DEBUG True)

# https://cmake.org/cmake/help/latest/variable/CPACK_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION.html
set(CPACK_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION "ON")
include(CPack)
