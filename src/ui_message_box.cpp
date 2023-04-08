#include "ui_message_box.h"

MessageBox messageBox;

void MessageBox::OpenPopup(std::function<void()> f)
{
	callback = std::move(f);
	requestOpen = true;
}

void MessageBox::Draw()
{
	if (requestOpen) {
		requestOpen = false;
		ImGui::OpenPopup(title.c_str());
	}
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	bool open = true;
	if (ImGui::BeginPopupModal(title.c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (error != "")
		{
			ImGui::TextWrapped(message.c_str());
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::InputTextMultiline("##err", &error, { 500, 300 });
		}
		else
		{
			ImGui::BeginChild("ch", { 300, 50 });
			ImGui::TextWrapped(message.c_str());
			ImGui::EndChild();
		}
		ImGui::Spacing();
		ImGui::Spacing();

		int n = (bool)(buttons & OK) + (bool)(buttons & Cancel) + (bool)(buttons & Yes) + (bool)(buttons & No);
		float w = (n * 80) + (n - 1) * ImGui::GetStyle().ItemSpacing.x;
		float x = (ImGui::GetContentRegionAvail().x - w) / 2.f;
		float y = ImGui::GetCursorPosY();
		if (buttons & OK)
		{
			if (ImGui::IsWindowAppearing())
				ImGui::SetKeyboardFocusHere();
			ImGui::SetCursorPos({ x, y });
			if (ImGui::Button("OK", { 80, 40 }) ||
				(ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter)))
			{
				ImGui::CloseCurrentPopup();
				if (callback) callback();
			}
			x += 80 + ImGui::GetStyle().ItemSpacing.x;
		}
		if (buttons & Yes)
		{
			ImGui::SetCursorPos({ x, y });
			if (ImGui::Button("Yes", { 80, 40 })) {
				ImGui::CloseCurrentPopup();
				if (callback) callback();
			}
			x += 80 + ImGui::GetStyle().ItemSpacing.x;
		}
		if (buttons & No)
		{
			ImGui::SetCursorPos({ x, y });
			if (ImGui::Button("No", { 80, 40 })) {
				ImGui::CloseCurrentPopup();
			}
			x += 80 + ImGui::GetStyle().ItemSpacing.x;
		}
		if (buttons & Cancel)
		{
			ImGui::SetCursorPos({ x, y });
			if (ImGui::Button("Cancel", { 80, 40 }) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
				ImGui::CloseCurrentPopup();
			}
			x += 80 + ImGui::GetStyle().ItemSpacing.x;
		}

		ImGui::EndPopup();
	}
}