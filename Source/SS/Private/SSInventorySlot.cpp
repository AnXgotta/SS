

#include "SS.h"
#include "SSInventorySlot.h"


USSInventorySlot::USSInventorySlot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	ItemName = "NULL";
	ItemUIImage = NULL;
	CurrentStackSize = -1;
	MaxStackSize = -1;

	// Not sure if needed
	//ItemInstances = new TArray<ASSItem*>();
}

bool USSInventorySlot::IsEmpty(){
	if (CurrentStackSize == 0) return true;
	return false;
}

bool USSInventorySlot::IsFull(){
	if (CurrentStackSize == MaxStackSize) return true;
	return false;
}

bool USSInventorySlot::AddItem(ASSItem* newItem){
	if (newItem == NULL) return false;
	if (IsFull()) return false;

	if (IsEmpty()){
		ItemName == newItem->GetItemProperties().ItemName;
		ItemUIImage = newItem->GetItemProperties().ItemUIImage;
		MaxStackSize = newItem->GetItemProperties().MaxStackSize;
		CurrentStackSize = 1;
	} else {
		CurrentStackSize++;
	}

	ItemInstances.Add(newItem);

	newItem->SetActorHiddenInGame(true);

	return true;
}

ASSItem* USSInventorySlot::RemoveItem(){
	if (IsEmpty()) return NULL;

	if (IsEmpty()){
		ItemName == "NULL";
		ItemUIImage = NULL;
		MaxStackSize = -1;
		CurrentStackSize = -1;
	} else {
		CurrentStackSize--;
	}

	return ItemInstances.Pop(true);
}


