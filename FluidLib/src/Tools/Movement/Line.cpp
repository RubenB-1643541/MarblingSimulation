#include "Line.h"
#include "Util/Rendering.h"

namespace FluidLib {

    GLuint Line::_buffer = -1;
    GLuint Line::_shader = -1;

    Line::Line()
    {
        _type = "Line";
        _xpos = 20.0f;
        _ypos = 200.0f;
        _len = 930.0f;
        if (_buffer == -1) {
            glCreateBuffers(1, &_buffer);
            std::vector<float> points = { 0.0, 0.0, 1.0, 0.0 };
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

        GLuint xpos = glGetUniformLocation(_shader, "xpos");
        glUniform1f(xpos, _xpos);
        GLuint ypos = glGetUniformLocation(_shader, "ypos");
        glUniform1f(ypos, _ypos);
        GLuint width = glGetUniformLocation(_shader, "len");
        glUniform1f(width, _len);
        GLuint color = glGetUniformLocation(_shader, "color");
        glUniform3f(color, 1.0f, 1.0f, 1.0f);
        GLuint proj = glGetUniformLocation(_shader, "projection");
        glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
        //if (_style == STYLE::FILLED)
        //    glDrawArrays(GL_POLYGON, 0, 6);
        //else if (_style == STYLE::DASHED)
        //    glDrawArrays(GL_LINES, 0, 6);//Dashed line
        //else
        glDrawArrays(GL_LINES, 0, 2);
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

    FPoint Line::Get(float x, float y)
    {
        if(x < _xpos)
            return { _xpos, _ypos };
        else if (x > _xpos + _len)
            return { _xpos + _len, _ypos };
        else
            return { x, _ypos };
    }

}