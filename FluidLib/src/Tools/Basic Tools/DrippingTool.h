#pragma once

#include "../ToolBase.h"
#include "../Surface/Circle.h"
#include "../Surface/Rectangle.h"
#include "../Surface/Triangle.h"
#include <random>
#include <cmath>
#include <corecrt_math_defines.h>
#define _USE_MATH_DEFINES

namespace FluidLib {

	class DrippingTool : public ToolBase
	{
	public:
		DrippingTool(); 
		void Draw() override;
		void OnUpdate() override;

		void OnActivate() override;
		void OnDeactivate() override;

		void OnUse() override;
		bool OnUseEvent(ToolUseEvent& event) override;
		bool OnEndUseEvent(ToolEndUseEvent& event) override;
		bool OnMoveEvent(ToolMoveEvent& event) override;
		bool OnScrollEvent(ToolScrollEvent& event) override;
		inline virtual void SetProjection(glm::mat4 proj) override { _proj = proj; _surface->SetProjection(proj); _rect->SetProjection(proj); _tri->SetProjection(proj); }

		inline float GetWidth() { return _width; }
		inline void SetWidth(float width) { _width = width; }
		inline float* GetWidthPtr() { return &_width; }

		inline float GetHeight() {return _height;}
		inline void SetHeight(float height) {_height = height;}
		inline float* GetHeightPtr() {return &_height;}
		
		inline float GetMinR() {return _minr;}
		inline void SetMinR(float minr) { _minr = minr; }
		inline float* GetMinRPtr() {return &_minr;}

		inline float GetMaxR(){return _maxr;}
		inline void SetMaxR(float maxr) { _maxr = maxr; }
		inline float* GetMaxRPtr(){return &_maxr;}

		inline float GetMinLen() { return _minlen; }
		inline void SetMinLen(float minlen) { _minlen = minlen; }
		inline float* GetMinLenPtr() { return &_minlen; }

		inline float GetMaxLen() {return _maxlen;}
		inline void SetMaxLen(float maxlen) { _maxlen = maxlen; }
		inline float* GetMaxLenPtr() {return &_maxlen;}

		inline float GetMinAngle() { return _minangle; }
		inline void SetMinAngle(float minangle) { _minangle = minangle; }
		inline float* GetMinAnglePtr() { return &_minangle; }

		inline float GetMaxAngle() { return _maxangle; }
		inline void SetMaxAngle(float maxangle) { _maxangle = maxangle; }
		inline float* GetMaxAnglePtr() { return &_maxangle; }

		inline bool GetArc() {return _arc;}
		inline void SetArc(bool arc) { _arc = arc; }
		inline bool* GetArcPtr() {return &_arc;}
	private:
		void SetCircleRandom();
		void SetCircleRandomArc();
		float _xpos = 0;
		float _ypos = 0;
		float _width = 100;
		float _height = 100;
		float _minr = 5;
		float _maxr = 15;

		bool _arc = false;

		float _minlen = 50;
		float _maxlen = 300;
		float _minangle = -0.5;
		float _maxangle = 0.5;
		Circle* _circle = nullptr;
		Rectangle* _rect = nullptr;
		Triangle* _tri = nullptr;
	};

}