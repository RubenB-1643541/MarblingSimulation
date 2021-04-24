#include "Rectangle.h"

#include "Util/Rendering.h"

namespace FluidLib {

    GLuint Rectangle::_buffer = -1;
    GLuint Rectangle::_shader = -1;

    Rectangle::Rectangle()
    {
        _type = "Rectangle";
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
        _widthold = _width;
        _heightold = _height;
        _rotold = _rotation;
    }

    void Rectangle::Draw() const
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

        GLuint rendertexture = glGetUniformLocation(_shader, "rendertexture");
        glUniform1f(rendertexture, _renderTex);
        if(_texture != nullptr)
            _texture->Bind();

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
                int xsign = _width >= 0 ? 1 : -1;
                int ysign = _height >= 0 ? 1 : -1;
                for (int i = xsign * -_width / 2; i < xsign * _width / 2; ++i) {
                    for (int j = ysign * -_height / 2; j < ysign * _height / 2; ++j) {
                        _points.push_back(IPoint(i, j).Rotate(_rotation));
                    }
                }
            }
            else {
                int xsign = _width >= 0 ? 1 : -1;
                int ysign = _height >= 0 ? 1 : -1;
                for (int i = 0; abs(i) < abs(_width); i += xsign) {
                    for (int j = 0; abs(j) < abs(_height); j += ysign) {
                        _points.push_back(IPoint(i, j).Rotate(_rotation));
                    }
                }
            }
        }
        return _points;
    }

    void Rectangle::StartEdit()
    {
        _pos.OnRelease();
        _size.OnRelease();
        _rotcon.OnRelease();
    }

    void Rectangle::EditDraw()
    {
        if (_centered) {
            _pos.SetX(_xpos);
            _pos.SetY(_ypos);
            _size.SetX(_width / 2);
            _size.SetY(_height / 2);
            _size.Rotate(_rotation);
            _size.VisualTranslate({ _xpos, _ypos });
            _rotcon.SetY(_width * sin(_rotation));
            _rotcon.SetX(_width * cos(_rotation));
            _rotcon.VisualTranslate(FPoint(_xpos, _ypos));
            _pos.Draw();
            _size.Draw();
            _rotcon.Draw();
        }
        else {
            _pos.SetX(_xpos + _width / 2);
            _pos.SetY(_ypos + _height / 2);
            _size.SetX(_width);
            _size.SetY(_height);

            _pos.Rotate(_rotation);
            _size.Rotate(_rotation);
            _size.VisualTranslate({ _xpos, _ypos });
            _rotcon.SetY(_width * sin(_rotation));
            _rotcon.SetX(_width * cos(_rotation));
            _rotcon.VisualTranslate(FPoint(_xpos, _ypos));

            _pos.Draw();
            _size.Draw();
            //_rotcon.Draw();
        }
    }

    bool Rectangle::OnEditMove(float x, float y)
    {
        if (_centered) {
            _pos.OnMove(x, y);
            if (_pos.Selected()) {
                _xpos = _pos.GetX();
                _ypos = _pos.GetY();
            }
            _size.OnMove(x, y);
            if (_size.Selected()) {
                _size.Rotate(-_rotation);
                _width = _size.GetX() * 2;
                _height = _size.GetY() * 2;
            }
            _rotcon.OnMove(x, y);
            if (_rotcon.Selected()) {
                _rotation = atan2(y - _ypos, x - _xpos);
            }
        }
        else {
            _pos.OnMove(x, y);
            if (_pos.Selected()) {
                _xpos = _pos.GetX()-_width/2;
                _ypos = _pos.GetY()-_height/2;
            }
            _size.OnMove(x, y);
            if (_size.Selected()) {
                _size.Rotate(-_rotation);
                _width = _size.GetX();
                _height = _size.GetY();
            }
            _rotcon.OnMove(x, y);
            if (_rotcon.Selected()) {
                _rotation = atan2(y - _ypos, x - _xpos);
            }
        }
        return false;
    }

    bool Rectangle::OnEditClick(float x, float y)
    {
        if (_pos.OnClick(x, y))
            return true;
        if (_size.OnClick(x, y))
            return true;
        return _rotcon.OnClick(x, y);
    }

    bool Rectangle::OnEditRelease(float x, float y)
    {
        _pos.OnRelease();
        _size.OnRelease();
        _rotcon.OnRelease();
        return true;
    }

    void Rectangle::SetProjection(glm::mat4 proj)
    {
        _projection = proj;
        _pos.SetProjection(proj);
        _size.SetProjection(proj);
        _rotcon.SetProjection(proj);
    }

    RectangleSurfParameters Rectangle::GetParam()
    {
        RectangleSurfParameters p;
        p.Width = _width;
        p.Height = _height;
        return p;
    }

    void Rectangle::LoadParam(Parameters* p)
    {
        RectangleSurfParameters* rp = static_cast<RectangleSurfParameters*>(p);
        _width = rp->Width;
        _height = rp->Height;
    }

}