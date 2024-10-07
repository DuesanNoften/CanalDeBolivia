cmake_minimum_required(VERSION 3.10)

# Set the project name
project(SchedulingShips C)

# Specify the C standard to use
set(CMAKE_C_STANDARD 99)

# Add the executable
add_executable(SchedulingShips
    Scheduling/scheduling.c
    main.c
)

# Include directories (for headers)
target_include_directories(SchedulingShips PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Scheduling)


