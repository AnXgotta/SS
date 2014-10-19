// Fill out your copyright notice in the Description page of Project Settings.

#include "SS.h"
#include "SSItem_CubeTrace.h"


ASSItem_CubeTrace::ASSItem_CubeTrace(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	TraceComponent = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("TraceComponent"));
	TraceComponent->BodyInstance.SetCollisionProfileName("Interactable_Trace");
	TraceComponent->SetSimulatePhysics(false);
	TraceComponent->SetEnableGravity(false);
	TraceComponent->AttachParent = StaticMeshComponent;

}


