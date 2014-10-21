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
		//TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

		// Ignore Actors
		//TraceParams.AddIgnoredActor(ActorToIgnore);

		// Re-initialize hit info
		HitOut = FHitResult(ForceInit);

		// Get World Context
		TObjectIterator< APlayerController > ThePC;
		if (!ThePC) return false;
		
		DrawDebugLine(ThePC->GetWorld(), Start, End, FColor::Green, false, -1.f, 0, 2.0f);

		// Trace
		ThePC->GetWorld()->LineTraceSingle(HitOut, Start, End, CollisionChannel, TraceParams);

		//Hit any Actor?
		return (HitOut.GetActor() != NULL);
	}

	template <typename ObjectType>
	static FORCEINLINE ObjectType* SpawnBlueprintActor(
		UWorld* TheWorld,
		UClass* TheBP,
		const FVector& Loc,
		const FRotator& Rot,
		const bool bNoCollisionFail = true,
		AActor* Owner = NULL,
		APawn* Instigator = NULL
		){
		if (!TheWorld) return NULL;
		if (!TheBP) return NULL;


		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoCollisionFail = bNoCollisionFail;
		SpawnInfo.Owner = Owner;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.bDeferConstruction = false;

		return TheWorld->SpawnActor<ObjectType>(TheBP, Loc, Rot, SpawnInfo);
	}


	static FORCEINLINE void ScreenMessage(const FString Message, float Time, FColor Color){
		GEngine->AddOnScreenDebugMessage(-1, Time, Color, Message);
	}





};
