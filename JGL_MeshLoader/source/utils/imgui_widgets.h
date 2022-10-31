#pragma once

#include "pch.h"

#include "imgui.h"
#include "imgui_internal.h"

namespace nimgui
{
	static void draw_vec3_widget(const std::string& label, glm::vec3& values, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];


		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushFont(boldFont);
		ImGui::Button("X", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Y", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Z", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();


	}

	static void draw_vec4_widget(const std::string& label, glm::vec4& values, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];


		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushFont(boldFont);
		ImGui::Button("R", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##R", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("G", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##G", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("B", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##B", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PushFont(boldFont);
		ImGui::Button("A", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##A", &values.a, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();


	}
}

