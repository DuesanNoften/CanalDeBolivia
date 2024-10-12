cmake_minimum_required(VERSION 3.10)

# Set the project name
project(SchedulingShips C)

# Specify the C standard to use
set(CMAKE_C_STANDARD 99)

# Add the executable
add_executable(SchedulingShips
    CEthreads/CEmutex_destroy.c
    CEthreads/CEmutex_init.c
    CEthreads/CEmutex_unlock.c
    CEthreads/CEmutex_lock.c
    CEthreads/CEthread_create.c
    CEthreads/CEthread_end.c
    CEthreads/CEthread_join.c
    Scheduling/scheduling.c
    canal.c
    main.c
)

# Include directories (for headers)
target_include_directories(SchedulingShips PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#Find the SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Link the SDL2 library to the project
target_link_libraries(SchedulingShips ${SDL2_LIBRARIES})


