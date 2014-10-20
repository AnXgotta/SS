// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SSItem.h"
#include "SSItem_SphereTrace.generated.h"

/**
 * 
 */
UCLASS()
class SS_API ASSItem_SphereTrace : public ASSItem
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TSubobjectPtr<class USphereComponent> TraceComponent;
	


	///////////////////////////////////////////////////
	// Funcitonality

	void OnAddedToContainer();

	void OnRemovedFromContainer();

};
