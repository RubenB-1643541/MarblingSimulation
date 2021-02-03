#pragma once

#include "Consts/Keys.h"
#include "Core/Events/Events.h"
#include <map>
#include <vector>
#define MAX_KEYS 350

struct ShortCut {
	using Action = std::function<void()>;
	int key = -1;
	bool ctrl;
	bool shift;
	//FunctionPointer Action
	Action action;
};

class ShortCutHandler
{

public:

	void OnKeyPressEvent(RenderEngine::KeyPressEvent e);
	void OnKeyReleaseEvent(RenderEngine::KeyReleaseEvent e);
	void AddShortCut(ShortCut shortcut);
	void RemoveShortCut(ShortCut shortcut);

private:
	bool _shift = false;
	bool _ctrl = false;
	bool _keys[MAX_KEYS];
	std::map<int, std::vector<ShortCut>> _shortcuts;

};

