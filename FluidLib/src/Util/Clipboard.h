#pragma once

#include <iostream>

namespace FluidLib {

	struct ClipboardData {
		size_t width;
		size_t height;
		size_t elementsize;
		void* data;
	};
	class Clipboard
	{
	public:
	
		static void Reset(size_t widht, size_t height, size_t elementsize);
		static void SetData(ClipboardData data);
		static ClipboardData* GetDataStruct();
		static void* GetData();
		static void Clear();
	private:
		
		static ClipboardData data;

	};

}