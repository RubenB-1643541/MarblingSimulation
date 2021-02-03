#include "CircleMovement.h"

namespace FluidLib {

	GLuint CircleMovement::_buffer = -1;
	GLuint CircleMovement::_shader = -1;

	CircleMovement::CircleMovement()
	{
		_type = "CircleMovement";
		if (_buffer == -1) {
			glCreateBuffers(1, &_buffer);
			std::vector<float> points;
			for (float i = 0; i <= 1; i += _precision) {
				float theta = 2.0 * M_PI * i;
				points.push_back(cosf(theta));
				points.push_back(sinf(theta));
			}
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
		}
		if (_shader == -1) {
			_shader = CompileShader(vertex, fragment);
		}
	}

	bool CircleMovement::OnScroll(float x, float y)
	{
		return false;
	}

	void CircleMovement::DrawPath()
	{
		glUseProgram(_shader);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


		GLuint xpos = glGetUniformLocation(_shader, "xpos");
		glUniform1f(xpos, _x);
		GLuint ypos = glGetUniformLocation(_shader, "ypos");
		glUniform1f(ypos, _y);
		GLuint radius = glGetUniformLocation(_shader, "radius");
		glUniform1f(radius, _r);
		GLuint color = glGetUniformLocation(_shader, "color");
		glUniform3f(color, _color[0], _color[1], _color[2]);
		GLuint proj = glGetUniformLocation(_shader, "projection");
		glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
		glDrawArrays(GL_LINES, 0, (int)(1 / _precision));//Dashed line
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void CircleMovement::StartEdit()
	{
		_pos.OnRelease();
		_size.OnRelease();
	}

	void CircleMovement::OnEditDraw()
	{
		_pos.SetX(_x);
		_pos.SetY(_y);
		_size.SetX(_r);
		_size.SetY(0);
		_size.VisualTranslate({ _x, _y });
		_pos.Draw();
		_size.Draw();
	}

	bool CircleMovement::OnEditClick(float x, float y)
	{
		if (_pos.OnClick(x, y))
			return true;
		return _size.OnClick(x, y);
	}

	bool CircleMovement::OnEditRelease(float x, float y)
	{
		_pos.OnRelease();
		_size.OnRelease();
		return false;
	}

	bool CircleMovement::OnEditMove(float x, float y)
	{
		_pos.OnMove(x, y);
		if (_pos.Selected()) {
			_x = _pos.GetX();
			_y = _pos.GetY();
		}
		_size.OnMove(x, y);
		if (_size.Selected()) {
			_r = _size.GetX();
		}
		return false;
	}

	FPoint CircleMovement::Get(float x, float y)
	{
		
		double theta = atan2(x - _x, y - _y);
		float newx = _x + _r * sin(theta);
		float newy = _y + _r * cos(theta);
		return FPoint(newx, newy);
	}

	void CircleMovement::SetProjection(glm::mat4 proj)
	{
		_projection = proj;
		_pos.SetProjection(proj);
		_size.SetProjection(proj);
	}

}