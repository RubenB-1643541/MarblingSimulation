#include "ShortCutHandler.h"

void ShortCutHandler::OnKeyPressEvent(RenderEngine::KeyPressEvent e)
{
	int key = e.GetKey();
	_keys[key] = true;
	if (key == KEY_LEFT_SHIFT || key == KEY_RIGHT_SHIFT)
		_shift = true;
	if (key == KEY_LEFT_CONTROL || key == KEY_RIGHT_CONTROL)
		_ctrl = true;

	std::vector<ShortCut> shortcuts = _shortcuts[key];
	for (ShortCut& shortcut : shortcuts) {
		if (shortcut.key != -1 && shortcut.shift == _shift && shortcut.ctrl == _ctrl) {
			shortcut.action();
		}
	}
	//Check Shortcut
}

void ShortCutHandler::OnKeyReleaseEvent(RenderEngine::KeyReleaseEvent e)
{
	int key = e.GetKey();
	_keys[key] = false;
	if (key == KEY_LEFT_SHIFT || key == KEY_RIGHT_SHIFT)
		_shift = false;
	if (key == KEY_LEFT_CONTROL || key == KEY_RIGHT_CONTROL)
		_ctrl = false;
}

void ShortCutHandler::AddShortCut(ShortCut shortcut)
{
	_shortcuts[shortcut.key].push_back(shortcut);
	//_shortcuts.insert(std::make_pair(shortcut.key, shortcut));
}

void ShortCutHandler::RemoveShortCut(ShortCut shortcut)
{
}
