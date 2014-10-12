

#pragma once

#include "SSItem.h"

#include "Object.h"
#include "SSInventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class SS_API USSInventorySlot : public UObject
{
	GENERATED_UCLASS_BODY()

	
private:

	FString ItemName;
	UTexture2D* ItemUIImage;

	int32 CurrentStackSize;
	int32 MaxStackSize;

	TArray<ASSItem*> ItemInstances;

	bool IsEmpty();
	bool IsFull();

public:

	bool AddItem(ASSItem* newItem);
	ASSItem* RemoveItem();
	
};
