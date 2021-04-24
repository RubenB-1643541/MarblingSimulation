#include "Circle.h"

#include "Util/Rendering.h"

namespace FluidLib {
    GLuint Circle::_buffer = -1;
    GLuint Circle::_shader = -1;
    Circle::Circle()
    {
        _type = "Circle";
        _changed = true;
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
        glUniform1f(xpos, _x + _trans.GetX());
        GLuint ypos = glGetUniformLocation(_shader, "ypos"); 
        glUniform1f(ypos, _y + _trans.GetY());
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

    void Circle::EditDraw()
    {
        _pos.SetX(_x);
        _pos.SetY(_y);
        _size.SetX(_r);
        _size.SetY(0);
        _size.VisualTranslate({ _x, _y });
        _pos.Draw();
        _size.Draw();
    }

    void Circle::StartEdit()
    {
        _pos.OnRelease();
        _size.OnRelease();
    }

    bool Circle::OnEditMove(float x, float y)
    {
        _pos.OnMove(x, y);
        if (_pos.Selected()) {
            _x = _pos.GetX();
            _y = _pos.GetY();
        }
        _size.OnMove(x, y);
        if(_size.Selected()) {
            _r = _size.GetX();
        }
        return false;
    }

    bool Circle::OnEditClick(float x, float y)
    {
        if (_pos.OnClick(x, y))
            return true;
        return _size.OnClick(x,y);
    }

    bool Circle::OnEditRelease(float x, float y)
    {
        _pos.OnRelease();
        _size.OnRelease();
        return false;
    }

    void Circle::OnScroll(float x, float y)
    {
        _r += y;
        _changed = true;
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
        if (_rold != _r)
            _changed = true;
        if (_changed) {
            _rold = _r;
            _points.clear();
            for (int i = 0; i < _r; ++i) {
                for (int j = 0; j < sqrt(pow(_r, 2) - pow(i, 2)); ++j) {
                    _points.push_back(IPoint(i + 1, j + 1));
                    _points.push_back(IPoint(-i, j + 1));
                    _points.push_back(IPoint(i + 1, -j));
                    _points.push_back(IPoint(-i, -j));
                }
            }
            _changed = false;
        }
        return _points;
    }

    void Circle::SetProjection(glm::mat4 proj)
    {
        _projection = proj;
        _pos.SetProjection(proj);
        _size.SetProjection(proj);
    }

    CircleSurfParameters Circle::GetParam()
    {
        CircleSurfParameters p;
        p.R = _r;
        return p ;
    }

    void Circle::LoadParam(Parameters* p)
    {
        CircleSurfParameters* cp = static_cast<CircleSurfParameters*>(p);
        _r = cp->R;
    }

}