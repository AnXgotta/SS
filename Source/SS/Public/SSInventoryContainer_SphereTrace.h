// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SSInventoryContainerBase.h"
#include "SSInventoryContainer_SphereTrace.generated.h"

/**
 * 
 */
UCLASS()
class SS_API ASSInventoryContainer_SphereTrace : public ASSInventoryContainerBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Item)
	TSubobjectPtr<class USphereComponent> TraceComponent;








public:
	
	void OnAddedToPlayer() override;

	void OnRemovedFromPlayer() override;


};
