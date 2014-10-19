

#pragma once
#include "SSInteractable.h"
#include "SSConstants.h"

#include "GameFramework/Actor.h"
#include "SSItem.generated.h"

USTRUCT()
struct FPropertiesStruct{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Item)
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = Item)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, Category = Item)
	UTexture2D* ItemUIImage;

	UPROPERTY(EditAnywhere, Category = Item)
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
class SS_API ASSItem : public AStaticMeshActor, public ISSInteractable
{
	GENERATED_UCLASS_BODY()


	///////////////////////////////////////////////////
	//  Item Properties

protected:
	UPROPERTY(EditAnywhere, Category = ItemProperties)
	FPropertiesStruct ItemProperties;


	////////////////////////////////////////////////////
	//  Accessor/Mutator

public:

	FPropertiesStruct GetItemProperties();


	///////////////////////////////////////////////////
	// Funcitonality

	void OnAddedToContainer();

	void OnRemovedFromContainer();


	///////////////////////////////////////////////////
	// Interaction

	virtual void OnRecognized();

	virtual void OnNotRecognized();

	virtual void OnInteract();

};