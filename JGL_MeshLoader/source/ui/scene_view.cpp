#include "pch.h"
#include "scene_view.h"

#include "imgui.h"
#include "utils/filesystem.h"
namespace nui
{
  void SceneView::resize(int32_t width, int32_t height)
  {
    mSize.x = width;
    mSize.y = height;

    mFrameBuffer->create_buffers((int32_t)mSize.x, (int32_t) mSize.y);
  }

  void SceneView::on_mouse_move(double x, double y, nelems::EInputButton button)
  {
    mCamera->on_mouse_move(x, y, button);
  }

  void SceneView::on_mouse_wheel(double delta)
  {
    mCamera->on_mouse_wheel(delta);
  }

  void SceneView::load_mesh(const std::string& filepath)
  {
    if(!mMesh)
      mMesh = std::make_shared<nelems::Mesh>();
    
    mMesh->load(filepath);
    
  }

  void SceneView::load_shader(const std::string& filepath)
  {
      mMaterial->load(filepath.c_str());
      std::string shadername;
      shadername = mMaterial->getshaderPath();
      size_t found = shadername.find_last_of('_');
      if (found != std::string::npos)
      {
          shadername = shadername.substr(0, found);
      }
      std::string vsname = shadername + "_vs.shader";
      std::string fsname = shadername + "_fs.shader";
      size_t shaderName_index = shadername.find_last_of('\\');
      m_shadername ="fur";
      mShader = std::make_unique<nshaders::Shader>(vsname.c_str(), fsname.c_str());
  }

  void SceneView::renderFur()
  {
      for (int i = 0; i < 10; i += 1) {

          mMesh->updateFur(mShader.get(),i);
          mMesh->render();
      }
  }
  void render2()
  {

  }

  void SceneView::render()
  {
    mShader->use();
    mShader->set_i1(0, "baseMap");
    mShader->set_i1(1, "waterbumpMap");
    mShader->set_i1(2, "RippleTexture");
    mShader->set_i1(3, "RippleTexture");

    mLight->update(mShader.get());

    mFrameBuffer->bind();

    if (mMesh)
    {
        if (m_shadername == "fur") {
            //mMesh->update(mShader.get());
            //renderFur();
           // if (mMultipass) {

                for (int i = 0; i < 10; i += 1) {
                    mMaterial->set_param("FurLength", "float", to_string(i));
                    mMaterial->update_shader_params(mShader.get());
                }
           // }
        }
        else
        {
            mMesh->update(mShader.get());
            mMesh->render();
        }
    }
    else {
        load_mesh(FileSystem::getPath("JGL_MeshLoader/resource/plane.fbx"));
    }

    mFrameBuffer->unbind();

    ImGui::Begin("Scene");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    mSize = { viewportPanelSize.x, viewportPanelSize.y };

    mCamera->set_aspect(mSize.x / mSize.y);
    mCamera->update(mShader.get());

    // add rendered texture to ImGUI scene window
    uint64_t textureID = mFrameBuffer->get_texture();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
  }
}
