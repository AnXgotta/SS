

#pragma once
#include "SSInventoryItem.h"

#include "GameFramework/Actor.h"
#include "SSItem.generated.h"

USTRUCT()
struct FPropertiesStruct{

	GENERATED_USTRUCT_BODY()

	FString ItemName;

	FString ItemDescription;

	UTexture2D* ItemUIImage;

	float ItemWeight;

	FPropertiesStruct(){
		ItemName = "NULL";
		ItemDescription = "NULL";
		ItemUIImage = NULL;
		ItemWeight = -1.0f;
	}

};


/**
 * 
 */
UCLASS()
class SS_API ASSItem : public AActor, public ISSInventoryItem
{
	GENERATED_UCLASS_BODY()


	///////////////////////////////////////////////////
	//  Item Properties

protected:

	FPropertiesStruct ItemProperties;


	////////////////////////////////////////////////////
	//  Accessor/Mutator

public:

	FPropertiesStruct GetItemProperties();


	///////////////////////////////////////////////////
	// Funcitonality

	void OnAddedToContainer();

	void OnRemovedFromContainer();

};