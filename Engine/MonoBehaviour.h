#pragma once
#include "Component.h"

// Component »ó¼Ó
class MonoBehaviour : public Component
{
public:
	MonoBehaviour();
	virtual ~MonoBehaviour();

private:
	virtual void FinalUpdate() sealed { }

};

