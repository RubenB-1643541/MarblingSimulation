#include "Surface.h"

void FluidLib::Surface::OnRotate(float r)
{
	if (_autorotate) {
		_rotation = r;

	}
}
