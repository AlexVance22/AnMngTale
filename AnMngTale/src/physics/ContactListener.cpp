#include "PCH.h"
#include "ContactListener.h"

#include "TriggerState.h"


void ContactListener::BeginContact(b2Contact * contact)
{
	TriggerState* ts = nullptr;

	if (contact->GetFixtureA()->GetUserData().pointer != 0)
		ts = (TriggerState*)contact->GetFixtureA()->GetUserData().pointer;
	if (contact->GetFixtureA()->GetUserData().pointer != 0)
		ts = (TriggerState*)contact->GetFixtureA()->GetUserData().pointer;

	if (!ts)
		return;

	ts->inside = true;
}

void ContactListener::EndContact(b2Contact* contact)
{
	TriggerState* ts = nullptr;

	if (contact->GetFixtureA()->GetUserData().pointer != 0)
		ts = (TriggerState*)contact->GetFixtureA()->GetUserData().pointer;
	if (contact->GetFixtureA()->GetUserData().pointer != 0)
		ts = (TriggerState*)contact->GetFixtureA()->GetUserData().pointer;

	if (!ts)
		return;

	if (ts->autoreset)
		ts->used = false;

	ts->inside = false;
}