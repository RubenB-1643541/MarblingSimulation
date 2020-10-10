#include "GridRenderer.h"

namespace FluidLib {

	GridRenderer::GridRenderer() : _projection(1.0f)
	{
	}

	GridRenderer::GridRenderer(GridManager* grids) : GridRenderer()
	{
		_grids = grids;
	}

	void GridRenderer::Draw()
	{
		//glBegin(GL_TRIANGLES);
		//glVertex3f(-1, -1, 0);
		//glVertex3f(0, 1, 0);
		//glVertex3f(1, -1, 0);
		//glEnd();
		//Set ViewPort (optional)
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		_shader.Use();
		_shader.BindUniforms();
		GLuint proj = glGetUniformLocation(_shader.GetId(), "projection");
		glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
		_grids->RenderGrids();
		
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glDrawArrays(GL_POINTS, 0, _size);
		glDisableClientState(GL_VERTEX_ARRAY);
		//Reset ViewPort
	}

	void GridRenderer::SetSimSize(float width, float height)
	{
		_width = width; _height = height;
		_size = width * height;
		_projection = glm::ortho(0.0f, _width, _height, 0.0f);
	}

}