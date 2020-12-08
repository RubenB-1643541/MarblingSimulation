#pragma once

#include "Tools/ToolBase.h"
#include <map>
#include <string>

namespace FluidLib {

	class BasicTool : public ToolBase
	{
	public:
		BasicTool();
		void AddSurface(const std::string& name, Surface* surface);
		void AddMovement(const std::string& name, Movement* movement);
		void AddAction(const std::string& name, ActionBase* action);
		void SetActiveSurface(const std::string& surfacename);
		void SetActiveMovement(const std::string& movementname);
		void SetActiveAction(const std::string& actionname);
		inline const std::vector<std::string>& GetSurfaceKeys() { return _surfacekeys; }
		inline const std::vector<std::string>& GetMovementKeys() { return _movementkeys; }
		inline const std::vector<std::string>& GetActionKeys() { return _actionkeys; }
	private:
		std::map<std::string, Surface*> _surfaces;
		std::map<std::string, Movement*> _movements;
		std::map<std::string, ActionBase*> _actions;
		std::vector<std::string> _surfacekeys;
		std::vector<std::string> _movementkeys;
		std::vector<std::string> _actionkeys;
	};

}