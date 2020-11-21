#include "Clipboard.h"

namespace FluidLib {

	ClipboardData Clipboard::data = {0,0,1,nullptr};

	void Clipboard::Reset(size_t width, size_t height, size_t elementsize)
	{
		if (width == data.width && height == data.height && elementsize == data.elementsize) {
			std::cout << "Same size, no allocation needed" << std::endl;
			return;
		}
		delete []data.data;
		data.data = nullptr;
		data.width = width;
		data.height = height;
		data.elementsize = elementsize;
		data.data = static_cast<void*>(new char[width * height * elementsize]{0});
		std::cout << "Allocated: " << width * height * elementsize << " Bytes" << std::endl;
	}

	void Clipboard::SetData(ClipboardData data)
	{
		Clear();
		Clipboard::data = data;
	}

	ClipboardData* Clipboard::GetDataStruct()
	{
		return &data;
	}

	void* Clipboard::GetData()
	{
		return data.data;
	}

	void Clipboard::Clear()
	{
		delete []data.data;
		data.data = nullptr;
		data.width = 0;
		data.height = 0;
		data.elementsize = 1;
	}

}