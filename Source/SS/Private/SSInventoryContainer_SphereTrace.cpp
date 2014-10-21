// Fill out your copyright notice in the Description page of Project Settings.

#include "SS.h"
#include "SSInventoryContainer_SphereTrace.h"


ASSInventoryContainer_SphereTrace::ASSInventoryContainer_SphereTrace(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	TraceComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("TraceComponent"));
	TraceComponent->InitSphereRadius(50.0f);
	TraceComponent->BodyInstance.SetCollisionProfileName("Interactable_Trace");
	TraceComponent->SetSimulatePhysics(false);
	TraceComponent->SetEnableGravity(false);
	TraceComponent->AttachParent = StaticMeshComponent;

}


///////////////////////////////////////////////////
// Funcitonality

void ASSInventoryContainer_SphereTrace::OnAddedToPlayer(){
	Super::OnAddedToPlayer();
	if (InventoryContainerType != EInventoryContainerEnum::IC_CLOTHES || InventoryContainerType != EInventoryContainerEnum::IC_VEST) return;
	if (TraceComponent){
		TraceComponent->SetSimulatePhysics(false);
		TraceComponent->SetEnableGravity(false);
		TraceComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASSInventoryContainer_SphereTrace::OnRemovedFromPlayer(){
	Super::OnRemovedFromPlayer();
	if (InventoryContainerType != EInventoryContainerEnum::IC_CLOTHES || InventoryContainerType != EInventoryContainerEnum::IC_VEST) return;
	if (TraceComponent){
		TraceComponent->SetSimulatePhysics(true);
		TraceComponent->SetEnableGravity(true);
		TraceComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}