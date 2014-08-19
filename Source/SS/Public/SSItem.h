

#pragma once

#include "GameFramework/Actor.h"
#include "SSItem.generated.h"

USTRUCT()
struct FPropertiesStruct{

	GENERATED_USTRUCT_BODY()

	FString ItemName;

	UTexture2D* ItemUIImage;

	int32 MaxStackSize;

	FPropertiesStruct(){
		ItemName = "NULL";
		ItemUIImage = NULL;
		MaxStackSize = -1;
	}

};


/**
 * 
 */
UCLASS()
class SS_API ASSItem : public AActor
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


	
	
};
