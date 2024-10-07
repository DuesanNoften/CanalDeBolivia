cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Proyecto_Canal_de_Bolivia)

# Add the executable
add_executable(Proyecto_Canal_de_Bolivia
        CEmutex_destroy.c
        CEmutex_init.c
        CEmutex_unlock.c
        CEmutex_lock.c
        CEthread_create.c
        CEthread_end.c
        CEthread_join.c
        Equidad.c
        Letrero.c
        Tico.c
        main.c
)

# Include directories
target_include_directories(Proyecto_Canal_de_Bolivia PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries if needed (add any libraries your project depends on)
# target_link_libraries(Proyecto_Canal_de_Bolivia <library_name>)
