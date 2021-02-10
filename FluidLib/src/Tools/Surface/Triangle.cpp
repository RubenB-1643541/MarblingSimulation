#include "Triangle.h"

#include "Util/Rendering.h"

namespace FluidLib {

	GLuint Triangle::_buffer = -1;
	GLuint Triangle::_shader = -1;

	Triangle::Triangle()
	{
		_type = "Triangle";
		_centered = true;
		_changed = true;
		if (_buffer == -1) {
			glCreateBuffers(1, &_buffer);
			std::vector<float> points = { 0.0f, 1.0f, 0.5f, 0.0f, 1.0f, 1.0f };
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
		}
		if (_shader == -1) {
			_shader = CompileShader(vertex, fragment);
		}
		_widthold = _width;
		_heightold = _height;
		_rotold = _rotation;
	}

	void Triangle::Draw() const
	{
		glUseProgram(_shader);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


		GLuint xpos = glGetUniformLocation(_shader, "xpos");
		glUniform1f(xpos, _xpos + _trans.GetX());
		GLuint ypos = glGetUniformLocation(_shader, "ypos");
		glUniform1f(ypos, _ypos + _trans.GetY());
		GLuint width = glGetUniformLocation(_shader, "width");
		glUniform1f(width, _width);
		GLuint height = glGetUniformLocation(_shader, "height");
		glUniform1f(height, _height);
		GLuint center = glGetUniformLocation(_shader, "center");
		glUniform1i(center, _centered);
		GLuint color = glGetUniformLocation(_shader, "color");
		glUniform3f(color, _color[0], _color[1], _color[2]);
		GLuint proj = glGetUniformLocation(_shader, "projection");
		glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
		glm::mat4 rotation = glm::rotate(_rotation, glm::vec3(0, 0, 1));
		GLuint rot = glGetUniformLocation(_shader, "rotation");
		glUniformMatrix4fv(rot, 1, GL_FALSE, &rotation[0][0]);
		glm::mat4 translation = glm::translate(glm::vec3(_xpos + _trans.GetX(), _ypos + _trans.GetY(), 0));
		GLuint trans = glGetUniformLocation(_shader, "translation");
		glUniformMatrix4fv(trans, 1, GL_FALSE, &translation[0][0]);
		if (_style == STYLE::FILLED)
			glDrawArrays(GL_POLYGON, 0, 3);
		else if (_style == STYLE::DASHED)
			glDrawArrays(GL_LINES, 0, 3);//Dashed line
		else
			glDrawArrays(GL_LINE_LOOP, 0, 3);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void Triangle::OnScroll(float x, float y)
	{
		_width += y * 2;
		_height += y * 2;
		_changed = true;
	}

	void Triangle::OnMove(float x, float y)
	{
		_xpos = x; _ypos = y;
	}

	float Triangle::GetSurfaceArea() const
	{
		return _width * _height / 2;
	}

	std::vector<IPoint>& Triangle::GetSurfacePoints()
	{
		if (_widthold != _width)
			_changed = true;
		if (_heightold != _height)
			_changed = true;
		if (_rotold != _rotation)
			_changed = true;
		if (_changed) {
			_widthold = _width;
			_heightold = _height;
			_rotold = _rotation;
			_changed = false;
			_points.clear();
			if (_centered) {
				for (int y = -_height/2; y < _height/2; ++y) {
					for(int x = -_width/2 * (y+_height/2)/_height; x < _width/2  * (y + _height / 2)/_height; ++x) {
						_points.push_back(IPoint(x, y).Rotate(_rotation));
					}
				}
			}
			else {
				for (int y = 0; y < abs(_height); ++y) {
					for (int x = _width/2 * (_height - y)/_height; x < _width - _width/2  * (_height - y) / _height; ++x) {
						_points.push_back(IPoint(x, y).Rotate(_rotation));
					}
				}
			}
		}
		return _points;
	}

	void Triangle::StartEdit()
	{
		_pos.OnRelease();
		_wcon.OnRelease();
		_hcon.OnRelease();
		_rotcon.OnRelease();
	}

	void Triangle::EditDraw()
	{
		_pos.SetX(_xpos);
		_pos.SetY(_ypos);
		_wcon.SetX(_width/2);
		_wcon.SetY(_height / 2);
		_wcon.Rotate(_rotation);
		_wcon.VisualTranslate({ _xpos, _ypos  });
		_hcon.SetX(0);
		_hcon.SetY(-_height/2);
		_hcon.Rotate(_rotation);
		_hcon.VisualTranslate({ _xpos, _ypos });
		_rotcon.SetY(_width * sin(_rotation));
		_rotcon.SetX(_width * cos(_rotation));
		_rotcon.VisualTranslate(FPoint(_xpos, _ypos));
		_wcon.Draw();
		_hcon.Draw();
		_pos.Draw();
		_rotcon.Draw();
	}

	bool Triangle::OnEditMove(float x, float y)
	{
		_pos.OnMove(x, y);
		if (_pos.Selected()) {
			_xpos = _pos.GetX();
			_ypos = _pos.GetY();
		}
		_wcon.OnMove(x, y);
		if (_wcon.Selected()) {
			_wcon.Rotate(-_rotation);
			_width = _wcon.GetX()*2;
		}
		_hcon.OnMove(x, y);
		if (_hcon.Selected()) {
			_hcon.Rotate(-_rotation);
			_height = -_hcon.GetY()*2;
		}
		_rotcon.OnMove(x, y);
		if (_rotcon.Selected()) {
			_rotation = atan2(y - _ypos, x - _xpos);
		}
		return false;
	}

	bool Triangle::OnEditClick(float x, float y)
	{
		if (_pos.OnClick(x, y))
			return true;
		if (_wcon.OnClick(x, y))
			return true;
		if (_hcon.OnClick(x, y))
			return true;
		return _rotcon.OnClick(x, y);
	}

	bool Triangle::OnEditRelease(float x, float y)
	{
		_pos.OnRelease();
		_wcon.OnRelease();
		_hcon.OnRelease();
		_rotcon.OnRelease();
		return false;
	}

	void Triangle::SetProjection(glm::mat4 proj)
	{
		_projection = proj;
		_pos.SetProjection(proj);
		_wcon.SetProjection(proj);
		_hcon.SetProjection(proj);
		_rotcon.SetProjection(proj);
	}

}