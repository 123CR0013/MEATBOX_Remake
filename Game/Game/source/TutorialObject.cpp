#include "TutorialObject.h"

TutorialObject::TutorialObject(ModeBase* mode, int id) :GameObject(mode)
{
	_objectType = TYPE::TUTORIAL;

	_id = id;
}

TutorialObject::~TutorialObject()
{

}
