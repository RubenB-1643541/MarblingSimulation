#pragma once

#include "Tools/ToolBase.h"
#include <map>
#include <string>
#include <Tools/Surface/Multisurface/MultiSurfaces.h>
#include "Simulation.h"

namespace FluidLib {

	class BasicTool : public ToolBase
	{
	public:
		BasicTool();
		void Draw() override;
		virtual void OnUpdate() override;

		void AddSurface(const std::string& name, Surface* surface);
		void AddMovement(const std::string& name, Movement* movement);
		void AddAction(const std::string& name, ActionBase* action);
		void AddMultisurface(const std::string& name, MultiSurface* multisurface);

		void SetActiveSurface(const std::string& surfacename);
		void SetActiveMovement(const std::string& movementname);
		void SetActiveAction(const std::string& actionname, bool firstactive = false);
		void SetActiveAction(const std::string& actionname, int button);
		void SetActiveMultisurface(const std::string& multisurfacename);

		inline const std::vector<std::string>& GetSurfaceKeys() { return _surfacekeys; }
		inline const std::vector<std::string>& GetMovementKeys() { return _movementkeys; }
		inline const std::vector<std::string>& GetActionKeys() { return _actionkeys; }
		inline const std::vector<std::string>& GetMultisurfaceKeys() { return _multisurfacekeys; }
		inline const std::vector<std::string>& GetActiveActions() { return _activeactions; }

		inline Surface* GetSurface(std::string surface) {return _surfaces[surface];}
		inline Movement* GetMovement(std::string movement) {return _movements[movement];}
		inline ActionBase* GetAction(std::string action) {return _actions[action];}
		inline MultiSurface* GetMultiSurface(std::string multisurface) {return _multisurfaces[multisurface];}

		inline MultiSurface* GetMultisurface() { return _multisurface; }

		void OnBeginUse() override;
		void OnEndUse() override;
		void OnUse() override;
	protected:
		MultiSurface* _multisurface = nullptr;

	private:
		void DrawSurface();
		std::map<std::string, Surface*> _surfaces;
		std::map<std::string, Movement*> _movements;
		std::map<std::string, ActionBase*> _actions;
		std::map<std::string, MultiSurface*> _multisurfaces;
		std::vector<std::string> _surfacekeys;
		std::vector<std::string> _movementkeys;
		std::vector<std::string> _actionkeys;
		std::vector<std::string> _multisurfacekeys;
		std::vector<std::string> _activeactions;
	};

}