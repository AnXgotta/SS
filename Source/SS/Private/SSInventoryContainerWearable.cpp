// Fill out your copyright notice in the Description page of Project Settings.

#include "SS.h"
#include "SSInventoryContainerWearable.h"


ASSInventoryContainerWearable::ASSInventoryContainerWearable(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	StaticMeshComponent->BodyInstance.SetCollisionProfileName("Interactable_OnPawn");
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetEnableGravity(false);
	TraceComponent->SetSimulatePhysics(false);
	TraceComponent->SetEnableGravity(false);
}


