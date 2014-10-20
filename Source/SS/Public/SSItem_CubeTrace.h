// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SSItem.h"
#include "SSItem_CubeTrace.generated.h"

/**
 * 
 */
UCLASS()
class SS_API ASSItem_CubeTrace : public ASSItem
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TSubobjectPtr<class UBoxComponent> TraceComponent;
	

	///////////////////////////////////////////////////
	// Funcitonality

	void OnAddedToContainer();

	void OnRemovedFromContainer();

};
