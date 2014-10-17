// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SSConstants.generated.h"




/**
 * 
 */
UCLASS()
class SS_API USSConstants : public UObject
{

	GENERATED_UCLASS_BODY()


	static FORCEINLINE bool TraceInteractable(AActor* ActorToIgnore, const FVector& Start, const FVector& End, FHitResult& HitOut, ECollisionChannel CollisionChannel = ECollisionChannel::ECC_GameTraceChannel2, bool ReturnPhysMat = false) {
		FCollisionQueryParams TraceParams(FName(TEXT("Trace Interactable")), true, ActorToIgnore);
		TraceParams.bTraceComplex = true;
		//TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

		// Ignore Actors
		TraceParams.AddIgnoredActor(ActorToIgnore);

		// Re-initialize hit info
		HitOut = FHitResult(ForceInit);

		// Get World Context
		TObjectIterator< APlayerController > ThePC;
		if (!ThePC) return false;

		// Trace
		ThePC->GetWorld()->LineTraceSingle(HitOut, Start, End, CollisionChannel, TraceParams);

		//Hit any Actor?
		return (HitOut.GetActor() != NULL);
	}


	static FORCEINLINE void ScreenMessage(const FString Message, float Time, FColor Color){
		GEngine->AddOnScreenDebugMessage(-1, Time, Color, Message);
	}


};
