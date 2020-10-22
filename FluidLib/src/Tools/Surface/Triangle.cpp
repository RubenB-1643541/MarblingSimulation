#include "Triangle.h"

#include "Util/Rendering.h"

namespace FluidLib {

	GLuint Triangle::_buffer = -1;
	GLuint Triangle::_shader = -1;

	Triangle::Triangle()
	{
		_type = "Triangle";
		_centered = false;
		if (_buffer == -1) {
			glCreateBuffers(1, &_buffer);
			std::vector<float> points = { 0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f };
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
		}
		if (_shader == -1) {
			_shader = CompileShader(vertex, fragment);
		}
	}

	void Triangle::Draw() const
	{
		glUseProgram(_shader);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


		GLuint xpos = glGetUniformLocation(_shader, "xpos");
		glUniform1f(xpos, _xpos);
		GLuint ypos = glGetUniformLocation(_shader, "ypos");
		glUniform1f(ypos, _ypos);
		GLuint width = glGetUniformLocation(_shader, "width");
		glUniform1f(width, _len);
		GLuint height = glGetUniformLocation(_shader, "height");
		glUniform1f(height, _len);
		GLuint center = glGetUniformLocation(_shader, "center");
		glUniform1i(center, _centered);
		GLuint color = glGetUniformLocation(_shader, "color");
		glUniform3f(color, _color[0], _color[1], _color[2]);
		GLuint proj = glGetUniformLocation(_shader, "projection");
		glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
		if (_style == STYLE::FILLED)
			glDrawArrays(GL_POLYGON, 0, 6);
		else if (_style == STYLE::DASHED)
			glDrawArrays(GL_LINES, 0, 6);//Dashed line
		else
			glDrawArrays(GL_LINE_LOOP, 0, 6);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void Triangle::OnScroll(float x, float y)
	{
		_len += y * 2;
		if (_len < 1)
			_len = 1.0f;
		_changed = true;
	}

	void Triangle::OnMove(float x, float y)
	{
		_xpos = x; _ypos = y;
	}

	float Triangle::GetSurfaceArea() const
	{
		return _len * _len / 2;
	}

	std::vector<IPoint>& Triangle::GetSurfacePoints()
	{
		if (_changed) {
			_points.clear();//TODO
			if (_centered) {
				for (int y = -abs(_len / 2); y < abs(_len / 2); ++y) {
					for (int x = -(abs(_len) - y); x < (abs(_len) - y/2)/2; ++x) {
						_points.push_back(IPoint(x, y));
					}
				}
			}
			else {
				for (int y = 0; y < abs(_len); ++y) {
					for (int x = y/2; x < abs(_len) - y/2; ++x) {
						_points.push_back(IPoint(x, y));
					}
				}
			}
		}
		return _points;
	}

}