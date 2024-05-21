// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstPersonPawn.h"
#include "DynamicPawnController.generated.h"

class UPawnAttributeAsset;
class AFirstPersonPawn;


UCLASS()
class ASSIGNMENT_1_2_API ADynamicPawnController : public APlayerController
{
	GENERATED_BODY()
public:
	ADynamicPawnController();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
	void DisplayAttributes(UPawnAttributeAsset* PawnAttributes);
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void ToggleSpawnPawn();

	UPROPERTY(EditAnywhere)
	class UDataTable* PawnTable;

	FPawnData* CurrentPawn;




private:
	TArray<FPawnData> PawnDataList;
	int32 CurrentPawnIndex;
	AFirstPersonPawn* SpawnedPawn;
	int32 CurrentRowIndex;
	void SpawnPawn();
	void DestroySpawnedPawn();
	APawn* SP = nullptr;
	
};
