// Fill out your copyright notice in the Description page of Project Settings.

#include "SS.h"
#include "SSItem_SphereTrace.h"


ASSItem_SphereTrace::ASSItem_SphereTrace(const class FPostConstructInitializeProperties& PCIP)
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

void ASSItem_SphereTrace::OnAddedToContainer(){
	Super::OnAddedToContainer();
	SetActorHiddenInGame(true);
	TraceComponent->SetSimulatePhysics(false);
	TraceComponent->SetEnableGravity(false);
	TraceComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASSItem_SphereTrace::OnRemovedFromContainer(){
	Super::OnRemovedFromContainer();
	SetActorHiddenInGame(false);
	TraceComponent->SetSimulatePhysics(true);
	TraceComponent->SetEnableGravity(true);
	TraceComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
