#include "BeamBody.h"

BeamBody::BeamBody(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::BEAM_BODY;
}

BeamBody::~BeamBody()
{
	
}
