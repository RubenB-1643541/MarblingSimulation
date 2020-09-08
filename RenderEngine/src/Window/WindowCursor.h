#pragma once
#include <string>

namespace RenderEngine {

	struct Image {
		int width, height;
		unsigned char* data;
	};

	struct Cursor
	{
		Image image;
		int centerx, centery = 0;
		std::string name;
	};

	struct WindowCursor {
		Cursor cursorObj;
		GLFWcursor* cursorPtr;
	};

}

