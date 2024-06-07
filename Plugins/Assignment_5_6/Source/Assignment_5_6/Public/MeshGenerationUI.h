// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/EditableTextBox.h>
#include "MeshGenerationUI.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_5_6_API UMeshGenerationUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ShapeComboBox;

	UPROPERTY(meta = (BindWidget))
	class USpinBox* RadiusDimensionsBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* GenerateMeshesButton;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MeshGenerationProgressBar;

	UPROPERTY(meta = (BindWidget))
	class USpinBox* NumInstancesBox;

	UFUNCTION()
	void OnGenerateMeshesClicked();
protected:
	virtual void NativeConstruct() override;

};
