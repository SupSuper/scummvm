/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BACKENDS_GRAPHICS_OPENGL_PIPELINES_SHADER_H
#define BACKENDS_GRAPHICS_OPENGL_PIPELINES_SHADER_H

#include "backends/graphics/opengl/pipelines/pipeline.h"

namespace OpenGL {

#if !USE_FORCED_GLES
class Shader;

class ShaderPipeline : public Pipeline {
public:
	ShaderPipeline(Shader *shader);
	~ShaderPipeline() override;

	void setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) override;

	void setProjectionMatrix(const Math::Matrix4 &projectionMatrix) override;

protected:
	void activateInternal() override;
	void deactivateInternal() override;
	void drawTextureInternal(const Texture &texture, const GLfloat *coordinates, const GLfloat *texcoords) override;

	GLuint _coordsVBO;
	GLuint _texcoordsVBO;
	GLuint _colorVBO;

	GLfloat _colorAttributes[4*4];
	bool _colorDirty;

	Shader *const _activeShader;
};
#endif // !USE_FORCED_GLES

} // End of namespace OpenGL

#endif
