// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SSInventoryContainerBase.h"
#include "SSInventoryContainer_CubeTrace.generated.h"

/**
 * 
 */
UCLASS()
class SS_API ASSInventoryContainer_CubeTrace : public ASSInventoryContainerBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TSubobjectPtr<class UBoxComponent> TraceComponent;
	



public:

	void OnAddedToPlayer() override;

	void OnRemovedFromPlayer() override;

};
