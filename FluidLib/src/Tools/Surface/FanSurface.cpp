#include "FanSurface.h"
#include "Util/Rendering.h"
namespace FluidLib {

    GLuint FanSurface::_buffer = -1;
    GLuint FanSurface::_shader = -1;

    FanSurface::FanSurface()
    {
        if (_buffer == -1) {
            glCreateBuffers(1, &_buffer);
            std::vector<float> points = { 
                -15.0, 0.0, -15.0, -1.0,
                15.0, 0.0, 15.0, -1.0
            };
            glBindBuffer(GL_ARRAY_BUFFER, _buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
        }
        if (_shader == -1) {
            _shader = CompileShader(vertex, fragment);
        }
    }

    void FanSurface::Draw() const
    {
        glUseProgram(_shader);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _buffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        GLuint xpos = glGetUniformLocation(_shader, "xpos");
        glUniform1f(xpos, _xpos + _trans.GetX());
        GLuint ypos = glGetUniformLocation(_shader, "ypos");
        glUniform1f(ypos, _ypos + _trans.GetY());
        GLuint ang = glGetUniformLocation(_shader, "angle");
        glUniform1f(ang, _angle);
        GLuint len = glGetUniformLocation(_shader, "len");
        glUniform1f(len, _len);
        GLuint color = glGetUniformLocation(_shader, "color");
        glUniform3f(color, 1.0f, 1.0f, 1.0f);
        GLuint proj = glGetUniformLocation(_shader, "projection");
        glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
        //GLuint rot = glGetUniformLocation(_shader, "rotation");
        //glUniformMatrix4fv(rot, 1, GL_FALSE, &rotation[0][0]);
        //glm::mat4 translation = glm::translate(glm::vec3(_xpos + _trans.GetX(), _ypos + _trans.GetY(), 0));
        //GLuint trans = glGetUniformLocation(_shader, "translation");
        glDrawArrays(GL_LINES, 0, 4);
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
    }
    
    void FanSurface::OnScroll(float x, float y)
    {
        _angle += y/20;
        if (_angle > M_PI / 2)
            _angle = M_PI / 2;
        if (_angle < 0)
            _angle = 0;
    }
    
    void FanSurface::OnMove(float x, float y)
    {
        _xpos = x;
        _ypos = y;
    }
    
    float FanSurface::GetSurfaceArea() const
    {
        return 0.0f;
    }
    
    std::vector<IPoint>& FanSurface::GetSurfacePoints()
    {
        _points.clear();
        int n = 15;
        for (float i = -_angle; i <= _angle; i += _angle/n) {
            for (float j = 0; j < _len; ++j) {
                _points.push_back({ (int)(sin(i)*2*j), (int)(-j * cos(i)) });
            }
        }
        return _points;
    }

}