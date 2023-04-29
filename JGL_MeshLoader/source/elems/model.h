#pragma once
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;
namespace nelems
{
    class AssimpGLMHelpers
    {
    public:

        static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
        {
            glm::mat4 to;
            //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
            to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
            to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
            to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
            to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
            return to;
        }

        static inline glm::vec3 GetGLMVec(const aiVector3D& vec)
        {
            return glm::vec3(vec.x, vec.y, vec.z);
        }

        static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
        {
            return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
        }
    };

    class Model
    {
    public:
        // model data 
        //vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        vector<nelems::Mesh>  meshes;
        string directory;
        bool gammaCorrection;

        Model(string const& path, bool gamma = false) : gammaCorrection(gamma)
        {
            loadModel(path);
        }

        // draws the model, and thus all its meshes
        void Draw()
        {
            for (unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].render();
        }
    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(string const& path)
        {
            // read file via ASSIMP
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
            // check for errors
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
                return;
            }
            // retrieve the directory path of the filepath
            directory = path.substr(0, path.find_last_of('/'));

            // process ASSIMP's root node recursively
            processNode(scene->mRootNode, scene);
        }

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene)
        {
            // process each mesh located at the current node
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                Mesh submesh = processMesh(mesh, scene);
                meshes.push_back(submesh);
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }

        }

        void SetVertexBoneDataToDefault(VertexHolder& vertex)
        {
            for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
            {
                vertex.mBoneIDs[i] = -1;
                vertex.mWeights[i] = 0.0f;
            }
        }

        Mesh processMesh(aiMesh* mesh, const aiScene* scene)
        {
            vector<VertexHolder> vertices;
            vector<unsigned int> indices;
            //vector<Texture> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                VertexHolder vertex;
                SetVertexBoneDataToDefault(vertex);
                vertex.mPos = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
                vertex.mNormal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);

                if (mesh->mTextureCoords[0])
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.mTextureCoords = vec;
                }
                else
                    vertex.mTextureCoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            //vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            //vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

            //ExtractBoneWeightForVertices(vertices, mesh, scene);
            
            return Mesh(vertices, indices);

        }

        /*void ExtractBoneWeightForVertices(std::vector<VertexHolder>& vertices, aiMesh* mesh, const aiScene* scene)
        {
            auto& boneInfoMap = mBoneInfoMap;
            int& boneCount = mBoneCounter;

            for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
            {
                int boneID = -1;
                std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
                if (boneInfoMap.find(boneName) == boneInfoMap.end())
                {
                    BoneInfo newBoneInfo;
                    newBoneInfo.id = boneCount;
                    newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
                    boneInfoMap[boneName] = newBoneInfo;
                    boneID = boneCount;
                    boneCount++;
                }
                else
                {
                    boneID = boneInfoMap[boneName].id;
                }
                assert(boneID != -1);
                auto weights = mesh->mBones[boneIndex]->mWeights;
                int numWeights = mesh->mBones[boneIndex]->mNumWeights;

                for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
                {
                    int vertexId = weights[weightIndex].mVertexId;
                    float weight = weights[weightIndex].mWeight;
                    assert(vertexId <= vertices.size());
                    SetVertexBoneData(vertices[vertexId], boneID, weight);
                }
            }
        }*/

    };
}