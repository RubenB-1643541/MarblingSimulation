#include "ControlPoint.h"

namespace FluidLib {

	GLuint ControlPoint::_buffer = -1;
	GLuint ControlPoint::_shader = -1;

	ControlPoint::ControlPoint() : FPoint()
	{
		Init();
	}

	ControlPoint::ControlPoint(float x, float y) : FPoint(x, y)
	{
		Init();
	}

	ControlPoint::ControlPoint(const FPoint& p) : FPoint(p)
	{
		Init();
	}

	void ControlPoint::Draw()
	{
		glUseProgram(_shader);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);

		GLuint x = glGetUniformLocation(_shader, "xpos");
		glUniform1f(x, GetX() + _trans.GetX());
		GLuint y = glGetUniformLocation(_shader, "ypos");
		glUniform1f(y, GetY() + _trans.GetY());
		GLuint size = glGetUniformLocation(_shader, "size");
		glUniform1f(size, _size);
		GLuint color = glGetUniformLocation(_shader, "color");
		if(_selected)
			glUniform3f(color, 1.0f, 0.0f, 0.0f);
		else
			glUniform3f(color, 1.0f, 1.0f, 1.0f);
		GLuint proj = glGetUniformLocation(_shader, "projection");
		glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);

		glDrawArrays(GL_POINTS, 0, 1);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	bool ControlPoint::OnClick(float x, float y)
	{
		if (floor(x) >= floor(GetX() + _trans.GetX() - _size * 2) && floor(x) <= floor(GetX() + _trans.GetX() + _size * 2) && floor(y) >= floor(GetY() + _trans.GetY() - _size * 2) && floor(y) <= floor(GetY() + _trans.GetY() + _size * 2)) {
			_selected = true;
			return true;
		}
		return false;
	}

	void ControlPoint::OnRelease()
	{
		_selected = false;
	}

	void ControlPoint::OnMove(float x, float y)
	{
		if (_selected) {
			SetX(x - _trans.GetX()); SetY(y - _trans.GetY());
		}
	}

	void ControlPoint::VisualTranslate(const FPoint& p)
	{
		_trans = p;
	}

	void ControlPoint::Init()
	{
		glEnable(GL_PROGRAM_POINT_SIZE);
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
	}

}