

#include "SS.h"
#include "SSTestInventoryContainer_Clothes.h"


ASSTestInventoryContainer_Clothes::ASSTestInventoryContainer_Clothes(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SetInventoryContainerType(EInventoryContainerEnum::IC_CLOTHES);
	SetWeightCapacity(25.0f);
}


