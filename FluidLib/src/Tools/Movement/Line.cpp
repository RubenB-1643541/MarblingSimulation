#include "Line.h"
#include "Util/Rendering.h"

namespace FluidLib {

    GLuint Line::_buffer = -1;
    GLuint Line::_shader = -1;

    Line::Line()
    {
        _type = "Line";
        _p1 = { 10.0f, 200.0f };
        _p2 = { 800.0f, 200.0f };
        if (_buffer == -1) {
            glCreateBuffers(1, &_buffer);
            std::vector<float> points = { 0.0, 0.0, 1.0, 1.0 };
            glBindBuffer(GL_ARRAY_BUFFER, _buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
        }
        if (_shader == -1) {
            _shader = CompileShader(vertex, fragment);
        }
    }

    bool Line::OnScroll(float x, float y)
	{
        //_ypos += y;
		return false;
	}

	void Line::DrawPath()
	{
        glUseProgram(_shader);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _buffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        GLuint p1l = glGetUniformLocation(_shader, "p1");
        glUniform2f(p1l, _p1.GetX(), _p1.GetY());
        GLuint p2l = glGetUniformLocation(_shader, "p2");
        glUniform2f(p2l, _p2.GetX(), _p2.GetY());
        GLuint color = glGetUniformLocation(_shader, "color");
        glUniform3f(color, 1.0f, 1.0f, 1.0f);
        GLuint proj = glGetUniformLocation(_shader, "projection");
        glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
        glDrawArrays(GL_LINES, 0, 2);
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

    void Line::StartEdit()
    {
        _p1.OnRelease();
        _p2.OnRelease();
    }

    void Line::OnEditDraw()
    {
        _p1.Draw();
        _p2.Draw();
    }

    bool Line::OnEditClick(float x, float y)
    {
        if (_p1.OnClick(x, y))
            return true;
        if (_p2.OnClick(x, y))
            return true;
        return false;
    }

    bool Line::OnEditRelease(float x, float y)
    {
        _p1.OnRelease();
        _p2.OnRelease();

        return false;
    }

    bool Line::OnEditMove(float x, float y)
    {
        _p1.OnMove(x, y);
        _p2.OnMove(x, y);
        return false;
    }

    FPoint Line::Get(float x, float y)
    {        
        if (x <= _p1.GetX()) {
            x = _p1.GetX();
        }
        else if (x >= _p2.GetX()) {
            x = _p2.GetX();
        }
        float oldx = x;
        x -= _p1.GetX();
        y = _p1.GetY() +  x * (_p2.GetY() - _p1.GetY()) / (_p2.GetX() - _p1.GetX());
        return {oldx, y};
    }

    void Line::SetProjection(glm::mat4 proj)
    {
        _projection = proj;
        _p1.SetProjection(proj);
        _p2.SetProjection(proj);
    }

}