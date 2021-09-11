add_library(shader_simple STATIC shader_simple.cpp shader_simple.hpp)

add_executable(1_1 1_1_hello_window.cpp)
add_executable(1_2 1_2_hello_window_clear.cpp)
add_executable(2_1 2_1_hello_triangle.cpp)
add_executable(2_2 2_2_hello_triangle_indexed.cpp)
add_executable(3_1 3_1_shaders_uniform.cpp)
add_executable(3_2 3_2_shaders_interpolation.cpp)

add_executable(3_3 3_3_shaders_class.cpp)
target_link_libraries(3_3 PUBLIC shader_simple)
add_dependencies(3_3 copy_shaders)