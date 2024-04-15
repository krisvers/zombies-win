# CMakeList.txt : CMake project for zombies, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.9)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("zombies")

include_directories(include)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

include_directories("include")
link_libraries("d3d10" "dxgi" "dinput8" "dsound" "dxguid" ${JNI_LIBRARIES})
include_directories(zombies PUBLIC ${JNI_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable(zombies "src/main.cpp"  "include/global.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET zombies PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
