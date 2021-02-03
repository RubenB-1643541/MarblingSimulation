#include "Sine.h"
#include "Util/Rendering.h"
#include <Simulation.h>

namespace FluidLib {

	GLuint Sine::_buffer = -1;
	GLuint Sine::_shader = -1;

	Sine::Sine()
	{
		_type = "Sine";
		if (_buffer == -1) {
			glCreateBuffers(1, &_buffer);
		
			for (float i = 0; i <= 1; i += _precision) {
				_sinepoints.push_back(i);
				_sinepoints.push_back(sinf(i));
			}
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _sinepoints.size(), &_sinepoints[0], GL_STATIC_DRAW);
		}
		if (_shader == -1) {
			_shader = CompileShader(vertex, fragment);
		}
		_size.VisualTranslate({ _hortrans,  _verttrans});

	}

	bool Sine::OnScroll(float x, float y)
	{
		return false;
	}

	void Sine::DrawPath()
	{
		glUseProgram(_shader);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		GLuint verttrans = glGetUniformLocation(_shader, "verttrans");
		glUniform1f(verttrans, _verttrans);
		GLuint hortrans = glGetUniformLocation(_shader, "hortrans");
		glUniform1f(hortrans, _hortrans);
		GLuint amplitude = glGetUniformLocation(_shader, "amplitude");
		glUniform1f(amplitude, _amplitude);
		GLuint period = glGetUniformLocation(_shader, "period");
		glUniform1f(period, _period);
		GLuint color = glGetUniformLocation(_shader, "color");
		glUniform3f(color, 1.0f, 1.0f, 1.0f);
		GLuint proj = glGetUniformLocation(_shader, "projection");
		glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
		//if (_style == STYLE::FILLED)
		//    glDrawArrays(GL_POLYGON, 0, 6);
		//else if (_style == STYLE::DASHED)
		//    glDrawArrays(GL_LINES, 0, 6);//Dashed line
		//else
		glDrawArrays(GL_LINES, 0, (int)(1 / _precision));
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void Sine::OnEdithDraw()
	{
		_pos.SetX(_hortrans);
		_pos.SetY(_verttrans);
		_size.SetX(_period*2);
		_size.SetY(-_amplitude);
		_pos.Draw();
		_size.Draw();
	}

	bool Sine::OnMoveClick(float x, float y)
	{
		if (_pos.OnClick(x, y))
			return true;
		return _size.OnClick(x, y);

	}

	bool Sine::OnMoveRelease(float x, float y)
	{
		_pos.OnRelease();
		_size.OnRelease();
		return false;
	}

	bool Sine::OnMoveMove(float x, float y)
	{
		_pos.OnMove(x, y);
		if (_pos.Selected()) {
			_hortrans = _pos.GetX();
			_verttrans = _pos.GetY();
			_size.VisualTranslate({ _hortrans, _verttrans });
		}
		_size.OnMove(x, y);
		if (_size.Selected()) {
			_period = _size.GetX()/2;
			_amplitude = -_size.GetY();
		}
		return false;
	}

	FPoint Sine::Get(float x, float y)
	{
		return FPoint(x, sinf((x - _hortrans) /_period)*_amplitude + _verttrans);
	}

	void Sine::SetProjection(glm::mat4 proj)
	{
		_projection = proj;
		_pos.SetProjection(proj);
		_size.SetProjection(proj);
	}

}