#include "pch.h"
#include "property_panel.h"

namespace nui
{
  void Property_Panel::render(nui::SceneView* scene_view)
  {
    auto mesh = scene_view->get_mesh();

    ImGui::Begin("Properties");
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
      
      if (ImGui::Button("Open..."))
      {
        mFileDialog.SetTitle("Mesh");
        mFileDialog.SetFileFilters({ ".fbx", ".obj" });
        mFileDialog.Open();
      }
      ImGui::SameLine(0, 5.0f);
      ImGui::Text(mCurrentFile.c_str());
    }

    if (ImGui::CollapsingHeader("Shader", ImGuiTreeNodeFlags_DefaultOpen))
    {

        if (ImGui::Button("Openshader..."))
        {
            mFileDialog.SetTitle("Shader");
            mFileDialog.SetFileFilters({ ".shader"});
            mFileDialog.Open();
        }
        ImGui::SameLine(0, 5.0f);
        ImGui::Text(mCurrentFile.c_str());
    }

    if (ImGui::CollapsingHeader("Material") && mesh)
    {
      ImGui::ColorPicker3("Color", (float*)&mesh->mColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB); 
      ImGui::SliderFloat("Roughness", &mesh->mRoughness, 0.0f, 1.0f);
      ImGui::SliderFloat("Metallic", &mesh->mMetallic, 0.0f, 1.0f);
      static char basetexture[128] = "E:\\GitHubDemo\\jgl_demos\\JGL_MeshLoader\\resource\\textures\\weather\\color.png";
      if (ImGui::InputText("BaseTexure", &basetexture[0], (int)(sizeof(basetexture) / sizeof(*(basetexture))))) 
      {
          mesh->mbaseTexture_id = TextureSystem::getTextureId(basetexture);
      }
    }

    if (ImGui::CollapsingHeader("Light"))
    {

      ImGui::Separator();
      ImGui::Text("Position");
      ImGui::Separator();
      nimgui::draw_vec3_widget("Position", scene_view->get_light()->mPosition, 80.0f);
    }

    ImGui::End();

    mFileDialog.Display();
    if (mFileDialog.HasSelected())
    {
      auto file_path = mFileDialog.GetSelected().string();
      mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);
      std::string title = mFileDialog.get_FileBrower_title();
      if (title == "Shader") {
         mShaderLoadCallback(file_path);
      }else if(title == "Mesh")
        mMeshLoadCallback(file_path);

      mFileDialog.ClearSelected();
    }

  }
}
