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
               mesh->mbaseTexture_id = TextureSystem::getTextureId(tmptexture);
           }
       }

    //  ImGui::ColorPicker3("Color", (float*)&mesh->mColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB); 
    //  ImGui::SliderFloat("Roughness", &mesh->mRoughness, 0.0f, 1.0f);
    //  //ImGui::SliderFloat("Metallic", &mesh->mMetallic, 0.0f, 1.0f);
    //  //ImGui::SliderFloat("_RippleSize", &mesh->mRippleSize, 0.0f, 20.0f);
    //  //mesh->mBaseTexture = "/resource/textures/weather/color.png";

    //  static char basetexture[128];
    //  strcpy(basetexture, mesh->mBaseTexture.c_str());
    //  if (ImGui::InputText("BaseTexure", &basetexture[0], (int)(sizeof(basetexture) / sizeof(*(basetexture))))) 
    //  {
    //      mesh->mbaseTexture_id = TextureSystem::getTextureId(basetexture);
    //  }
    //  strcpy(basetexture, mesh->mbumpTexture.c_str());
    //  if (ImGui::InputText("waterbumpMap", &basetexture[0], (int)(sizeof(basetexture) / sizeof(*(basetexture)))))
    //  {
    //      mesh->mbumpTexture_id = TextureSystem::getTextureId(basetexture);
    //  }

    //  nimgui::draw_vec3_widget("RiverParam01", mesh->mRiverParame01, 80.0f);
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
