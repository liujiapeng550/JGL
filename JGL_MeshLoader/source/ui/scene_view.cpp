#include "pch.h"
#include "scene_view.h"
#include "imgui.h"
#include<filesystem>
namespace nui
{
	void SceneView::loadSkyBox()
	{
		std::string vsname = FileSystem::getPath("JGL_MeshLoader/shaders/buit_in/skybox_vs.shader");
		std::string fsname = FileSystem::getPath("JGL_MeshLoader/shaders/buit_in/skybox_fs.shader");
		mSkyShader = std::make_unique<nshaders::Shader>(vsname.c_str(), fsname.c_str());
		mSkyBox = std::make_shared<nelems::Model>(FileSystem::getPath("JGL_MeshLoader/resource/defaultcube.fbx"));

		vector<std::string> faces
		{
			FileSystem::getPath("JGL_MeshLoader/resource/built_in/textures/skybox/right.jpg"),
			FileSystem::getPath("JGL_MeshLoader/resource/built_in/textures/skybox/left.jpg"),
			FileSystem::getPath("JGL_MeshLoader/resource/built_in/textures/skybox/top.jpg"),
			FileSystem::getPath("JGL_MeshLoader/resource/built_in/textures/skybox/bottom.jpg"),
			FileSystem::getPath("JGL_MeshLoader/resource/built_in/textures/skybox/front.jpg"),
			FileSystem::getPath("JGL_MeshLoader/resource/built_in/textures/skybox/back.jpg")
		};
		mCubemapTexture = TextureSystem::loadCubemap(faces,false);

	}
	void SceneView::loadPlane()
	{
		std::string vsname = FileSystem::getPath("JGL_MeshLoader/shaders/buit_in/base_vs.shader");
		std::string fsname = FileSystem::getPath("JGL_MeshLoader/shaders/buit_in/base_fs.shader");
		mPlaneShader = std::make_unique<nshaders::Shader>(vsname.c_str(), fsname.c_str());
		mPlane = std::make_shared<nelems::Model>(FileSystem::getPath("JGL_MeshLoader/resource/built_in/plane.fbx"));
		mPlaneTexture = TextureSystem::getTextureId(FileSystem::getPath("JGL_MeshLoader/resource/built_in/textures/wood.png").c_str());

	}
	void SceneView::resize(int32_t width, int32_t height)
	{
		mSize.x = width;
		mSize.y = height;

		mFrameBuffer->create_buffers((int32_t)mSize.x, (int32_t)mSize.y);
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
		if (!mModel)
			mModel = std::make_shared<nelems::Model>();
		mModel = std::make_shared<nelems::Model>(filepath);
		mIsSkin = mModel->GetIsSkinInModel();
		if(mIsSkin) {
			//���ض���
			load_shader(FileSystem::getPath("JGL_MeshLoader/resource/Anim.xml"));
			mAnimation = Animation(filepath, mModel.get());
			mAnimator = Animator(&mAnimation);

		}

		//mMesh->load(filepath);
	}

	void SceneView::load_shader(const std::string& filepath)
	{
		//??mMaterial?????????
        if (mMaterial)
            mMaterial = std::make_shared<Material>();
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
		std::filesystem::path pathpath(shadername);
		m_shadername = pathpath.stem().string();
		mShader = std::make_unique<nshaders::Shader>(vsname.c_str(), fsname.c_str());
	}
	void SceneView::renderSkyBox()
	{
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		mSkyShader->use();
		mCamera->setcam(mSkyShader.get());
		mSkyShader.get()->set_i1(GL_TEXTURE0, "skybox");
		mSkyShader.get()->set_texture(GL_TEXTURE0, GL_TEXTURE_CUBE_MAP, mCubemapTexture);

		mSkyBox->Draw();
		glDepthFunc(GL_LESS); // set depth function back to default
	}

	void SceneView::renderPlane()
	{
		mPlaneShader->use();
		mCamera->update(mPlaneShader.get());
		mPlaneShader.get()->set_i1(GL_TEXTURE0, "baseMap");
		mPlaneShader.get()->set_texture(GL_TEXTURE0, GL_TEXTURE_2D, mPlaneTexture);
		mPlane->Draw();
	}

	void SceneView::render()
	{
		mFrameBuffer->bind();
		mShader->use();

		float currentFrame = glfwGetTime();
		mdeltaTime = currentFrame - mlastFrame;
		mlastFrame = currentFrame;
		mAnimator.UpdateAnimation(mdeltaTime);
		//����й�����Ƥ
		if (mIsSkin) {
			auto transforms = mAnimator.GetFinalBoneMatrices();
			for (int i = 0; i < transforms.size(); ++i)
				mShader.get()->set_mat4(transforms[i],"finalBonesMatrices[" + std::to_string(i) + "]");
		}

		if (mModel && mShader)
		{
			mShader.get()->set_f1((float)glfwGetTime(), "time");
			if (m_shadername == "fur") {
				if (mMaterial->isMultyPass()) {
					int passcount = int(mMaterial->getFloatMap()["Pass"]);
					for (int i = 0; i < passcount; i += 1) {
						//shader��������
						mShader.get()->set_f1(i, "PassIndex");
						mMaterial->update_shader_params(mShader.get());
						mModel->Draw();
					}
				}
			}
			else
			{
				mMaterial->update_shader_params(mShader.get());
				mModel->Draw();

			}

		}
		else {
			load_mesh(FileSystem::getPath("JGL_MeshLoader/resource/cube.fbx"));
		}

		ImGui::Begin("Scene");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mSize = { viewportPanelSize.x, viewportPanelSize.y };
		mCamera->set_aspect(mSize.x / mSize.y);
		mCamera->update(mShader.get());

		renderPlane();
		renderSkyBox();

		mFrameBuffer->unbind();


		// add rendered texture to ImGUI scene window
		uint64_t textureID = mFrameBuffer->get_texture();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
	}
}