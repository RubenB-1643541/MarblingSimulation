#include "Square.h"

#include "Util/Rendering.h"

namespace FluidLib {

    GLuint Square::_buffer = -1;
    GLuint Square::_shader = -1;

    Square::Square()
    {
        _type = "Square";
        _changed = true;
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
        _lenold = _len;
        _rotold = _rotation;
    }


    void Square::Draw() const
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
        glUniform1f(width, _len);
        GLuint height = glGetUniformLocation(_shader, "height");
        glUniform1f(height, _len);
        GLuint center = glGetUniformLocation(_shader, "center");
        glUniform1i(center, _centered);
        GLuint color = glGetUniformLocation(_shader, "color");
        glUniform3f(color, _color[0], _color[1], _color[2]);
        GLuint proj = glGetUniformLocation(_shader, "projection");
        glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
        glm::mat4 rotation = glm::rotate(_rotation, glm::vec3(0,0,1));
        GLuint rot = glGetUniformLocation(_shader, "rotation");
        glUniformMatrix4fv(rot, 1, GL_FALSE, &rotation[0][0]);
        glm::mat4 translation = glm::translate(glm::vec3(_xpos + _trans.GetX(), _ypos + _trans.GetY(), 0));
        GLuint trans = glGetUniformLocation(_shader, "translation");
        glUniformMatrix4fv(trans, 1, GL_FALSE, &translation[0][0]);
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
        if (_len < 1)
            _len = 1.0f;
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
        if (_lenold != _len)
            _changed = true;
        if (_rotold != _rotation)
            _changed = true;
        if (_changed) {
            _lenold = _len;
            _rotold = _rotation;
            _points.clear();
            if (_centered) {
                for (int i = -abs(_len / 2); i < abs(_len / 2); ++i) {
                    for (int j = -abs(_len / 2); j < abs(_len / 2); ++j) {
                        _points.push_back(IPoint(i, j).Rotate(_rotation));
                    }
                }
            }
            else {
                for (int i = 0; i < abs(_len); ++i) {
                    for (int j = 0; j < abs(_len); ++j) {
                        _points.push_back(IPoint(i, j).Rotate(_rotation));
                    }
                }
            }
        }
        return _points;
    }

    void Square::StartEdit()
    {
        _pos.OnRelease();
        _size.OnRelease();
        _rotcon.OnRelease();
    }

    void Square::EditDraw()
    {
        _pos.SetX(_xpos);
        _pos.SetY(_ypos);
        _size.SetX(_len/2);
        _size.SetY(0);
        _size.Rotate(_rotation);
        _size.VisualTranslate(FPoint( _xpos, _ypos ));
        _rotcon.SetY(_len * sin(_rotation));
        _rotcon.SetX(_len * cos(_rotation));
        _rotcon.VisualTranslate(FPoint(_xpos, _ypos));
        _pos.Draw();
        _size.Draw();
        _rotcon.Draw();
    }

    bool Square::OnEditMove(float x, float y)
    {
        _pos.OnMove(x, y);
        if (_pos.Selected()) {
            _xpos = _pos.GetX();
            _ypos = _pos.GetY();
        }
        _size.OnMove(x, y);
        if (_size.Selected()) {
            _size.Rotate(-_rotation);
            _len = _size.GetX()*2;
        }
        _rotcon.OnMove(x, y);
        if (_rotcon.Selected()) {
            _rotation = atan2(y - _ypos, x - _xpos);
        }
        return true;
    }

    bool Square::OnEditClick(float x, float y)
    {
        if (_pos.OnClick(x, y))
            return true;
        if (_size.OnClick(x, y))
            return true;
        return _rotcon.OnClick(x, y);
  
    }

    bool Square::OnEditRelease(float x, float y)
    {
        _pos.OnRelease();
        _size.OnRelease();
        _rotcon.OnRelease();
        return false;
    }

    void Square::SetProjection(glm::mat4 proj)
    {
        _projection = proj;
        _pos.SetProjection(proj);
        _size.SetProjection(proj);
        _rotcon.SetProjection(proj);
    }

}