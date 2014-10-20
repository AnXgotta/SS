

#include "SS.h"
#include "SSItem.h"


ASSItem::ASSItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	StaticMeshComponent->BodyInstance.SetCollisionProfileName("Interactable_Item");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(true);
	StaticMeshComponent->SetMobility(EComponentMobility::Movable);

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
	USSConstants::ScreenMessage("Item On Added To Container", 5.0f, FColor::Cyan);
	SetActorHiddenInGame(true);
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetEnableGravity(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASSItem::OnRemovedFromContainer(){
	SetActorHiddenInGame(false);
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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