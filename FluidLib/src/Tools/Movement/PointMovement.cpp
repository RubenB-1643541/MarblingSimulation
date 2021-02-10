#include "PointMovement.h"

namespace FluidLib {

	GLuint PointMovement::_buffer = -1;
	GLuint PointMovement::_shader = -1;

	PointMovement::PointMovement()
	{
		_type = "Point";
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
		_pos = { { 250, 250 } };
	}

	bool PointMovement::OnScroll(float x, float y)
	{
		return false;
	}

	void PointMovement::DrawPath()
	{
		glUseProgram(_shader);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);

		GLuint x = glGetUniformLocation(_shader, "xpos");
		glUniform1f(x, _pos.GetX());
		GLuint y = glGetUniformLocation(_shader, "ypos");
		glUniform1f(y, _pos.GetY());
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

	void PointMovement::StartEdit()
	{
		_pos.OnRelease();
	}

	void PointMovement::OnEditDraw()
	{
		_pos.Draw();
	}

	bool PointMovement::OnEditClick(float x, float y)
	{
		return _pos.OnClick(x,y);
	}

	bool PointMovement::OnEditRelease(float x, float y)
	{
		_pos.OnRelease();
		return false;
	}

	bool PointMovement::OnEditMove(float x, float y)
	{
		_pos.OnMove(x, y);
		return false;
	}

	FPoint PointMovement::Get(float x, float y)
	{
		return _pos;
	}

	void PointMovement::SetProjection(glm::mat4 proj)
	{
		_projection = proj;
		_pos.SetProjection(proj);
	}

}