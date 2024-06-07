// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SelectionArea.h"
#include "MeshGenerationUI.h"
#include <MeshGenerationUI.h>
#include "ToonShaderPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_5_6_API AToonShaderPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AToonShaderPlayerController();
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Selection")
	ASelectionArea* SelectionAreaInstance;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Selection")
	bool bMove = true;
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateProgressBar(float Progress);
protected:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly,Category = "Selection")
	TSubclassOf<ASelectionArea> SelectionAreaClass;

};
