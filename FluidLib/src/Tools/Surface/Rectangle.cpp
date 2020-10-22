#include "Rectangle.h"

#include "Util/Rendering.h"

namespace FluidLib {

    GLuint Rectangle::_buffer = -1;
    GLuint Rectangle::_shader = -1;

    Rectangle::Rectangle()
    {
        _type = "Rectangle";
        _centered = true;
        if (_buffer == -1) {
            glCreateBuffers(1, &_buffer);
            std::vector<float> points = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
            glBindBuffer(GL_ARRAY_BUFFER, _buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
        }
        if (_shader == -1) {
            _shader = CompileShader(vertex, fragment);
        }
    }

    void Rectangle::Draw() const
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
        glUniform1f(width, _width);
        GLuint height = glGetUniformLocation(_shader, "height");
        glUniform1f(height, _height);
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

    void Rectangle::OnScroll(float x, float y)
    {
        _width += y * 2;
        _height += y * 2;
        if (_width < 1)
            _width = 1.0f;
        if (_height < 1)
            _height = 1.0f;
        _changed = true;
    }

    void Rectangle::OnMove(float x, float y)
    {
        _xpos = x; _ypos = y;
    }

    float Rectangle::GetSurfaceArea() const
    {
        return _width * _height;
    }

    std::vector<IPoint>& Rectangle::GetSurfacePoints()
    {
        if (_changed) {
            _points.clear();
            if (_centered) {
                for (int i = -abs(_width / 2); i < abs(_width / 2); ++i) {
                    for (int j = -abs(_height / 2); j < abs(_height / 2); ++j) {
                        _points.push_back(IPoint(i, j));
                    }
                }
            }
            else {
                for (int i = 0; i < abs(_width); ++i) {
                    for (int j = 0; j < abs(_height); ++j) {
                        _points.push_back(IPoint(i, j));
                    }
                }
            }
        }
        return _points;
    }

}