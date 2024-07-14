#include "shader_pipeline.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

/**
 * precondition to all methods:
 *  opengl has been initialized, otherwise you may get segfaults
 */

/**
 * description
 *  given the shader's source code and type, register the shader with opengl
 *  and return the id that opengl has given the shader
 */
GLuint ShaderPipeline::create_and_compile_shader_object(const char *shader_source_code, GLenum shader_type) {
    GLuint shader_object;
    shader_object = glCreateShader(shader_type);
    glShaderSource(shader_object, 1, &shader_source_code, NULL);
    glCompileShader(shader_object);

    int success;
    char info_log[512];
    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader_object, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    return shader_object;
}

/**
 * description:
 *  creates a shader program given a vertex and fragment shader, also uses the program
 */
void ShaderPipeline::create_and_link_and_use_shader_program(GLuint vertex_shader_id, GLuint fragment_shader_id) {
    this->shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, fragment_shader_id);
    glLinkProgram(shader_program_id);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shader_program_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // this queues up the shaders for deletion
    // see: https://docs.gl/gl3/glDeleteShader for more details
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    glUseProgram(shader_program_id);
}

/**
 * description:
 *  given a valid path with respect to the build folder for a vertex and fragment shader
 *  it loads in the shaders and creates a shader program
 */
void ShaderPipeline::load_in_shaders_from_file(const char *vertex_shader_path, const char *fragment_shader_path) {

    std::cout << "loading: " << vertex_shader_path << ", " << fragment_shader_path << std::endl;
    // load in shaders from file
    std::string vertex_shader_source_code, fragment_shader_source_code;
    std::ifstream vertex_shader_file_stream, fragment_shader_file_stream;
    vertex_shader_file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertex_shader_file_stream.open(vertex_shader_path);
        fragment_shader_file_stream.open(fragment_shader_path);
        std::stringstream vertex_shader_string_stream, fragment_shader_string_stream;

        vertex_shader_string_stream << vertex_shader_file_stream.rdbuf();
        fragment_shader_string_stream << fragment_shader_file_stream.rdbuf();

        vertex_shader_file_stream.close();
        fragment_shader_file_stream.close();

        vertex_shader_source_code = vertex_shader_string_stream.str();
        fragment_shader_source_code = fragment_shader_string_stream.str();
        std::cout << "Successfully loaded in the vertex shader: \n" << vertex_shader_source_code;
        std::cout << "Successfully loaded in the fragment shader: \n" << fragment_shader_source_code;

    } catch (std::ifstream::failure error) {
        // TODO use logging here
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: EITHER FRAG OR VERT SHADER HAS THE ERROR "
                  << strerror(errno) << std::endl;
    }

    // compile the shaders
    const char *vertex_shader_source_code_c_str = vertex_shader_source_code.c_str();
    const char *fragment_shader_source_code_c_str = fragment_shader_source_code.c_str();

    GLuint vertex_shader_id = this->create_and_compile_shader_object(vertex_shader_source_code_c_str, GL_VERTEX_SHADER);
    GLuint fragment_shader_id =
        this->create_and_compile_shader_object(fragment_shader_source_code_c_str, GL_FRAGMENT_SHADER);

    create_and_link_and_use_shader_program(vertex_shader_id, fragment_shader_id);
};
