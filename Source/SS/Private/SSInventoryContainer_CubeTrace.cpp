// Fill out your copyright notice in the Description page of Project Settings.

#include "SS.h"
#include "SSInventoryContainer_CubeTrace.h"


ASSInventoryContainer_CubeTrace::ASSInventoryContainer_CubeTrace(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	TraceComponent = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("TraceComponent"));
	TraceComponent->BodyInstance.SetCollisionProfileName("Interactable_Trace");
	TraceComponent->SetSimulatePhysics(false);
	TraceComponent->SetEnableGravity(false);
	TraceComponent->AttachParent = StaticMeshComponent;

}


///////////////////////////////////////////////////
// Funcitonality

void ASSInventoryContainer_CubeTrace::OnAddedToPlayer(){
	Super::OnAddedToPlayer();
	if (InventoryContainerType != EInventoryContainerEnum::IC_CLOTHES || InventoryContainerType != EInventoryContainerEnum::IC_VEST) return;
	if (TraceComponent){
		TraceComponent->SetSimulatePhysics(false);
		TraceComponent->SetEnableGravity(false);
		TraceComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASSInventoryContainer_CubeTrace::OnRemovedFromPlayer(){
	Super::OnRemovedFromPlayer();
	if (InventoryContainerType != EInventoryContainerEnum::IC_CLOTHES || InventoryContainerType != EInventoryContainerEnum::IC_VEST) return;
	if (TraceComponent){
		TraceComponent->SetSimulatePhysics(true);
		TraceComponent->SetEnableGravity(true);
		TraceComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

