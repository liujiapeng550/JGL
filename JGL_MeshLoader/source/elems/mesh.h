#pragma once

#include "pch.h"

#include "render/render_base.h"
#include "vertex_holder.h"
#include "elems/element.h"
#include "utils/texturessystem.h"

namespace nelems
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(std::vector<VertexHolder> vertices, std::vector<unsigned int> indices);
		Mesh(float(&arr)[]);
		// �������캯��
		Mesh(const Mesh& other);
		virtual ~Mesh();
		bool load(const std::string& filepath);
		void add_vertex(const VertexHolder& vertex) { mVertices.push_back(vertex); }
		void add_vertex_index(unsigned int vertex_idx) { mVertexIndices.push_back(vertex_idx); }
		std::vector<unsigned int> get_vertex_indices() { return mVertexIndices; }
		void create_buffers();
		void delete_buffers();
		void render();
		void bind();
		void unbind();
	private:
		// Buffers manager
		std::unique_ptr<nrender::VertexIndexBuffer> mRenderBufferMgr;
		// Vertices and indices
		std::vector<VertexHolder> mVertices;
		std::vector<unsigned int> mVertexIndices;
	};
}

