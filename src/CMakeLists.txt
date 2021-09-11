add_library(shader_simple STATIC shader_simple.cpp shader_simple.hpp)
add_library(shader_m STATIC shader_m.cpp shader_m.hpp)
add_library(camera STATIC camera.cpp camera.hpp)

set(COMMON_LIBS shader_m camera)
set(DEPS copy_assets copy_shaders)

add_executable(1_1 1_1_hello_window.cpp)
add_executable(1_2 1_2_hello_window_clear.cpp)
add_executable(2_1 2_1_hello_triangle.cpp)
add_executable(2_2 2_2_hello_triangle_indexed.cpp)
add_executable(3_1 3_1_shaders_uniform.cpp)
add_executable(3_2 3_2_shaders_interpolation.cpp)

add_executable(3_3 3_3_shaders_class.cpp)
target_link_libraries(3_3 PUBLIC shader_simple)
add_dependencies(3_3 copy_shaders)

add_executable(4_1 4_1_textures.cpp)
target_link_libraries(4_1 PUBLIC shader_simple)
add_dependencies(4_1 copy_assets)
add_dependencies(4_1 copy_shaders)

add_executable(4_2 4_2_textures_combined.cpp)
target_link_libraries(4_2 PUBLIC shader_simple)
add_dependencies(4_1 copy_assets)
add_dependencies(4_2 copy_shaders)

add_executable(5_1 5_1_transformations.cpp)
target_link_libraries(5_1 PUBLIC shader_m)
add_dependencies(5_1 copy_assets)
add_dependencies(5_1 copy_shaders)

add_executable(6_1 6_1_coordinate_systems.cpp)
target_link_libraries(6_1 PUBLIC shader_m)
add_dependencies(6_1 copy_assets)
add_dependencies(6_1 copy_shaders)

add_executable(6_2 6_2_coordinate_systems_depth.cpp)
target_link_libraries(6_2 PUBLIC shader_m)
add_dependencies(6_2 copy_assets)
add_dependencies(6_2 copy_shaders)

add_executable(6_3 6_3_coordinate_systems_multiple.cpp)
target_link_libraries(6_3 PUBLIC shader_m)
add_dependencies(6_3 copy_assets)
add_dependencies(6_3 copy_shaders)

add_executable(7_1 7_1_camera_circle.cpp)
target_link_libraries(7_1 PUBLIC shader_m)
add_dependencies(7_1 copy_assets)
add_dependencies(7_1 copy_shaders)

add_executable(7_2 7_2_camera_keyboard.cpp)
target_link_libraries(7_2 PUBLIC shader_m)
add_dependencies(7_2 copy_assets)
add_dependencies(7_2 copy_shaders)

add_executable(7_3 7_3_camera_mouse_zoom.cpp)
target_link_libraries(7_3 PUBLIC shader_m)
add_dependencies(7_3 copy_assets)
add_dependencies(7_3 copy_shaders)

add_executable(7_4 7_4_camera_class.cpp)
target_link_libraries(7_4 PUBLIC ${COMMON_LIBS})
add_dependencies(7_4 ${DEPS})

add_executable(8_1 8_1_colors.cpp)
target_link_libraries(8_1 PUBLIC ${COMMON_LIBS})
add_dependencies(8_1 ${DEPS})

add_executable(9_1 9_1_basic_lighting_diffuse.cpp)
target_link_libraries(9_1 PUBLIC ${COMMON_LIBS})
add_dependencies(9_1 ${DEPS})

add_executable(9_2 9_2_basic_lighting_specular.cpp)
target_link_libraries(9_2 PUBLIC ${COMMON_LIBS})
add_dependencies(9_2 ${DEPS})

add_executable(10_1 10_1_material.cpp)
target_link_libraries(10_1 PUBLIC ${COMMON_LIBS})
add_dependencies(10_1 ${DEPS})