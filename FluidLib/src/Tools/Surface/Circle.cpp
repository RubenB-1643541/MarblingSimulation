#include "Circle.h"

#include "Util/Rendering.h"

namespace FluidLib {
    GLuint Circle::_buffer = -1;
    GLuint Circle::_shader = -1;
    Circle::Circle()
    {
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
    void Circle::Draw() const
    {
        //glBegin(GL_TRIANGLES);
        //glVertex3f(0,0,0);
        //glVertex3f(0.5, _r,0);
        //glVertex3f(_r, 0,0);
        //glEnd();

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
        if (_style == STYLE::FILLED)
            glDrawArrays(GL_POLYGON, 0, (int)(1 / _precision));
        else if (_style == STYLE::DASHED)
            glDrawArrays(GL_LINES, 0, (int)(1 / _precision));//Dashed line
        else
            glDrawArrays(GL_LINE_LOOP, 0, (int)(1 / _precision));
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
    }

    void Circle::OnScroll(float x, float y)
    {
        _r += y;
    }

    void Circle::OnMove(float x, float y)
    {
        _x = x;
        _y = y;
    }

    float Circle::GetSurfaceArea() const
    {
        return M_PI * _r * _r;
    }

    std::vector<IPoint>& Circle::GetSurfacePoints()
    {
        return _points;
    }

}