cmake_minimum_required(VERSION 3.10)
project(luamapd VERSION "0.0.1" DESCRIPTION "A wrapper for the arbitrary memory mapper as a unix daemon." LANGUAGES CXX C)

add_compile_definitions(LINUX_BUILD)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-ggdb -Wall -O0 -static -static-libgcc -static-libstdc++")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-DNDEBUG -Wall -O3 -static -static-libgcc -static-libstdc++")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/..)
include_directories(${CMAKE_SOURCE_DIR}/../lua-5.4.4/src)
include_directories(${CMAKE_SOURCE_DIR}/../AMM)
include_directories(${CMAKE_SOURCE_DIR}/../AMM/math)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_sources( ${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lapi.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lauxlib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lbaselib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lcode.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lcorolib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lctype.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/ldblib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/ldebug.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/ldo.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/ldump.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lfunc.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lgc.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/linit.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/liolib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/llex.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lmathlib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lmem.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/loadlib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lobject.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lopcodes.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/loslib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lparser.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lstate.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lstring.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lstrlib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/ltable.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/ltablib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/ltm.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lundump.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lutf8lib.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lvm.c
	${CMAKE_SOURCE_DIR}/../lua-5.4.4/src/lzio.c
)
target_sources( ${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/../AMM/InterprocessMemorySpace.cpp
	${CMAKE_SOURCE_DIR}/../AMM/LoggedPage.cpp
	${CMAKE_SOURCE_DIR}/../AMM/LuaPage.cpp
	${CMAKE_SOURCE_DIR}/../AMM/WindowsPageAccessor.cpp
	${CMAKE_SOURCE_DIR}/../AMM/addressSpace.cpp
	${CMAKE_SOURCE_DIR}/../AMM/callbackOnWritePage.cpp
	${CMAKE_SOURCE_DIR}/../AMM/filePage.cpp
	${CMAKE_SOURCE_DIR}/../AMM/math/hsv2rgb.cpp
	${CMAKE_SOURCE_DIR}/../AMM/multiPage.cpp
	${CMAKE_SOURCE_DIR}/../AMM/pageVisualizer.cpp
	${CMAKE_SOURCE_DIR}/../AMM/selfRefrence.cpp
	${CMAKE_SOURCE_DIR}/../AMM/virtualMemorySpace.cpp
	${CMAKE_SOURCE_DIR}/../AMM/virtualPage.cpp
	${CMAKE_SOURCE_DIR}/../ArbitraryMemMap.cpp
	${CMAKE_SOURCE_DIR}/../LuaMemoryMapperLib.cpp
)
add_executable(ammsvc ammsvc.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11) # update your C++ version here if you like

# Configure .service file
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.service)
configure_file(${CMAKE_SOURCE_DIR}/systemd/daemonpp.service.in ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.service)
endif()

# Configure .conf file
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.conf)
configure_file(${CMAKE_SOURCE_DIR}/systemd/daemonpp.conf.in ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.conf)
endif()

# Rename default daemonpp.cpp to ${PROJECT_NAME}.cpp
if(EXISTS ${CMAKE_SOURCE_DIR}/daemonpp.cpp)
    file(RENAME ${CMAKE_SOURCE_DIR}/daemonpp.cpp ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.cpp)
endif()

# Install the config file .conf
install(FILES ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.conf DESTINATION /etc/${PROJECT_NAME}/)

# Install the systemd file .service
install(FILES ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.service DESTINATION /etc/systemd/system/)

# Install the binary program
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin/)
install(TARGETS ammsvc DESTINATION /usr/bin/)

# make uninstall
add_custom_target("uninstall" COMMENT "Uninstall daemon files")
add_custom_command(
        TARGET "uninstall"
        POST_BUILD
        COMMENT "Uninstall files with install_manifest.txt"
        COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in install_manifest.txt to be uninstalled!
)
