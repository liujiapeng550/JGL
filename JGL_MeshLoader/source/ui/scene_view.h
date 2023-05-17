#pragma once

#include "elems/camera.h"
#include "elems/mesh.h"
#include "elems/model.h"
#include "elems/light.h"
#include "shader/shader_util.h"
#include "render/opengl_buffer_manager.h"
#include "elems/input.h"
#include "elems/material.h"
#include "elems/animation.h"
#include "elems/animator.h"

namespace nui
{
  class SceneView
  {
  public:
    SceneView() : 
      mCamera(nullptr), mFrameBuffer(nullptr), mShader(nullptr),
      mLight(nullptr), mSize(800, 600),mMaterial()
    {
      mFrameBuffer = std::make_unique<nrender::OpenGL_FrameBuffer>();
      mFrameBuffer->create_buffers(800, 600);
     // mShader = std::make_unique<nshaders::Shader>("shaders/pbr_vs.shader", "shaders/pbr_fs.shader");
      mLight = std::make_unique<nelems::Light>();
      mCamera = std::make_unique<nelems::Camera>(glm::vec3(0, 0, 3), 45.0f, 1.3f, 0.1f, 1000.0f);
      mMaterial = std::make_unique<Material>();
      load_shader(FileSystem::getPath("JGL_MeshLoader/resource/PBR.xml"));
      loadSkyBox();
      loadPlane();

    }
    void loadSkyBox();

    void loadPlane();

    ~SceneView()
    {
      mShader->unload();
    }
    nelems::Light* get_light() { return mLight.get(); }
    void resize(int32_t width, int32_t height);
    void render();
    void load_mesh(const std::string& filepath);
    void load_shader(const std::string& filepath);
    void set_model(std::shared_ptr<nelems::Model> model)
    {
        mModel = model;
    }

    std::shared_ptr<nelems::Model> get_model() { return mModel; }
    std::shared_ptr<Material> get_Material() { return mMaterial; }
    void renderSkyBox();
    void renderPlane();
    void on_mouse_move(double x, double y, nelems::EInputButton button);

    void on_mouse_wheel(double delta);

    void reset_view()
    {
      mCamera->reset();
    }
    void set_plane_show(bool isShow) { m_showPlane = isShow; };
  private:
    std::unique_ptr<nelems::Camera> mCamera;
    std::unique_ptr<nrender::OpenGL_FrameBuffer> mFrameBuffer;
    std::unique_ptr<nshaders::Shader> mShader;
    std::unique_ptr<nelems::Light> mLight;
    std::shared_ptr<nelems::Model> mModel;
    bool mIsSkin=false;
    glm::vec2 mSize;
    std::string m_shadername;
    std::shared_ptr<Material> mMaterial;
    Animation mAnimation;
    Animator mAnimator;
    std::unique_ptr<nshaders::Shader> mSkyShader;
   std::shared_ptr<nelems::Model> mSkyBox;
   std::unique_ptr<nshaders::Shader> mPlaneShader;
   std::shared_ptr<nelems::Model> mPlane;
   //
   bool m_showPlane;

    // timing
    float mdeltaTime = 0.0f;
    float mlastFrame = 0.0f;
    unsigned int mCubemapTexture;
    unsigned int mPlaneTexture;
  };
}

