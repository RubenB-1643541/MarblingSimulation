#include "BezierCurve.h"
#include "Util/Rendering.h"
#include "Simulation.h"

namespace FluidLib {

    GLuint BezierCurve::_buffer = -1;
    GLuint BezierCurve::_shader = -1;

    BezierCurve::BezierCurve()
    {
        _type = "BezierCurve";
        if (_buffer == -1) {
            glCreateBuffers(1, &_buffer);
            std::vector<float> points;
            for (float i = 0; i < 1.0f; i += _precision) {
                points.push_back(i);
            }
            glBindBuffer(GL_ARRAY_BUFFER, _buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
        }
        if (_shader == -1) {
            _shader = CompileShader(vertex, fragment);
        }
        _controlpoints[0] = FPoint(0, 200);
        _controlpoints[1] = FPoint(0, 0);
        _controlpoints[2] = FPoint(300, 0);
        _controlpoints[3] = FPoint(300, 200);
    }

    bool BezierCurve::OnScroll(float x, float y)
    {
        return false;
    }

    void BezierCurve::DrawPath()
    {
        glUseProgram(_shader);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _buffer);
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);

        GLuint p = glGetUniformLocation(_shader, "control[0]");
        glUniform2f(p, _controlpoints[0].GetX(), _controlpoints[0].GetY());
        p = glGetUniformLocation(_shader, "control[1]");
        glUniform2f(p, _controlpoints[1].GetX(), _controlpoints[1].GetY());
        p = glGetUniformLocation(_shader, "control[2]");
        glUniform2f(p, _controlpoints[2].GetX(), _controlpoints[2].GetY());
        p = glGetUniformLocation(_shader, "control[3]");
        glUniform2f(p, _controlpoints[3].GetX(), _controlpoints[3].GetY());
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

    void BezierCurve::OnEditDraw()
    {
        for (int i = 0; i < 4; ++i) {
            _controlpoints[i].Draw();
        }
    }

    void BezierCurve::StartEdit()
    {
        for (int i = 0; i < 4; ++i) {
            _controlpoints[i].OnRelease();
        }
    }

    bool BezierCurve::OnEditClick(float x, float y)
    {
        for (int i = 0; i < 4; ++i) {
            if (_controlpoints[i].OnClick(x, y))
                return true;
        }
        return false;
    }

    bool BezierCurve::OnEditRelease(float x, float y)
    {
        for (int i = 0; i < 4; ++i) {
            _controlpoints[i].OnRelease();
        }
        return false;
    }

    bool BezierCurve::OnEditMove(float x, float y)
    {
        for (int i = 0; i < 4; ++i) {
            _controlpoints[i].OnMove(x, y);
        }
        return false;
    }

    FPoint BezierCurve::Get(float x, float y)//TODO FIX y from x
    {
        //float t = x / FluidLib::Simulation::Get()->GetSizeX();
        //return pow((1 - t), 3) * _controlpoints[0] + 3 * pow((1 - t), 2) * t * _controlpoints[1] + 3 * (1 - t) * pow(t, 2) * _controlpoints[2] + pow(t, 3) * _controlpoints[3];
        CalculatePoints();
        FPoint pos(x, y);
        float mindis = -1;
        FPoint minpos;
        for (FPoint p : _points) {
            float dis = pos.Distance(p);
            if (mindis == -1) {
                mindis = dis;
                minpos = p;
            }
            else if (dis < mindis) {
                mindis = dis;
                minpos = p;
            }
        }
        return minpos;
    }

    float BezierCurve::GetNormalRotation(float x, float y)
    {
        float t = x / FluidLib::Simulation::Get()->GetSizeX();
        //Derivative
        FPoint p = pow(3 * (1 - t), 2)* _controlpoints[1] - _controlpoints[0] + 6 * (1 - t) * t * (_controlpoints[2] - _controlpoints[1]) + 3 * pow(t, 2) * (_controlpoints[3] - _controlpoints[2]);
        //return p.GetY()/ p.GetX();
        return atan2(p.GetY(), p.GetX());
    }

    void BezierCurve::SetProjection(glm::mat4 proj)
    {
        _projection = proj; 
        for (int i = 0; i < 4; ++i)
            _controlpoints[i].SetProjection(proj);
    }

    void BezierCurve::CalculatePoints()
    {
        _points.clear();
        for (float t = 0.0f; t < 1.0f; t += 0.005) {
            FPoint res = pow((1 - t), 3) * _controlpoints[0] + 3 * pow((1 - t), 2) * t * _controlpoints[1] + 3 * (1 - t) * pow(t, 2) * _controlpoints[2] + pow(t, 3) * _controlpoints[3];
            _points.push_back(res);
        }
    }

    BezierCurveMoveParameters BezierCurve::GetParam()
    {
        BezierCurveMoveParameters p;
        p.p1 = _controlpoints[0];
        p.p2 = _controlpoints[1];
        p.p3 = _controlpoints[2];
        p.p4 = _controlpoints[3];
        return p;
    }

    void BezierCurve::LoadParam(Parameters* p)
    {
        BezierCurveMoveParameters* bp = static_cast<BezierCurveMoveParameters*>(p);
        _controlpoints[0] = bp->p1;
        _controlpoints[1] = bp->p2;
        _controlpoints[2] = bp->p3;
        _controlpoints[3] = bp->p4;
    }

}