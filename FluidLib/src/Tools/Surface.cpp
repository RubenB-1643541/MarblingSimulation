#include "Surface.h"

void FluidLib::Surface::OnRotate(float r)
{
	if (_autorotate) {
		_rotation = r;

	}
}

void FluidLib::Surface::SetTexture(Texture* tex) 
{ 
	delete _texture;
	_texture = tex; 
}

