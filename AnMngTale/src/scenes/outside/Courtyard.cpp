#include "PCH.h"
#include "Courtyard.h"

#include "gui/gui.h"

#include "scenes/floors/Floor300.h"
#include "scenes/outside/Basketball.h"
#include "scenes/outside/AulaYard.h"


void Courtyard::loadFlags()
{

}

void Courtyard::dumpFlags()
{
	rapidjson::Document doc;
	loadjson(doc, "config/saves/save0/scenes/courtyard.json");

	switch (m_state)
	{
	case AreaState::INTRO: {
		auto& constr = doc["camera"]["constraints"];
		constr[0][0].SetInt(-1785);
		constr[0][1].SetInt(80);
		constr[1][0].SetInt(2086);
		constr[1][1].SetInt(3240);

		auto& boxes = doc["physics"]["statics"]["boxes"];
		boxes.Erase(&boxes[5]);
		boxes.Erase(&boxes[6]);

		doc["state"].SetInt(doc["state"].GetInt() + 1);
		break; }
	}

	dumpjson(doc, "config/saves/save0/scenes/courtyard.json");
}


void Courtyard::impl(const float deltaTime)
{
	switch (m_state)
	{
	case AreaState::INTRO:
		//if (seq->getSignal() == 0)
			//menus.push(new NameSelect(&menus));
		break;
	case AreaState::DEFAULT:
		break;
	}
}


Courtyard::Courtyard() : Scene("courtyard")
{
	m_triggers["entrance"].onCollide.bind(&Scene::loadScene<Floor300>, this);
	m_triggers["basketball"].onCollide.bind(&Scene::loadScene<Basketball>, this);
	m_triggers["aulayard"].onCollide.bind(&Scene::loadScene<AulaYard>, this);

	//{ 312, 200 }, { 240, 135 }
	//{ 390, 202 }, { 240, 135 }
	//{ 200, 50 }, { 200, 50 }
}