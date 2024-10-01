cmake_minimum_required(VERSION 3.10)
project(CanalDeBolivia)

set(CMAKE_C_STANDARD 99)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/Canal/Factories)

# Add the path to Collections-C headers
include_directories(/usr/local/include)

# Find and add the path to Collections-C library
find_library(COLLECTIONS_C_LIB collections-c /usr/local/lib)

# Specify source files
set(SOURCE_FILES
        main.c
        CEthreads/CEmutex_destroy.c
        CEthreads/CEmutex_init.c
        CEthreads/CEmutex_unlock.c
        CEthreads/CEthread_create.c
        CEthreads/CEthread_end.c
        CEthreads/CEthread_join.c
        # Add additional source files here if necessary
)

# Add the executable target
add_executable(CanalDeBolivia ${SOURCE_FILES})

# Link Collections-C to your target
target_link_libraries(CanalDeBolivia ${COLLECTIONS_C_LIB})