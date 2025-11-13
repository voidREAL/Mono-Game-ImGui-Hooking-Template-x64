#include "../include/hackrender.h"

HackRender hackRender;

void HackRender::render()
{
	//Flags để cấu hình cho window
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowSize(ImVec2(500, 400));
	ImGui::Begin("##Window", 0, windowFlags);
	{
		ImGui::BeginTabBar("##Main feature");
		if (ImGui::BeginTabItem("Visual"))
		{
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Aimbot"))
		{
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Misc"))
		{
			hackRender.miscRender();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Settings"))
		{
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Debug"))
		{
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void HackRender::miscRender()
{
	ImGui::BeginGroup();
	{
		ImGui::Checkbox("Bunny Hop", &state.misc.isBunnyHopOn);
	}
	ImGui::EndGroup();
}
