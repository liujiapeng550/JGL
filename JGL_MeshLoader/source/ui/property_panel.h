#pragma once

#include "elems/light.h"
#include "ui/scene_view.h"

#include "imgui.h"
#include "utils/imgui_widgets.h"
#include <ImFileBrowser.h>

namespace nui
{
  class Property_Panel
  {
  public:

    Property_Panel()
    {
        mCurrentMaterialFile = "< ... >";
        mCurrentMeshFile = "< ... >";

      //mFileDialog.SetTitle("Open mesh");
      //mFileDialog.SetFileFilters({ ".fbx", ".obj" });
    }

    void render(nui::SceneView* mScene);

    void set_mesh_load_callback(const std::function<void(const std::string&)>& callback)
    {
      mMeshLoadCallback = callback;
    }

    void set_shader_load_callback(const std::function<void(const std::string&)>& callback)
    {
        mShaderLoadCallback = callback;
    }

  private:
    // create a file browser instance
    ImGui::FileBrowser mFileDialog;

    std::function<void(const std::string&)> mMeshLoadCallback;
    std::function<void(const std::string&)> mShaderLoadCallback;
    std::string mCurrentMeshFile;
    std::string mCurrentMaterialFile;


  };
}

