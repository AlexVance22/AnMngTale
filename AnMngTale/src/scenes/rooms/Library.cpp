#include "PCH.h"
#include "Library.h"

#include "scenes/floors/Floor300.h"


void Library::impl(const float deltaTime)
{

}


Library::Library() : Scene("library")
{
	LOAD_SCENE("exit", Floor300, 15, -95);
}