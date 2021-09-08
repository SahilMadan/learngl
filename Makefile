SHELL := /bin/bash

CC=g++
SRCDIR=./src
BUILDIR=./build
OBJDIR=./obj
FLAGS=-Wall -Werror -Wpedantic -Wextra -lglad -lglfw -ldl
CAMERA=${OBJDIR}/camera.o
SHADER_S=${OBJDIR}/shader_simple.o
SHADER_M=${OBJDIR}/shader_m.o
MESH=${OBJDIR}/mesh.o
MODEL=${OBJDIR}/model.o
# STB=-lstb
ASSIMP=-lassimp

setup:
	@echo "Setting up..."
	mkdir -p ${BUILDIR}
	mkdir -p ${OBJDIR}
	@echo "Ready to build individual targets. For example, \"make 1_1\""

1_1: ${SRCDIR}/1_1_hello_window.cpp 
	${CC} ${SRCDIR}/1_1_hello_window.cpp \
		${FLAGS} -o ${BUILDIR}/1_1 &&\
		${BUILDIR}/1_1

1_2: ${SRCDIR}/1_2_hello_window_clear.cpp 
	${CC} ${SRCDIR}/1_2_hello_window_clear.cpp \
		${FLAGS} -o ${BUILDIR}/1_2 &&\
		${BUILDIR}/1_2

2_1: ${SRCDIR}/2_1_hello_triangle.cpp 
	${CC} ${SRCDIR}/2_1_hello_triangle.cpp \
		${FLAGS} -o ${BUILDIR}/2_1 &&\
		${BUILDIR}/2_1

2_2: ${SRCDIR}/2_2_hello_triangle_indexed.cpp 
	${CC} ${SRCDIR}/2_2_hello_triangle_indexed.cpp \
		${FLAGS} -o ${BUILDIR}/2_2 &&\
		${BUILDIR}/2_2

3_1: ${SRCDIR}/3_1_shaders_uniform.cpp 
	${CC} ${SRCDIR}/3_1_shaders_uniform.cpp \
		${FLAGS} -o ${BUILDIR}/3_1 &&\
		${BUILDIR}/3_1

3_2: ${SRCDIR}/3_2_shaders_interpolation.cpp 
	${CC} ${SRCDIR}/3_2_shaders_interpolation.cpp \
		${FLAGS} -o ${BUILDIR}/3_2 &&\
		${BUILDIR}/3_2

3_3: ${SRCDIR}/3_3_shaders_class.cpp shader_simple
	${CC} ${SRCDIR}/3_3_shaders_class.cpp ${SHADER_S} \
		${FLAGS} -o ${BUILDIR}/3_3 && \
		${BUILDIR}/3_3

4_1: ${SRCDIR}/4_1_textures.cpp shader_simple
	${CC} ${SRCDIR}/4_1_textures.cpp ${SHADER_S} \
		${FLAGS} ${STB} -o ${BUILDIR}/4_1 && \
		${BUILDIR}/4_1

4_2: ${SRCDIR}/4_2_textures_combined.cpp shader_simple
	${CC} ${SRCDIR}/4_2_textures_combined.cpp ${SHADER_S} \
		${FLAGS} ${STB} -o ${BUILDIR}/4_2 && \
		${BUILDIR}/4_2

5_1: ${SRCDIR}/5_1_transformations.cpp shader_m
	${CC} ${SRCDIR}/5_1_transformations.cpp ${SHADER_M} \
		${FLAGS} ${STB} -o ${BUILDIR}/5_1 && \
		${BUILDIR}/5_1

6_1: ${SRCDIR}/6_1_coordinate_systems.cpp shader_m
	${CC} ${SRCDIR}/6_1_coordinate_systems.cpp ${SHADER_M} \
		${FLAGS} ${STB} -o ${BUILDIR}/6_1 && \
		${BUILDIR}/6_1

6_2: ${SRCDIR}/6_2_coordinate_systems_depth.cpp shader_m
	${CC} ${SRCDIR}/6_2_coordinate_systems_depth.cpp ${SHADER_M} \
		${FLAGS} ${STB} -o ${BUILDIR}/6_2 && \
		${BUILDIR}/6_2

6_3: ${SRCDIR}/6_3_coordinate_systems_multiple.cpp shader_m
	${CC} ${SRCDIR}/6_3_coordinate_systems_multiple.cpp ${SHADER_M} \
		${FLAGS} ${STB} -o ${BUILDIR}/6_3 && \
		${BUILDIR}/6_3

7_1: ${SRCDIR}/7_1_camera_circle.cpp shader_m
	${CC} ${SRCDIR}/7_1_camera_circle.cpp ${SHADER_M} \
		${FLAGS} ${STB} -o ${BUILDIR}/7_1 && \
		${BUILDIR}/7_1

7_2: ${SRCDIR}/7_2_camera_keyboard.cpp shader_m
	${CC} ${SRCDIR}/7_2_camera_keyboard.cpp ${SHADER_M} \
		${FLAGS} ${STB} -o ${BUILDIR}/7_2 && \
		${BUILDIR}/7_2

7_3: ${SRCDIR}/7_3_camera_mouse_zoom.cpp shader_m
	${CC} ${SRCDIR}/7_3_camera_mouse_zoom.cpp ${SHADER_M} \
		${FLAGS} ${STB} -o ${BUILDIR}/7_3 && \
		${BUILDIR}/7_3

7_4: ${SRCDIR}/7_4_camera_class.cpp shader_m camera
	${CC} ${SRCDIR}/7_4_camera_class.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/7_4 && \
		${BUILDIR}/7_4

8_1: ${SRCDIR}/8_1_colors.cpp shader_m camera
	${CC} ${SRCDIR}/8_1_colors.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/8_1 && \
		${BUILDIR}/8_1

9_1: ${SRCDIR}/9_1_basic_lighting_diffuse.cpp shader_m camera
	${CC} ${SRCDIR}/9_1_basic_lighting_diffuse.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/9_1 && \
		${BUILDIR}/9_1

9_2: ${SRCDIR}/9_2_basic_lighting_specular.cpp shader_m camera
	${CC} ${SRCDIR}/9_2_basic_lighting_specular.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/9_2 && \
		${BUILDIR}/9_2

10_1: ${SRCDIR}/10_1_material.cpp shader_m camera
	${CC} ${SRCDIR}/10_1_material.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/10_1 && \
		${BUILDIR}/10_1

11_1: ${SRCDIR}/11_1_diffuse_map.cpp shader_m camera
	${CC} ${SRCDIR}/11_1_diffuse_map.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/11_1 && \
		${BUILDIR}/11_1

11_2: ${SRCDIR}/11_2_specular_map.cpp shader_m camera
	${CC} ${SRCDIR}/11_2_specular_map.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/11_2 && \
		${BUILDIR}/11_2

12_1: ${SRCDIR}/12_1_light_casters_directional.cpp shader_m camera
	${CC} ${SRCDIR}/12_1_light_casters_directional.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/12_1 && \
		${BUILDIR}/12_1

12_2: ${SRCDIR}/12_2_light_casters_point.cpp shader_m camera
	${CC} ${SRCDIR}/12_2_light_casters_point.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/12_2 && \
		${BUILDIR}/12_2

12_3: ${SRCDIR}/12_3_light_casters_spot.cpp shader_m camera
	${CC} ${SRCDIR}/12_3_light_casters_spot.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/12_3 && \
		${BUILDIR}/12_3

12_4: ${SRCDIR}/12_4_light_casters_spot_soft.cpp shader_m camera
	${CC} ${SRCDIR}/12_4_light_casters_spot_soft.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/12_4 && \
		${BUILDIR}/12_4

13_1: ${SRCDIR}/13_1_multiple_lights.cpp shader_m camera
	${CC} ${SRCDIR}/13_1_multiple_lights.cpp ${SHADER_M} ${CAMERA} \
		${FLAGS} ${STB} -o ${BUILDIR}/13_1 && \
		${BUILDIR}/13_1

14_1: ${SRCDIR}/14_1_model_loading.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/14_1_model_loading.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/14_1 && \
		${BUILDIR}/14_1

14_2: ${SRCDIR}/14_2_model_with_lighting.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/14_2_model_with_lighting.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/14_2 && \
		${BUILDIR}/14_2

15_1: ${SRCDIR}/15_1_depth_testing.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/15_1_depth_testing.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/15_1 && \
		${BUILDIR}/15_1

15_2: ${SRCDIR}/15_2_visualize_depth_buffer.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/15_2_visualize_depth_buffer.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/15_2 && \
		${BUILDIR}/15_2

15_3: ${SRCDIR}/15_3_visualize_linear_depth.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/15_3_visualize_linear_depth.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/15_3 && \
		${BUILDIR}/15_3

16_1: ${SRCDIR}/16_1_stencil_testing.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/16_1_stencil_testing.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/16_1 && \
		${BUILDIR}/16_1

17_1: ${SRCDIR}/17_1_blending_discard.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/17_1_blending_discard.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/17_1 && \
		${BUILDIR}/17_1

17_2: ${SRCDIR}/17_2_blending_sorted.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/17_2_blending_sorted.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/17_2 && \
		${BUILDIR}/17_2

18_1: ${SRCDIR}/18_1_face_culling.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/18_1_face_culling.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/18_1 && \
		${BUILDIR}/18_1

19_1: ${SRCDIR}/19_1_framebuffers_inversion.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/19_1_framebuffers_inversion.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/19_1 && \
		${BUILDIR}/19_1

19_2: ${SRCDIR}/19_2_framebuffers_grayscale.cpp shader_m camera mesh model
	${CC} ${SRCDIR}/19_2_framebuffers_grayscale.cpp ${SHADER_M} ${CAMERA} ${MESH} ${MODEL} \
		${FLAGS} ${STB} ${ASSIMP} -o ${BUILDIR}/19_2 && \
		${BUILDIR}/19_2

camera: ${SRCDIR}/camera.cpp
	${CC} ${SRCDIR}/camera.cpp \
		${FLAGS} -c -o ${CAMERA} 
	
shader_simple: ${SRCDIR}/shader_simple.cpp
	${CC} ${SRCDIR}/shader_simple.cpp \
		${FLAGS} -c -o ${SHADER_S} 

shader_m: ${SRCDIR}/shader_m.cpp
	${CC} ${SRCDIR}/shader_m.cpp \
		${FLAGS} -c -o ${SHADER_M} 

mesh: ${SRCDIR}/mesh.cpp
	${CC} ${SRCDIR}/mesh.cpp \
		${FLAGS} -c -o ${MESH}

model: ${SRCDIR}/model.cpp
	${CC} ${SRCDIR}/model.cpp \
		${FLAGS} -c -o ${MODEL}

clean:
	rm -rf ${BUILDIR}
	rm -rf ${OBJDIR}	
