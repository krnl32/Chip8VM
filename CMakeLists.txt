cmake_minimum_required(VERSION 3.30)

project(CHIP8VM
	VERSION 1.0.0
	LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(
		$<$<CONFIG:Debug>:-ggdb3>
		$<$<CONFIG:Debug>:-Og>
		$<$<CONFIG:Debug>:-O0>
		$<$<CONFIG:Debug>:-DDEBUG>
		$<$<CONFIG:Debug>:-fno-omit-frame-pointer>
		$<$<CONFIG:Debug>:-Wall>
		$<$<CONFIG:Debug>:-Wextra>
		$<$<CONFIG:Debug>:-Wfloat-equal>
		$<$<CONFIG:Debug>:-Wundef>
		$<$<CONFIG:Debug>:-Wshadow>
		$<$<CONFIG:Debug>:-Wpointer-arith>
		$<$<CONFIG:Debug>:-Wcast-align>
		$<$<CONFIG:Debug>:-Wstrict-prototypes>
		$<$<CONFIG:Debug>:-Wstrict-overflow=5>
		$<$<CONFIG:Debug>:-Wwrite-strings>
		$<$<CONFIG:Debug>:-Wcast-qual>
		$<$<CONFIG:Debug>:-Wswitch-default>
		$<$<CONFIG:Debug>:-Wswitch-enum>
		$<$<CONFIG:Debug>:-Wconversion>
		$<$<CONFIG:Debug>:-Wunreachable-code>

		$<$<CONFIG:Release>:-O3>
		$<$<CONFIG:Release>:-DNDEBUG>
		$<$<CONFIG:Release>:-Wall>
		$<$<CONFIG:Release>:-Wextra>
		$<$<CONFIG:Release>:-s>
	)
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
	add_compile_options(
		$<$<CONFIG:Debug>:/Zi>
		$<$<CONFIG:Debug>:/Od>
		$<$<CONFIG:Debug>:/DDEBUG>
		$<$<CONFIG:Debug>:/W4>
		$<$<CONFIG:Debug>:/Oy->
		$<$<CONFIG:Debug>:/EHsc>

		$<$<CONFIG:Release>:/O2>
		$<$<CONFIG:Release>:/DNDEBUG>
		$<$<CONFIG:Release>:/GL>
		$<$<CONFIG:Release>:/W3>
		$<$<CONFIG:Release>:/Gy>
		$<$<CONFIG:Release>:/EHsc>
	)
else()
	message(WARNING "Unknown compiler: ${CMAKE_C_COMPILER_ID}. No compiler flags set.")
endif()

set(CHIP8VM_TARGET_NAME chip8vm)

# Extensions
include(FetchContent)

FetchContent_Declare(
        sdl2
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
        GIT_TAG "release-2.32.10"
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(sdl2)

add_subdirectory(${CHIP8VM_TARGET_NAME})
