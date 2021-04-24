#pragma once

#include "../Parameters.h"
#include "Tools/Surface.h"
#include "GL/glew.h"
#include <Util/ControlPoint.h>

#define maxHt 800 
#define maxWd 600 
#define maxVer 10000 

namespace FluidLib {

	//https://www.geeksforgeeks.org/scan-line-polygon-filling-using-opengl-c/#:~:text=Scanline%20Polygon%20filling%20Algorithm,the%20vertices%20of%20the%20figure.
	struct EdgeBucket {
		int ymax; //max y-coordinate of edge 
		float xofymin; //x-coordinate of lowest edge point updated only in aet 
		float slopeinverse;
	};

	struct EdgeTableTuple {
		// the array will give the scanline number 
		// The edge table (ET) with edges entries sorted  
		// in increasing y and x of the lower end 
		int countEdgeBucket;    //no. of edgebuckets 
		EdgeBucket buckets[maxVer];
	};

	class Polygon : public Surface
	{
	public:
		Polygon();

		void Draw() const override;
		void OnScroll(float x, float y) override;
		void OnMove(float x, float y) override;
		float GetSurfaceArea() const override;
		//Loop through surface points
		std::vector<IPoint>& GetSurfacePoints() override;

		inline float GetX() { return _pos.GetX(); }
		inline float GetY() { return _pos.GetY(); }
		inline void SetX(float x) { _pos.SetX(x); _changed = true; }
		inline void SetY(float y) { _pos.SetY(y); _changed = true; }

		virtual void StartEdit() override;
		virtual void EditDraw() override;
		virtual bool OnEditMove(float x, float y) override;
		virtual bool OnEditClick(float x, float y) override;
		virtual bool OnEditRelease(float x, float y) override;

		void AddPoint(const ControlPoint& point);
		void RemovePoint(int i);
		ControlPoint& GetPoint(int i);
		int PointCount() const { return _controlpoints.size(); }

		void SetProjection(glm::mat4 proj) override;
		PolygonSurfParameters GetParam();
		void LoadParam(Parameters* p) override;
	private:
		void Refresh();
		void ScanLineAlg();

		std::vector<ControlPoint> _controlpoints;
		FPoint _pos;
		float _color[3] = { 1.0,1.0,1.0 };

		GLuint _buffer = -1;
		static GLuint _shader;
		const char* vertex = "res/shaders/surfaces/polygon_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/polygon_fragmentshader.glsl";

		EdgeTableTuple _edgeTable[maxHt];
		EdgeTableTuple _activeEdgeTuple;
		int _xtrans = 0;
		int _ytrans = 0;
		void SetupScanLine();
		void InitEdgeTable();
		void PrintTuple(EdgeTableTuple* tup);
		void PrintTable();
		/* Function to sort an array using insertion sort*/
		void InsertionSort(EdgeTableTuple* ett);
		void StoreEdgeInTuple(EdgeTableTuple* receiver, int ym, int xm, float slopInv);
		void StoreEdgeInTable(int x1, int y1, int x2, int y2);
		void RemoveEdgeByYmax(EdgeTableTuple* Tup, int yy);
		void Updatexbyslopeinv(EdgeTableTuple* Tup);
	};

}