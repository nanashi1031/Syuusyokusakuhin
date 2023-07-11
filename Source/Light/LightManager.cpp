#include "LightManager.h"
#include "imgui.h"

LightManager::~LightManager()
{
	Clear();
}

void LightManager::Register(Light* light)
{
	lights.emplace_back(light);
}

void LightManager::Remove(Light* light)
{
	std::vector<Light*>::iterator	it = std::find(lights.begin(), lights.end(), light);
	if (it != lights.end())
	{
		lights.erase(it);
		delete	light;
	}
}

void LightManager::RemoveIndex(int index)
{
	lights.erase(lights.begin()+ (index -1 ));
}

void LightManager::Clear()
{
	lights.clear();
}

void LightManager::PushRenderContext(RenderContext& rc)
{
	// ŠÂ‹«Œõ‚Ìî•ñ‚ð’Ç‰Á
	rc.ambientLightColor = ambientColor;

	// “o˜^‚³‚ê‚Ä‚¢‚éŒõŒ¹‚Ìî•ñ‚ðÝ’è
	for (Light* light : lights)
	{
		light->PushRenderContext(rc);
	}
}

void LightManager::DrawDebugGUI()
{
	float alpha = 0.35f;
	ImGui::SetNextWindowBgAlpha(alpha);
	if (ImGui::TreeNode("Lights"))
	{
		ImGui::ColorEdit3("AmbientColor", &ambientColor.x);
		int nodeId = 0;
		for (Light* light : lights)
		{
			ImGui::PushID(nodeId++);
			light->DrawDebugGUI();
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
}

void LightManager::DrawDebugPrimitive()
{
	for (Light* light : lights)
	{
		light->DrawDebugPrimitive();
	}
}