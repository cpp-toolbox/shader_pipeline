#ifndef SHADER_PIPELINE_HPP
#define SHADER_PIPELINE_HPP

#include "glad/gl.h"

/*
 * A class that can construct an opengl shader program given
 * file paths as inputs. This acts as a wrapper to operate
 * with shader programs easily.
 *
 * Note that the shader programs id is a public attribute 
 * which you can use to set uniforms.
 *
 */
class ShaderPipeline{
	public:
		GLuint shader_program_id;
		void load_in_shaders_from_file(const char *vertex_shader_path, const char* fragment_shader_path);
	private:
		void create_and_link_and_use_shader_program(GLuint vertex_shader_id, GLuint fragment_shader_id);
		GLuint create_and_compile_shader_object(const char * shader_source_code, GLenum shader_type);

};
#endif
