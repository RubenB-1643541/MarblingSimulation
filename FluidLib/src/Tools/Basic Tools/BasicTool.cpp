#include "BasicTool.h"

void FluidLib::BasicTool::AddSurface(const std::string& name, Surface* surface)
{
	_surfaces.insert(std::make_pair(name, surface));
}

void FluidLib::BasicTool::AddMovement(const std::string& name, Movement* movement)
{
	_movements.insert(std::make_pair(name, movement));
}

void FluidLib::BasicTool::AddAction(const std::string& name, ActionBase* action)
{
	_actions.insert(std::make_pair(name, action));
}

void FluidLib::BasicTool::SetActiveSurface(const std::string& surfacename)
{
	SetSurface(_surfaces[surfacename]);
}

void FluidLib::BasicTool::SetActiveMovement(const std::string& movementname)
{
	SetMovement(_movements[movementname]);
}

void FluidLib::BasicTool::SetActiveAction(const std::string& actionname)
{
	SetAction(_actions[actionname]);
}
