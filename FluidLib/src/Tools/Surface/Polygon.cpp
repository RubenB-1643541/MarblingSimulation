#include "Polygon.h"

namespace FluidLib {

	GLuint Polygon::_shader = -1;

	Polygon::Polygon()
	{

		_type = "Polygon";
		//_centered = true;
		AddPoint(ControlPoint(0.0f, 0.0f));
		AddPoint(ControlPoint(50.0f, 0.0f));
		AddPoint(ControlPoint(50.0f, 50.0f));
		AddPoint(ControlPoint(0.0f, 50.0f));
		if (_buffer == -1) {
			glCreateBuffers(1, &_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * _controlpoints.size(), &_controlpoints[0], GL_DYNAMIC_DRAW);
		}
		if (_shader == -1) {
			_shader = CompileShader(vertex, fragment);
		}
	}

	void Polygon::Draw() const
	{
		glUseProgram(_shader);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ControlPoint), 0);

		GLuint xpos = glGetUniformLocation(_shader, "xpos");
		glUniform1f(xpos, _pos.GetX() + _trans.GetX());
		GLuint ypos = glGetUniformLocation(_shader, "ypos");
		glUniform1f(ypos, _pos.GetY() + _trans.GetY());
		//GLuint width = glGetUniformLocation(_shader, "width");
		//GLuint center = glGetUniformLocation(_shader, "center");
		//glUniform1i(center, _centered);
		GLuint color = glGetUniformLocation(_shader, "color");
		glUniform3f(color, _color[0], _color[1], _color[2]);
		GLuint proj = glGetUniformLocation(_shader, "projection");
		glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
		//if (_style == STYLE::FILLED)
		//glDrawArrays(GL_POLYGON, 0, PointCount());
		//else if (_style == STYLE::DASHED)
		//	glDrawArrays(GL_LINES, 0, 6);//Dashed line
		//else
		glDrawArrays(GL_LINE_LOOP, 0, PointCount());
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void Polygon::OnScroll(float x, float y)
	{
		float mul;
		if (y > 0.0f)
			mul = 1.1;
		else
			mul = 0.9;
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i] *= mul;
		}
		Refresh();
	}

	void Polygon::OnMove(float x, float y)
	{
		FPoint newpos = { x, y };
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i].VisualTranslate({ x, y });
		}
		_pos = newpos;
		Refresh();
	}

	float Polygon::GetSurfaceArea() const
	{
		return 0.0f;
	}

	std::vector<IPoint>& Polygon::GetSurfacePoints()
	{
		return std::vector<IPoint>();
	}

	void Polygon::StartEdit()
	{
	}

	void Polygon::EditDraw()
	{
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i].Draw();
		}
	}

	bool Polygon::OnEditMove(float x, float y)
	{
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i].OnMove(x, y);
		}
		Refresh();
		return false;
	}

	bool Polygon::OnEditClick(float x, float y)
	{
		for (int i = 0; i < PointCount(); ++i) {
			if (_controlpoints[i].OnClick(x, y))
				return true;
		}
		return false;
	}

	bool Polygon::OnEditRelease(float x, float y)
	{
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i].OnRelease();
		}
		return true;
	}

	void Polygon::AddPoint(const ControlPoint& point)
	{

		_controlpoints.push_back(point);
		_controlpoints[PointCount() - 1].SetProjection(_projection);
		_controlpoints[PointCount() - 1].VisualTranslate(_pos);
		Refresh();
	}

	void Polygon::RemovePoint(int i)
	{
		std::vector<ControlPoint>::iterator it = _controlpoints.begin() + i;
		_controlpoints.erase(it);
		Refresh();
	}

	ControlPoint& Polygon::GetPoint(int i)
	{
		return _controlpoints[i];
	}

	void Polygon::SetProjection(glm::mat4 proj)
	{
		_projection = proj;
		for (int i = 0; i < _controlpoints.size(); ++i)
			_controlpoints[i].SetProjection(proj);
	}

	void Polygon::Refresh()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		//std::cout << sizeof(ControlPoint) << std::endl;
		glBufferData(GL_ARRAY_BUFFER, sizeof(ControlPoint) * _controlpoints.size(), &_controlpoints[0], GL_DYNAMIC_DRAW);
	}



}