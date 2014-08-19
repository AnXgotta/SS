

#pragma once

#include "GameFramework/PlayerController.h"
#include "SSPlayerController.generated.h"

USTRUCT()
struct FInventorySlotStruct{

	GENERATED_USTRUCT_BODY()

	FString ItemName;

	UTexture2D* ItemUIImage;

	int32 MaxStackSize;

	int32 CurrentStackSize;

	TArray<int32> Items;

	

	FInventorySlotStruct(){
		ItemName = "NULL";
		ItemUIImage = NULL;
		MaxStackSize = -1;
		CurrentStackSize = -1;
		Items.Empty();
	}

};


/**
 * 
 */
UCLASS()
class SS_API ASSPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

	
	
};
