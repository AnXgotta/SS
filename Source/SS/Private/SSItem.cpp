

#include "SS.h"
#include "SSItem.h"


ASSItem::ASSItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	StaticMeshComponent->BodyInstance.SetCollisionProfileName("Interactable_Item");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(true);

	bReplicates = true;
	bReplicateMovement = true;
}





////////////////////////////////////////////////////
//  Accessor/Mutator

FPropertiesStruct ASSItem::GetItemProperties(){
	return ItemProperties;
}


///////////////////////////////////////////////////
// Funcitonality

void ASSItem::OnAddedToContainer(){
	SetActorHiddenInGame(true);
}

void ASSItem::OnRemovedFromContainer(){
	SetActorHiddenInGame(false);
}


void ASSItem::OnRecognized(){
	FString Message = FString::Printf(TEXT("OnRecognized - %s"), *ItemProperties.ItemName);
	USSConstants::ScreenMessage(Message, 5.0f, FColor::Yellow);
}

void ASSItem::OnNotRecognized(){
	FString Message = FString::Printf(TEXT("OnNotRecognized - %s"), *ItemProperties.ItemName);
	USSConstants::ScreenMessage(Message, 5.0f, FColor::Yellow);
}


void ASSItem::OnInteract(){
	FString Message = FString::Printf(TEXT("OnInteract - %s"), *ItemProperties.ItemName);
	USSConstants::ScreenMessage(Message, 5.0f, FColor::Yellow);
}