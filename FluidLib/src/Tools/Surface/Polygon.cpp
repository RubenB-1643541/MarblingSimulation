#include "Polygon.h"

namespace FluidLib {

	GLuint Polygon::_shader = -1;

	Polygon::Polygon()
	{
		_changed = true;
		_type = "Polygon";
		//_centered = true;
		//AddPoint(ControlPoint(0.0f, 0.0f));
		//AddPoint(ControlPoint(50.0f, 0.0f));
		//AddPoint(ControlPoint(50.0f, 50.0f));
		//AddPoint(ControlPoint(0.0f, 50.0f));
		AddPoint(ControlPoint(50.0f, 30.0f));
		AddPoint(ControlPoint(-20.0f, 15.0f));
		AddPoint(ControlPoint(-40.0f, -51.0f));
		AddPoint(ControlPoint(30.0f, -50.0f));
		if (_buffer == -1) {
			glCreateBuffers(1, &_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, _buffer);
			glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * _controlpoints.size(), &_controlpoints[0], GL_DYNAMIC_DRAW);
		}
		if (_shader == -1) {
			_shader = CompileShader(vertex, fragment);
		}
		Refresh();
	}

	void Polygon::Draw() const
	{
		glUseProgram(_shader);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ControlPoint), 0);

		GLuint xpos = glGetUniformLocation(_shader, "xpos");
		glUniform1f(xpos, _pos.GetX() + _trans.GetX());
		GLuint ypos = glGetUniformLocation(_shader, "ypos");
		glUniform1f(ypos, _pos.GetY() + _trans.GetY());
		//GLuint width = glGetUniformLocation(_shader, "width");
		//GLuint center = glGetUniformLocation(_shader, "center");
		//glUniform1i(center, _centered);
		GLuint color = glGetUniformLocation(_shader, "color");
		glUniform3f(color, _color[0], _color[1], _color[2]);
		GLuint proj = glGetUniformLocation(_shader, "projection");
		glUniformMatrix4fv(proj, 1, GL_FALSE, &_projection[0][0]);
		//if (_style == STYLE::FILLED)
		//glDrawArrays(GL_POLYGON, 0, PointCount());
		//else if (_style == STYLE::DASHED)
		//	glDrawArrays(GL_LINES, 0, 6);//Dashed line
		//else
		glDrawArrays(GL_LINE_LOOP, 0, PointCount());
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void Polygon::OnScroll(float x, float y)
	{
		float mul;
		if (y > 0.0f)
			mul = 1.1;
		else
			mul = 0.9;
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i] *= mul;
		}
		Refresh();
	}

	void Polygon::OnMove(float x, float y)
	{
		FPoint newpos = { x, y };
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i].VisualTranslate({ x, y });
		}
		_pos = newpos;
		//Refresh();
	}

	float Polygon::GetSurfaceArea() const
	{
		return 0.0f;
	}

	void Polygon::StartEdit()
	{
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i].OnRelease();
		}
	}

	void Polygon::EditDraw()
	{
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i].Draw();
		}
	}

	bool Polygon::OnEditMove(float x, float y)
	{
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i].OnMove(x, y);
		}
		Refresh();
		return false;
	}

	bool Polygon::OnEditClick(float x, float y)
	{
		for (int i = 0; i < PointCount(); ++i) {
			if (_controlpoints[i].OnClick(x, y))
				return true;
		}
		return false;
	}

	bool Polygon::OnEditRelease(float x, float y)
	{
		for (int i = 0; i < PointCount(); ++i) {
			_controlpoints[i].OnRelease();
		}
		return true;
	}

	void Polygon::AddPoint(const ControlPoint& point)
	{

		_controlpoints.push_back(point);
		_controlpoints[PointCount() - 1].SetProjection(_projection);
		_controlpoints[PointCount() - 1].VisualTranslate(_pos);
		Refresh();
	}

	void Polygon::RemovePoint(int i)
	{
		std::vector<ControlPoint>::iterator it = _controlpoints.begin() + i;
		_controlpoints.erase(it);
		Refresh();
	}

	ControlPoint& Polygon::GetPoint(int i)
	{
		return _controlpoints[i];
	}

	void Polygon::SetProjection(glm::mat4 proj)
	{
		_projection = proj;
		for (int i = 0; i < _controlpoints.size(); ++i)
			_controlpoints[i].SetProjection(proj);
	}

	void Polygon::Refresh()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		//std::cout << sizeof(ControlPoint) << std::endl;
		glBufferData(GL_ARRAY_BUFFER, sizeof(ControlPoint) * _controlpoints.size(), &_controlpoints[0], GL_DYNAMIC_DRAW);
		_changed = true;
	}

	
	std::vector<IPoint>& Polygon::GetSurfacePoints()
	{
		if (_changed)
			ScanLineAlg();
		return _points;
	}

	void Polygon::ScanLineAlg()
	{
		SetupScanLine();

		/* Follow the following rules:
		1. Horizontal edges: Do not include in edge table
		2. Horizontal edges: Drawn either on the bottom or on the top.
		3. Vertices: If local max or min, then count twice, else count
			once.
		4. Either vertices at local minima or at local maxima are drawn.*/


		int i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;

		// we will start from scanline 0;  
		// Repeat until last scanline: 
		for (i = 0; i < maxHt; i++)//4. Increment y by 1 (next scan line) 
		{

			// 1. Move from ET bucket y to the 
			// AET those edges whose ymin = y (entering edges) 
			for (j = 0; j < _edgeTable[i].countEdgeBucket; j++)
			{
				StoreEdgeInTuple(&_activeEdgeTuple, _edgeTable[i].buckets[j].
					ymax, _edgeTable[i].buckets[j].xofymin,
					_edgeTable[i].buckets[j].slopeinverse);
			}
			//PrintTuple(&_activeEdgeTuple);

			// 2. Remove from AET those edges for  
			// which y=ymax (not involved in next scan line) 
			RemoveEdgeByYmax(&_activeEdgeTuple, i);

			//sort AET (remember: ET is presorted) 
			InsertionSort(&_activeEdgeTuple);

			//PrintTuple(&_activeEdgeTuple);

			//3. Fill lines on scan line y by using pairs of x-coords from AET 
			j = 0;
			FillFlag = 0;
			coordCount = 0;
			x1 = 0;
			x2 = 0;
			ymax1 = 0;
			ymax2 = 0;
			while (j < _activeEdgeTuple.countEdgeBucket)
			{
				if (coordCount % 2 == 0)
				{
					x1 = (int)(_activeEdgeTuple.buckets[j].xofymin);
					ymax1 = _activeEdgeTuple.buckets[j].ymax;
					if (x1 == x2)
					{
						/* three cases can arrive-
							1. lines are towards top of the intersection
							2. lines are towards bottom
							3. one line is towards top and other is towards bottom
						*/
						if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
						{
							x2 = x1;
							ymax2 = ymax1;
						}

						else
						{
							coordCount++;
						}
					}

					else
					{
						coordCount++;
					}
				}
				else
				{
					x2 = (int)_activeEdgeTuple.buckets[j].xofymin;
					ymax2 = _activeEdgeTuple.buckets[j].ymax;

					FillFlag = 0;

					// checking for intersection... 
					if (x1 == x2)
					{
						/*three cases can arive-
							1. lines are towards top of the intersection
							2. lines are towards bottom
							3. one line is towards top and other is towards bottom
						*/
						if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
						{
							x1 = x2;
							ymax1 = ymax2;
						}
						else
						{
							coordCount++;
							FillFlag = 1;
						}
					}
					else
					{
						coordCount++;
						FillFlag = 1;
					}


					if (FillFlag)
					{
						int sign = 1;
						if (x2 < x1)
							sign = -1;
						for (int k = x1; k < x2; k += sign) {
							_points.push_back({ k - _xtrans,i -_ytrans });
						}
						//_points.push_back({ x1, i });
						//_points.push_back({ x2, i });
					}
				}
				j++;
			}
			// 5. For each nonvertical edge remaining in AET, update x for new y 
			Updatexbyslopeinv(&_activeEdgeTuple);
		}


		//printf("\nScanline filling complete");
	}

	void Polygon::SetupScanLine()
	{
		_points.clear();
		InitEdgeTable();

		_xtrans = 0;
		_ytrans = 0;
		for (int i = 0; i < _controlpoints.size(); ++i) {
			if (_controlpoints[i].GetX() + _xtrans < 0)
				_xtrans = -_controlpoints[i].GetX();
			if (_controlpoints[i].GetY() + _ytrans < 0)
				_ytrans = -_controlpoints[i].GetY();
		}

		int x1, x2, y1, y2;
		x1 = _controlpoints[0].GetX();
		y1 = _controlpoints[0].GetY();
		for (int i = 1; i < _controlpoints.size(); ++i) {
			x2 = _controlpoints[i].GetX();
			y2 = _controlpoints[i].GetY();
			StoreEdgeInTable(x1+_xtrans, y1 + _ytrans, x2 + _xtrans, y2 + _ytrans);
			x1 = x2;
			y1 = y2;
		}
		x2 = _controlpoints[0].GetX();
		y2 = _controlpoints[0].GetY();
		x1 = _controlpoints[_controlpoints.size() - 1].GetX();
		y1 = _controlpoints[_controlpoints.size() - 1].GetY();
		StoreEdgeInTable(x1 + _xtrans, y1 + _ytrans, x2 + _xtrans, y2 + _ytrans);
	}

	void Polygon::InitEdgeTable()
	{
		int i;
		for (i = 0; i < maxHt; i++)
		{
			_edgeTable[i].countEdgeBucket = 0;
		}

		_activeEdgeTuple.countEdgeBucket = 0;
	}

	void Polygon::PrintTuple(EdgeTableTuple* tup)
	{
		int j;

		if (tup->countEdgeBucket)
			printf("\nCount %d-----\n", tup->countEdgeBucket);

		for (j = 0; j < tup->countEdgeBucket; j++)
		{
			printf(" %d+%.2f+%.2f",
				tup->buckets[j].ymax, tup->buckets[j].xofymin, tup->buckets[j].slopeinverse);
		}
	}

	void Polygon::PrintTable()
	{
		int i, j;
		for (i = 0; i < maxHt; i++)
		{
			if (_edgeTable[i].countEdgeBucket)
				printf("\nScanline %d", i);

			PrintTuple(&_edgeTable[i]);
		}
	}

	void Polygon::InsertionSort(EdgeTableTuple* ett)
	{
		int i, j;
		EdgeBucket temp;

		for (i = 1; i < ett->countEdgeBucket; i++)
		{
			temp.ymax = ett->buckets[i].ymax;
			temp.xofymin = ett->buckets[i].xofymin;
			temp.slopeinverse = ett->buckets[i].slopeinverse;
			j = i - 1;

			while ((temp.xofymin < ett->buckets[j].xofymin) && (j >= 0))
			{
				ett->buckets[j + 1].ymax = ett->buckets[j].ymax;
				ett->buckets[j + 1].xofymin = ett->buckets[j].xofymin;
				ett->buckets[j + 1].slopeinverse = ett->buckets[j].slopeinverse;
				j = j - 1;
			}
			ett->buckets[j + 1].ymax = temp.ymax;
			ett->buckets[j + 1].xofymin = temp.xofymin;
			ett->buckets[j + 1].slopeinverse = temp.slopeinverse;
		}
	}

	void Polygon::StoreEdgeInTuple(EdgeTableTuple* receiver, int ym, int xm, float slopInv)
	{
		// both used for edgetable and active edge table.. 
		// The edge tuple sorted in increasing ymax and x of the lower end. 
		(receiver->buckets[(receiver)->countEdgeBucket]).ymax = ym;
		(receiver->buckets[(receiver)->countEdgeBucket]).xofymin = (float)xm;
		(receiver->buckets[(receiver)->countEdgeBucket]).slopeinverse = slopInv;

		// sort the buckets 
		InsertionSort(receiver);

		(receiver->countEdgeBucket)++;
	}

	void Polygon::StoreEdgeInTable(int x1, int y1, int x2, int y2)
	{
		float m, minv;
		int ymaxTS, xwithyminTS, scanline; //ts stands for to store 

		if (x2 == x1)
		{
			minv = 0.000000;
		}
		else
		{
			m = ((float)(y2 - y1)) / ((float)(x2 - x1));

			// horizontal lines are not stored in edge table 
			if (y2 == y1)
				return;

			minv = (float)1.0 / m;
			//printf("\nSlope string for %d %d & %d %d: %f", x1, y1, x2, y2, minv);
		}

		if (y1 > y2)
		{
			scanline = y2;
			ymaxTS = y1;
			xwithyminTS = x2;
		}
		else
		{
			scanline = y1;
			ymaxTS = y2;
			xwithyminTS = x1;
		}
		// the assignment part is done..now storage.. 
		StoreEdgeInTuple(&_edgeTable[scanline], ymaxTS, xwithyminTS, minv);
	}

	void Polygon::RemoveEdgeByYmax(EdgeTableTuple* Tup, int yy)
	{
		int i, j;
		for (i = 0; i < Tup->countEdgeBucket; i++)
		{
			if (Tup->buckets[i].ymax == yy)
			{
				//printf("\nRemoved at %d", yy);

				for (j = i; j < Tup->countEdgeBucket - 1; j++)
				{
					Tup->buckets[j].ymax = Tup->buckets[j + 1].ymax;
					Tup->buckets[j].xofymin = Tup->buckets[j + 1].xofymin;
					Tup->buckets[j].slopeinverse = Tup->buckets[j + 1].slopeinverse;
				}
				Tup->countEdgeBucket--;
				i--;
			}
		}
	}

	void Polygon::Updatexbyslopeinv(EdgeTableTuple* Tup)
	{
		int i;
		for (i = 0; i < Tup->countEdgeBucket; i++)
		{
			(Tup->buckets[i]).xofymin = (Tup->buckets[i]).xofymin + (Tup->buckets[i]).slopeinverse;
		}
	}

}