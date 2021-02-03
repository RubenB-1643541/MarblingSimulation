#pragma once

#include "MultiSurface.h"

namespace FluidLib {

	class Comb : public MultiSurface
	{
	public:
		Comb();
		virtual std::vector<FPoint>& GetPoints() override;

		float GetLen() { return _len; }
		void SetLen(float len) { _len = len; }
		float* GetLenPtr() { return &_len; }

		int GetRep() { return _rep; }
		void SetRep(int rep) { _rep = rep; }
		int* GetRepPtr() { return &_rep; }

		bool GetVert() { return _vert; }
		void SetVert(bool vert) { _vert = vert; }
		bool* GetVertPtr() { return &_vert; }

	private:
		float _len = 20;
		float _lenold;
		int _rep = 5;
		int _repold;
		bool _vert = false;
		bool _vertold;
	};

}