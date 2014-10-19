

#pragma once
#include "SSItem.h"
#include "SSInteractable.h"

#include "GameFramework/Actor.h"
#include "SSInventoryContainerBase.generated.h"


UENUM(BlueprintType)
namespace EInventoryContainerEnum {
	enum Type {
		IC_CLOTHES UMETA(DisplayName = "Clothes"),
		IC_VEST UMETA(DisplayName = "Belt/Vest"),
		IC_PACK UMETA(DisplayName = "Packs"),
		IC_CRATE UMETA(DisplayName = "Crate"),
		IC_NULL UMETA(DisplayName = "Null")
	};
}

USTRUCT()
struct FContainerSlot {

	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString ItemName;

	UPROPERTY()
		FString ItemDescription;

	UPROPERTY()
		UTexture2D* ItemUIImage;

	UPROPERTY()
		TArray<ASSItem*> Items;

	
	void AddItem(ASSItem* NewItem) {
		Items.Add(NewItem);
	}

	ASSItem* RemoveItem() {
		return Items.Pop();
	}

	bool IsEmpty() {
		return (Items.Num() <= 0);
	}

	// constructor
	FContainerSlot() {
		ItemName = "NULL";
		ItemDescription = "NULL";
		ItemUIImage = NULL;
		Items = TArray<ASSItem*>();
	}
};



UCLASS()
class SS_API ASSInventoryContainerBase : public AStaticMeshActor, public ISSInteractable
{
	GENERATED_UCLASS_BODY()

private:
	// Container Indentifier
	UPROPERTY(EditAnywhere, Category = Enum)
		TEnumAsByte<EInventoryContainerEnum::Type> InventoryContainerType;
	
	UPROPERTY()
		float WeightCapacity;

	UPROPERTY()
		float CurrentWeight;

	UPROPERTY()
		TArray<FContainerSlot> ItemMap;

protected:

	UFUNCTION()
		EInventoryContainerEnum::Type GetInventoryContainerType();

	UFUNCTION()
		void SetInventoryContainerType(EInventoryContainerEnum::Type NewContainerType);

	UFUNCTION()
		float GetWeightCapacity();

	UFUNCTION()
		void SetWeightCapacity(float NewWeightCapacity);

	UFUNCTION()
		void AdjustWeightAdd(ASSItem* AddedItem);

	UFUNCTION()
		void AdjustWeightRemove(ASSItem* RemovedItem);

	UFUNCTION()
		TArray<FContainerSlot> GetItemMap();

	UFUNCTION()
		void AddItemNewContainerSlot(ASSItem* NewItem);

	UFUNCTION()
		bool ContainsItem(FString ItemName);

	UFUNCTION()
		int32 GetItemIndex(FString ItemName);
		

public:
	UFUNCTION()
		bool AddItem(ASSItem* NewItem);

	UFUNCTION()
		ASSItem* RemoveItem(FString RemovedItemName);

	////////////////////////////////////////////////////////////
	// ISSInventoryContainer Methods
	

	
};
