#pragma once

#include "Grid.h"
#include <vector>
#include "glm/gtx/transform.hpp"

namespace FluidLib {

	template <class T>
	class ColorGrid : public Grid<T>
	{
	public:
		ColorGrid();
		/*
		Parameters:
		- GLuint id: ShaderStorageBuffer Id
		- int size: Number of elements in the buffer, not the size of the buffer
		*/
		ColorGrid(GLuint id, int size, bool render = false);
		ColorGrid(GLuint id, int size, int attr, bool render = false);
		ColorGrid(GLuint id, int size, int attr, int colattr, bool render = false);
		uint32_t GetId() { return _id; }
		void SetId(uint32_t id) { _id = id; }
		void AddColor(glm::vec3 col);
		void ClearColors();
		glm::vec3 GetColor(int id) { return _colors[id]; }
		std::vector<glm::vec3>& GetColors() { return _colors; }
		void SetColAttr(int attr) { _colattr = attr; }

		void UseGrid() override;
		void DrawGrid() override;
		void WriteToFile(std::ofstream& stream) override;
	private:
		void CreateColorBuffer();
		uint32_t _id = 0;
		std::vector<glm::vec3> _colors = { {0,0,0} };
		bool _colorchange = false;

		int _colattr = 0;
		GLuint _colid = -1;
	};

	template<class T>
	inline ColorGrid<T>::ColorGrid()
	{
		CreateColorBuffer();
	}

	template<class T>
	inline ColorGrid<T>::ColorGrid(GLuint id, int size, bool render) : Grid<T>(id, size, render)
	{
		CreateColorBuffer();
		//SetBufferId(id);
		//SetSize(size);
		//SetRender(render);
	}

	template<class T>
	inline ColorGrid<T>::ColorGrid(GLuint id, int size, int attr, bool render) : Grid<T>(id, size, attr, render)
	{
		CreateColorBuffer();
		//SetBufferId(id);
		//SetSize(size);
		//SetAttrPointer(attr);
		//SetRender(render);
	}

	template<class T>
	inline ColorGrid<T>::ColorGrid(GLuint id, int size, int attr, int colattr, bool render) : Grid<T>(id, size, attr, render)
	{
		_colattr = colattr;
		CreateColorBuffer();
	}

	template<class T>
	inline void ColorGrid<T>::AddColor(glm::vec3 col)
	{
		_colors.push_back(col); 
		++_id; 
		_colorchange = true;
	}

	template<class T>
	inline void ColorGrid<T>::ClearColors()
	{
		_colors.clear();
		_id = 0;
	}

	template<class T>
	inline void ColorGrid<T>::UseGrid()
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _data.attrpointer, _data.id);
	}

	template<class T>
	inline void ColorGrid<T>::DrawGrid()
	{
		if (GetRender()) {

			if (_colorchange) {
				//TODO Change buffer size of more then 10 cols
				glNamedBufferSubData(_colid, (_colors.size()-1) * sizeof(glm::vec4), sizeof(glm::vec4), &_colors[_colors.size()-1]);
				_colorchange = false;
				//GLint bufMask = GL_MAP_WRITE_BIT;
				//glBindBuffer(GL_SHADER_STORAGE_BUFFER, _colid);
				//glm::vec4* cols = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, _colors.size() * sizeof(glm::vec4), bufMask);
				//for (int i = 0; i < _colors.size(); ++i) {
				//	std::cout << cols[i].x << " " << cols[i].y << " " << cols[i].z << std::endl;
				//}
				//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
			}

			glBindBuffer(GL_ARRAY_BUFFER, _data.id );
			glVertexAttribIPointer(0, _data.elementsize, _data.type, 0, (void*)0);
			//glVertexAttributePointer(_data.elementsize, _data.type, 0, (void*)0);
			glEnableVertexAttribArray(_data.attrpointer);
			//Load colors
			//TODO
			//glBindBuffer(GL_UNIFORM_BUFFER, _colid);
			glBindBufferRange(GL_UNIFORM_BUFFER, _colattr, _colid, 0, _colors.size() * sizeof(glm::vec4));
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, 0, (void*)0);
			glEnableVertexAttribArray(_colattr);
		}
	}

	template<class T>
	inline void ColorGrid<T>::WriteToFile(std::ofstream& stream)
	{
		//Write Colors
		stream << _colors.size() << std::endl;
		for (int i = 0; i < _colors.size(); ++i) {
			stream << _colors[i].r << " " << _colors[i].g << " " << _colors[i].b << " ";
		}
		stream << std::endl;
		//Write Data;
		T* data = GetBufferPointer();
		for (int i = 0; i < _data.size; ++i) {
			stream << data[i] << " ";
		}
		ReleaseBufferLock();
	}

	template<class T>
	inline void ColorGrid<T>::CreateColorBuffer()
	{
		//_colors.push_back({ 0,0,0 });
		glGenBuffers(1, &_colid);
		glBindBuffer(GL_UNIFORM_BUFFER, _colid);
		glBufferData(GL_UNIFORM_BUFFER, 10 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, _colors.size() * sizeof(glm::vec3), &_colors[0], GL_STATIC_DRAW);
		//Copy buffer data
		//if(_colors.size() > 0)
		glNamedBufferSubData(_colid, 0, _colors.size() * sizeof(glm::vec4), &_colors[0]);
		//GLint bufMask = GL_MAP_WRITE_BIT;
		//glm::vec4* cols = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, _colors.size() * sizeof(glm::vec4), bufMask);
		//for (int i = 0; i < _colors.size(); ++i) {
		//	std::cout << cols[i].x << " " << cols[i].y << " "  << cols[i].z << std::endl;
		//}
		//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

}