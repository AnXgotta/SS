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


