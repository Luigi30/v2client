#include "HudManager.h"
#include "Hud/Elements/Elements.h"
#include "Hud/HudTargeting.h"

typedef std::vector<HudElement *>::iterator HudElementIterator;

HudManager::HudManager(void)
{
}

HudManager::~HudManager(void)
{
}

bool HudManager::AddElement(HudElement *el)
{
	assert(FindElementByName(el->name) == NULL);
	hudElements.push_back(el);

	return true;
}

bool HudManager::RemoveElement(HudElement *el)
{
	// todo
	return false;
}

HudElement * HudManager::FindElementByName(std::string name)
{
	HudElementIterator it = hudElements.begin();

	for (it; it != hudElements.end(); ++it) {
		if((*it)->name == name) return *it;
	}

	return NULL;
}

void HudManager::DrawHud()
{
	HudElementIterator it = hudElements.begin();
	for (it; it != hudElements.end(); ++it) {
		(*it)->Draw();
	}

}

void HudManager::CreateHud()
{
	// TODO: Define the HUD stuff in a config file?
	POINT p;
	p.x = 40;
	p.y = 500;
	HudText *hudText = new HudText("target_name", p, bmFontMonogram16);
	HudTextValue *value = new HudTextValue;
	value->text = "TGT: NONE";
	hudText->value = value;
	g_HudManager.AddElement(hudText);

}

void HudManager::UpdateHud()
{
	// Targeting info.
	if(playerSimobj->current_target != NULL)
	{
		HudText *hud_text = (HudText *)g_HudManager.FindElementByName("target_name");
		hud_text->value->text = "TGT: " + playerSimobj->current_target->name;

		HudTargeting::DrawTargetBrackets(playerSimobj->current_target);

	}
	else
	{
		HudText *hud_text = (HudText *)g_HudManager.FindElementByName("target_name");
		hud_text->value->text = "no target";
	}


}

HudManager g_HudManager;