#include "Square.h"

#include "Util/Rendering.h"

namespace FluidLib {

    GLuint Square::_buffer = -1;
    GLuint Square::_shader = -1;

    Square::Square()
    {
        if (_buffer == -1) {
            glCreateBuffers(1, &_buffer);
            std::vector<float> points = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
            glBindBuffer(GL_ARRAY_BUFFER, _buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
        }
        if (_shader == -1) {
            _shader = CompileShader(vertex, fragment);
        }
    }

    void Square::Draw() const
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

    void Square::OnScroll(float x, float y)
    {
        _len += y*2;
        _changed = true;
    }

    void Square::OnMove(float x, float y)
    {
        _xpos = x; _ypos = y;
    }

    float Square::GetSurfaceArea() const
    {
        return _len * _len;
    }

    std::vector<IPoint>& Square::GetSurfacePoints()
    {
        if (_changed) {
            _points.clear();
            for (int i = 0; i < abs(_len); ++i) {
                for (int j = 0; j < abs(_len); ++j) {
                    _points.push_back(IPoint(i, j));
                }
            }
        }
        return _points;
    }

}