#include "PointSurface.h"
#include "Util/Rendering.h"
namespace FluidLib {

	GLuint PointSurface::_buffer = -1;
	GLuint PointSurface::_shader = -1;


	PointSurface::PointSurface()
	{
		if (_buffer == -1) {
			glCreateBuffers(1, &_buffer);
			std::vector<float> points;

			points.push_back(1.0f);
			points.push_back(1.0f);
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
		}
		if (_shader == -1) {
			_shader = CompileShader(vertex, fragment);
		}
		_points = { { 1,1 } };
	}

	void PointSurface::Draw() const
	{
		glUseProgram(_shader);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);

		GLuint x = glGetUniformLocation(_shader, "xpos");
		glUniform1f(x, _xpos + _trans.GetX());
		GLuint y = glGetUniformLocation(_shader, "ypos");
		glUniform1f(y, _ypos + _trans.GetY());
		GLuint size = glGetUniformLocation(_shader, "size");
		glUniform1f(size, 1.0f);
		GLuint color = glGetUniformLocation(_shader, "color");
		glUniform3f(color, 1.0f, 1.0f, 1.0f);
		GLuint proj = glGetUniformLocation(_shader, "projection");
		glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);

		glDrawArrays(GL_POINTS, 0, 1);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void PointSurface::OnScroll(float x, float y)
	{
	}

	void PointSurface::OnMove(float x, float y)
	{
		_xpos = x; _ypos = y;
	}

	float PointSurface::GetSurfaceArea() const
	{
		return 1.0f;
	}

	std::vector<IPoint>& PointSurface::GetSurfacePoints()
	{
		return _points;
	}

}