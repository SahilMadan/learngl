# Third Party Libraries
find_package(assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_path(SYSTEM_INCLUDE_DIRS glad/glad.h)
include_directories(${SYSTEM_INCLUDE_DIRS})

set(CORELIBS glad::glad glfw)
set(COMMON_LIBS shader_m camera)
set(COMMON_LIBS_V2 shader_m camera model mesh)
set(DEPS copy_assets copy_shaders)

# Libraries
add_library(shader_simple STATIC shader_simple.cpp shader_simple.hpp)
add_library(shader_m STATIC shader_m.cpp shader_m.hpp)
add_library(camera STATIC camera.cpp camera.hpp)
add_library(model STATIC model.cpp model.hpp)
add_library(mesh STATIC mesh.cpp mesh.hpp)

add_executable(1_1 1_1_hello_window.cpp)
target_link_libraries(1_1 PRIVATE ${CORELIBS})

add_executable(1_2 1_2_hello_window_clear.cpp)
target_link_libraries(1_2 PRIVATE ${CORELIBS})

add_executable(2_1 2_1_hello_triangle.cpp)
target_link_libraries(2_1 PRIVATE ${CORELIBS})

add_executable(2_2 2_2_hello_triangle_indexed.cpp)
target_link_libraries(2_2 PRIVATE ${CORELIBS})

add_executable(3_1 3_1_shaders_uniform.cpp)
target_link_libraries(3_1 PRIVATE ${CORELIBS})

add_executable(3_2 3_2_shaders_interpolation.cpp)
target_link_libraries(3_2 PRIVATE ${CORELIBS})

add_executable(3_3 3_3_shaders_class.cpp)
target_link_libraries(3_3 PRIVATE ${CORELIBS})
target_link_libraries(3_3 PUBLIC shader_simple)
add_dependencies(3_3 copy_shaders)

add_executable(4_1 4_1_textures.cpp)
target_link_libraries(4_1 PRIVATE ${CORELIBS})
target_link_libraries(4_1 PUBLIC shader_simple)
add_dependencies(4_1 copy_assets)
add_dependencies(4_1 copy_shaders)

add_executable(4_2 4_2_textures_combined.cpp)
target_link_libraries(4_2 PRIVATE ${CORELIBS})
target_link_libraries(4_2 PUBLIC shader_simple)
add_dependencies(4_1 copy_assets)
add_dependencies(4_2 copy_shaders)

add_executable(5_1 5_1_transformations.cpp)
target_link_libraries(5_1 PRIVATE ${CORELIBS})
target_link_libraries(5_1 PUBLIC shader_m)
add_dependencies(5_1 copy_assets)
add_dependencies(5_1 copy_shaders)

add_executable(6_1 6_1_coordinate_systems.cpp)
target_link_libraries(6_1 PRIVATE ${CORELIBS})
target_link_libraries(6_1 PUBLIC shader_m)
add_dependencies(6_1 copy_assets)
add_dependencies(6_1 copy_shaders)

add_executable(6_2 6_2_coordinate_systems_depth.cpp)
target_link_libraries(6_2 PRIVATE ${CORELIBS})
target_link_libraries(6_2 PUBLIC shader_m)
add_dependencies(6_2 copy_assets)
add_dependencies(6_2 copy_shaders)

add_executable(6_3 6_3_coordinate_systems_multiple.cpp)
target_link_libraries(6_3 PRIVATE ${CORELIBS})
target_link_libraries(6_3 PUBLIC shader_m)
add_dependencies(6_3 copy_assets)
add_dependencies(6_3 copy_shaders)

add_executable(7_1 7_1_camera_circle.cpp)
target_link_libraries(7_1 PRIVATE ${CORELIBS})
target_link_libraries(7_1 PUBLIC shader_m)
add_dependencies(7_1 ${DEPS})

add_executable(7_2 7_2_camera_keyboard.cpp)
target_link_libraries(7_2 PRIVATE ${CORELIBS})
target_link_libraries(7_2 PUBLIC shader_m)
add_dependencies(7_2 ${DEPS})

add_executable(7_3 7_3_camera_mouse_zoom.cpp)
target_link_libraries(7_3 PRIVATE ${CORELIBS})
target_link_libraries(7_3 PUBLIC shader_m)
add_dependencies(7_3 ${DEPS})

add_executable(7_4 7_4_camera_class.cpp)
target_link_libraries(7_4 PRIVATE ${CORELIBS})
target_link_libraries(7_4 PUBLIC ${COMMON_LIBS})
add_dependencies(7_4 ${DEPS})

add_executable(8_1 8_1_colors.cpp)
target_link_libraries(8_1 PRIVATE ${CORELIBS})
target_link_libraries(8_1 PUBLIC ${COMMON_LIBS})
add_dependencies(8_1 ${DEPS})

add_executable(9_1 9_1_basic_lighting_diffuse.cpp)
target_link_libraries(9_1 PRIVATE ${CORELIBS})
target_link_libraries(9_1 PUBLIC ${COMMON_LIBS})
add_dependencies(9_1 ${DEPS})

add_executable(9_2 9_2_basic_lighting_specular.cpp)
target_link_libraries(9_2 PRIVATE ${CORELIBS})
target_link_libraries(9_2 PUBLIC ${COMMON_LIBS})
add_dependencies(9_2 ${DEPS})

add_executable(10_1 10_1_material.cpp)
target_link_libraries(10_1 PRIVATE ${CORELIBS})
target_link_libraries(10_1 PUBLIC ${COMMON_LIBS})
add_dependencies(10_1 ${DEPS})

add_executable(11_1 11_1_diffuse_map.cpp)
target_link_libraries(11_1 PRIVATE ${CORELIBS})
target_link_libraries(11_1 PUBLIC ${COMMON_LIBS})
add_dependencies(11_1 ${DEPS})

add_executable(11_2 11_2_specular_map.cpp)
target_link_libraries(11_2 PRIVATE ${CORELIBS})
target_link_libraries(11_2 PUBLIC ${COMMON_LIBS})
add_dependencies(11_2 ${DEPS})

add_executable(12_1 12_1_light_casters_directional.cpp)
target_link_libraries(12_1 PRIVATE ${CORELIBS})
target_link_libraries(12_1 PUBLIC ${COMMON_LIBS})
add_dependencies(12_1 ${DEPS})

add_executable(12_2 12_2_light_casters_point.cpp)
target_link_libraries(12_2 PRIVATE ${CORELIBS})
target_link_libraries(12_2 PUBLIC ${COMMON_LIBS})
add_dependencies(12_2 ${DEPS})

add_executable(12_3 12_3_light_casters_spot.cpp)
target_link_libraries(12_3 PRIVATE ${CORELIBS})
target_link_libraries(12_3 PUBLIC ${COMMON_LIBS})
add_dependencies(12_3 ${DEPS})

add_executable(12_4 12_4_light_casters_spot_soft.cpp)
target_link_libraries(12_4 PRIVATE ${CORELIBS})
target_link_libraries(12_4 PUBLIC ${COMMON_LIBS})
add_dependencies(12_4 ${DEPS})

add_executable(13_1 13_1_multiple_lights.cpp)
target_link_libraries(13_1 PRIVATE ${CORELIBS})
target_link_libraries(13_1 PUBLIC ${COMMON_LIBS})
add_dependencies(13_1 ${DEPS})

add_executable(14_1 14_1_model_loading.cpp)
target_link_libraries(14_1 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(14_1 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(14_1 ${DEPS})

add_executable(14_2 14_2_model_with_lighting.cpp)
target_link_libraries(14_2 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(14_2 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(14_2 ${DEPS})

add_executable(15_1 15_1_depth_testing.cpp)
target_link_libraries(15_1 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(15_1 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(15_1 ${DEPS})

add_executable(15_2 15_2_visualize_depth_buffer.cpp)
target_link_libraries(15_2 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(15_2 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(15_2 ${DEPS})

add_executable(15_3 15_3_visualize_linear_depth.cpp)
target_link_libraries(15_3 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(15_3 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(15_3 ${DEPS})

add_executable(16_1 16_1_stencil_testing.cpp)
target_link_libraries(16_1 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(16_1 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(16_1 ${DEPS})

add_executable(17_1 17_1_blending_discard.cpp)
target_link_libraries(17_1 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(17_1 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(17_1 ${DEPS})

add_executable(17_2 17_2_blending_sorted.cpp)
target_link_libraries(17_2 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(17_2 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(17_2 ${DEPS})

add_executable(18_1 18_1_face_culling.cpp)
target_link_libraries(18_1 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(18_1 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(18_1 ${DEPS})

add_executable(19_1 19_1_framebuffers_inversion.cpp)
target_link_libraries(19_1 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(19_1 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(19_1 ${DEPS})

add_executable(19_2 19_2_framebuffers_grayscale.cpp)
target_link_libraries(19_2 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(19_2 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(19_2 ${DEPS})

add_executable(19_3 19_3_framebuffers_kernel_sharpen.cpp)
target_link_libraries(19_3 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(19_3 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(19_3 ${DEPS})

add_executable(19_4 19_4_framebuffers_kernel_blur.cpp)
target_link_libraries(19_4 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(19_4 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(19_4 ${DEPS})

add_executable(19_5 19_5_framebuffers_kernel_edge_detection.cpp)
target_link_libraries(19_5 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(19_5 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(19_5 ${DEPS})

add_executable(20_1 20_1_cubemaps_skybox.cpp)
target_link_libraries(20_1 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(20_1 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(20_1 ${DEPS})

add_executable(20_2 20_2_cubemaps_skybox_optimized.cpp)
target_link_libraries(20_2 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(20_2 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(20_2 ${DEPS})

add_executable(20_3 20_3_cubemaps_environment_mapping_reflection.cpp)
target_link_libraries(20_3 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(20_3 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(20_3 ${DEPS})

add_executable(20_4 20_4_cubemaps_environment_mapping_refraction.cpp)
target_link_libraries(20_4 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(20_4 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(20_4 ${DEPS})

add_executable(21_1 21_1_point_size.cpp)
target_link_libraries(21_1 PRIVATE ${CORELIBS} assimp::assimp)
target_link_libraries(21_1 PUBLIC ${COMMON_LIBS_V2})
add_dependencies(21_1 ${DEPS})