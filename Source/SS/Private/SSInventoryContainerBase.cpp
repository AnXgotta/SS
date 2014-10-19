

#include "SS.h"
#include "SSInventoryContainerBase.h"



ASSInventoryContainerBase::ASSInventoryContainerBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	StaticMeshComponent->BodyInstance.SetCollisionProfileName("Interactable_Item");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(true);
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);

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
	USSConstants::ScreenMessage("Add Item", 5.0f, FColor::Cyan);
	// new item null
	if (NewItem == NULL) return false;
	USSConstants::ScreenMessage("Check Weight", 5.0f, FColor::Cyan);
	// container full
	if (WeightCapacity < (CurrentWeight + NewItem->GetItemProperties().ItemWeight)) return false;
	USSConstants::ScreenMessage("Check for stacking", 5.0f, FColor::Cyan);
	// an item of this type already exists in the container (stacking)
	FString ItemName = NewItem->GetItemProperties().ItemName;
	int32 ItemIndex = GetItemIndex(ItemName);
	if (ItemIndex > 0){
		USSConstants::ScreenMessage("Stack", 5.0f, FColor::Cyan);
		ItemMap[ItemIndex].AddItem(NewItem);
	}else{
		USSConstants::ScreenMessage("New Item", 5.0f, FColor::Cyan);
		AddItemNewContainerSlot(NewItem);
	}
	USSConstants::ScreenMessage("Adjust weight, other things, return true", 5.0f, FColor::Cyan);
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


///////////////////////////////////////////////////
// Funcitonality

void ASSInventoryContainerBase::OnAddedToPlayer(){
	if (StaticMeshComponent){
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASSInventoryContainerBase::OnRemovedFromPlayer(){
	if (StaticMeshComponent){
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}