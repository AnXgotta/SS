

#pragma once
#include "SSInteractable.generated.h"

/**
*
*/
UINTERFACE(MinimalAPI)
class USSInteractable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class ISSInteractable{

	GENERATED_IINTERFACE_BODY()

public:

	virtual void OnRecognized();

	virtual void OnNotRecognized();

	virtual void OnInteract();


};
