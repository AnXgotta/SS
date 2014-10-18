

#include "SS.h"
#include "SSInventoryContainerBase.h"



ASSInventoryContainerBase::ASSInventoryContainerBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	Mesh->AttachParent = RootComponent;

	CurrentWeight = 0.0f;
	WeightCapacity = -1.0f;
	InventoryContainerType = EInventoryContainerEnum::IC_NULL;

}


// Container Type
EInventoryContainerEnum::Type ASSInventoryContainerBase::GetInventoryContainerType(){
	return InventoryContainerType;
}

void ASSInventoryContainerBase::SetInventoryContainerType(EInventoryContainerEnum::Type NewContainerType){
	InventoryContainerType = NewContainerType;
}


// Weight Capacity
float ASSInventoryContainerBase::GetWeightCapacity(){
	return WeightCapacity;
}

void ASSInventoryContainerBase::SetWeightCapacity(float NewWeightCapacity){
	WeightCapacity = NewWeightCapacity;
}

void ASSInventoryContainerBase::AdjustWeightAdd(ASSItem* AddedItem){
	CurrentWeight = FMath::Min(WeightCapacity, (CurrentWeight + AddedItem->GetItemProperties().ItemWeight));
}

void ASSInventoryContainerBase::AdjustWeightRemove(ASSItem* RemovedItem){
	CurrentWeight = FMath::Max(0.0f, (CurrentWeight - RemovedItem->GetItemProperties().ItemWeight));
}


// Item Map
TArray<FContainerSlot> ASSInventoryContainerBase::GetItemMap(){
	return ItemMap;
}

void ASSInventoryContainerBase::AddItemNewContainerSlot(ASSItem* NewItem){
	FContainerSlot NewSlot = FContainerSlot();
	NewSlot.ItemName = NewItem->GetItemProperties().ItemName;
	NewSlot.ItemDescription = NewItem->GetItemProperties().ItemDescription;
	NewSlot.ItemUIImage = NewItem->GetItemProperties().ItemUIImage;
	ItemMap.Add(NewSlot);
}

bool ASSInventoryContainerBase::ContainsItem(FString ItemName){
	if (ItemName == NULL) return false;

	for (FContainerSlot slot : ItemMap){
		if (ItemName.Equals(slot.ItemName)){
			return true;
		}
	}
	return false;
}

int32 ASSInventoryContainerBase::GetItemIndex(FString ItemName){
	if (ItemName == NULL) return false;
	int32 index = 0;
	for (FContainerSlot slot : ItemMap){
		if (ItemName.Equals(slot.ItemName)){
			return index;
		}
		index++;
	}
	return -1;
}

bool ASSInventoryContainerBase::AddItem(ASSItem* NewItem){
	// new item null
	if (NewItem == NULL) return false;
	// container full
	if (WeightCapacity < (CurrentWeight + NewItem->GetItemProperties().ItemWeight)) return false;
	// an item of this type already exists in the container (stacking)
	FString ItemName = NewItem->GetItemProperties().ItemName;
	int32 ItemIndex = GetItemIndex(ItemName);
	if (ItemIndex > 0){
		ItemMap[ItemIndex].AddItem(NewItem);
	}else{
		AddItemNewContainerSlot(NewItem);
	}
		
	// item added, do the things
	AdjustWeightAdd(NewItem);
	NewItem->OnAddedToContainer();
	return true;
}

ASSItem* ASSInventoryContainerBase::RemoveItem(FString RemoveItemName){
	// new item null
	if (RemoveItemName == NULL) return NULL;
	// temp pointer to removed item
	ASSItem* RemovedItem = NULL;
	// check if item in container (would be odd if not...)
	int32 ItemIndex = GetItemIndex(RemoveItemName);
	if (ItemIndex > 0){
		RemovedItem = ItemMap[ItemIndex].RemoveItem();
		// if slot is empty, remove the slot
		if (ItemMap[ItemIndex].IsEmpty()){
			ItemMap.RemoveAt(ItemIndex);
		}
	}
	
	// item not found (would be odd if not...)
	if (RemovedItem == NULL) return NULL;
	// item found
	AdjustWeightRemove(RemovedItem);
	RemovedItem->OnRemovedFromContainer();
	return RemovedItem;
}