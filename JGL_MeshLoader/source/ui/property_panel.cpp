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
        //mFileDialog.SetFileFilters({ ".fbx", ".obj" });
        mFileDialog.Open();
      }
      ImGui::SameLine(0, 5.0f);
      ImGui::Text(mCurrentMeshFile.c_str());
    }

    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    {

        if (ImGui::Button("OpenMaterial..."))
        {
            mFileDialog.SetTitle("Material");
            mFileDialog.SetFileFilters({ ".xml"});
            mFileDialog.Open();
        }
        ImGui::SameLine(0, 5.0f);
        ImGui::Text(mCurrentMaterialFile.c_str());
    }

    if (ImGui::CollapsingHeader("Material") && mesh)
    {
       auto material = scene_view->get_Material();
       map<string, float> floatTypeMap;
       for (auto& it: material->getFloatMap())
       {
           float tmp;
           tmp = it.second;
           ImGui::SliderFloat(it.first.c_str(), (float*)&tmp, -100.0f, 100.0f);
           it.second = tmp;
       }
       for (auto& it : material->getFloat3Map())
       {
           glm::vec3 tmp;
           tmp = it.second;
           ImGui::SliderFloat3(it.first.c_str(), (float*)&tmp, -100.0f, 100.0f);
           it.second = tmp;
       }
       for (auto& it : material->getTextureMap())
       {
           static char tmptexture[128];
           strcpy(tmptexture, it.second.second.c_str());
           if (ImGui::InputText(it.first.c_str(), &tmptexture[0], (int)(sizeof(tmptexture) / sizeof(*(tmptexture)))))
           {
           }
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
      std::string title = mFileDialog.get_FileBrower_title();
      if (title == "Material") {
         mShaderLoadCallback(file_path);
         mCurrentMaterialFile = file_path.substr(file_path.find_last_of("/\\") + 1);
      }else if(title == "Mesh")
        mMeshLoadCallback(file_path);
        mCurrentMeshFile = file_path.substr(file_path.find_last_of("/\\") + 1);
      mFileDialog.ClearSelected();
    }

  }
}
