

#include "SS.h"
#include "SSItem.h"


ASSItem::ASSItem(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	SceneComponent = PCIP.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComponent"));
	RootComponent = SceneComponent;
		
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->BodyInstance.SetCollisionProfileName("Interactable_Item");	
	Mesh->BodyInstance.bAutoWeld = false;
	Mesh->SetSimulatePhysics(true);
	//Mesh->AttachTo(RootComponent);
	Mesh->AttachParent = RootComponent;


	TraceComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("PhysicsCollisionComponent"));
	TraceComponent->InitSphereRadius(10.0f);
	TraceComponent->BodyInstance.SetCollisionProfileName("Interactable_Trace");
	TraceComponent->SetEnableGravity(false);
	TraceComponent->AttachParent = Mesh;
	

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
	USSConstants::ScreenMessage("OnRecognized - Item", 5.0f, FColor::Yellow);
}

void ASSItem::OnNotRecognized(){
	USSConstants::ScreenMessage("OnNotRecognized - Item", 5.0f, FColor::Yellow);
}


void ASSItem::OnInteract(){
	USSConstants::ScreenMessage("OnInteract - Item", 5.0f, FColor::Yellow);
}