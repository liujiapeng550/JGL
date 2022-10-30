#pragma once

#include "pch.h"

#include "render/render_base.h"
#include "vertex_holder.h"
#include "elems/element.h"
#include "utils/texturessystem.h"

namespace nelems
{
  class Mesh : public Element
  {
    
  public:

    Mesh() = default;

    virtual ~Mesh();

    bool load(const std::string& filepath);

    void add_vertex(const VertexHolder& vertex) { mVertices.push_back(vertex);  }

    void add_vertex_index(unsigned int vertex_idx) { mVertexIndices.push_back(vertex_idx); }

    std::vector<unsigned int> get_vertex_indices() { return mVertexIndices; }

    void update(nshaders::Shader* shader) override
    {
      // pbr color
      //shader->set_vec3(mColor, "albedo");
      shader->set_texture(GL_TEXTURE0,GL_TEXTURE_2D, mbaseTexture_id);
      shader->set_f1(mRoughness, "roughness");
      shader->set_f1(mMetallic, "metallic");
      shader->set_f1(1.0f, "ao");
    }
    
    glm::vec3 mColor = { 1.0f, 0.0f, 0.0f };
    float mRoughness = 0.2f;
    float mMetallic = 0.1f;
    std::string mBaseTexture;
    unsigned int mbaseTexture_id;

    void init();

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

