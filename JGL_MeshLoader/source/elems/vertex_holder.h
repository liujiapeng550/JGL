#pragma once
#define MAX_BONE_INFLUENCE 4

namespace nelems
{
  class VertexHolder
  {

  public:
    VertexHolder() : mPos(), mNormal() {}
    
    // Initialize by string to be tokenized
    // TODO: Better handle outside of this class
    VertexHolder(const std::vector<std::string> tokens)
    {
    }

    VertexHolder(const glm::vec3& pos, const glm::vec3& normal)
      : mPos(pos), mNormal(normal)
    {
    }

    ~VertexHolder() = default;


    glm::vec3 mPos;
    glm::vec3 mNormal;
    glm::vec2 mTextureCoords;
    // tangent
    glm::vec3 mTangent;
    // bitangent
    glm::vec3 mBitangent;
    //bone indexes which will influence this vertex
    int mBoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float mWeights[MAX_BONE_INFLUENCE];
    // TODO
    // glm::vec2 mUV;


  };
}

