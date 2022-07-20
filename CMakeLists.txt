# cmake最低版本号要求
cmake_minimum_required(VERSION 3.14)

# 项目名称
project(xt_stl)



# 版本号 主版本号.子版本号.修复版本号
set(XT_STL_VERSION_MAJOR 1)
set(XT_STL_VERSION_MINOR 0)
set(XT_STL_VERSION_PATCH 0)
set(XT_STL_VERSION "${XT_STL_VERSION_MAJOR}.${XT_STL_VERSION_MINOR}.${XT_STL_VERSION_PATCH}")
message(STATUS "The version of this project is: ${XT_STL_VERSION}")

# build type
# 不可调试 体积小 有优化 
# set(CMAKE_BUILD_TYPE release)
# 可调试 体积大 
set(CMAKE_BUILD_TYPE Debug)

# 设置编译器以及编译器的编译选项
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Wno-sign-compare -Wno-unused-but-set-variable -Wno-array-bounds")
	# set(EXTRA_CXX_FLAGS -Weffc++ -Wswitch-default -Wfloat-equal -Wconversion -Wsign-conversion)
	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0.0")
		message(FATAL_ERROR "required GCC 5.0 or later")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Wno-sign-compare")
	# set(EXTRA_CXX_FLAGS -Weffc++ -Wswitch-default -Wfloat-equal -Wconversion -Wimplicit-fallthrough)
	if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.5.0")
		message(FATAL_ERROR "required Clang 3.5 or later")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	endif()
endif()
message(STATUS "The cmake_cxx_flags is: ${CMAKE_CXX_FLAGS}")

# 添加 Test 子目录
add_subdirectory(${PROJECT_SOURCE_DIR}/Test)




