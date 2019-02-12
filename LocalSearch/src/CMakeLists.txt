find_package(Threads REQUIRED)
find_package(Boost 1.58.0 COMPONENTS program_options REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(../easylocal-3/include)
include_directories(../include)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(FP FP_Main.cc FP_Basics.cc FP_Data.cc FP_Helpers.cc)

target_link_libraries(FP Boost::program_options Threads::Threads)