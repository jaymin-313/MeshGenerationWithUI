// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyWallWidget.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "Camera/CameraComponent.h"
#include "SelectionBox.h"
#include "ArchMeshActor.h"
#include "WallSpline.h"
#include "InteractiveArchController.generated.h"
/**
 * 
 */
UCLASS()
class ASSIGNMENT4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	void OnLeftMouseClick();
	void SpawnMesh(const FMeshData& MeshData);
	void ApplyMaterial(const FMaterialData& MaterialData);
	void ApplyTexture(const FTextureData& TextureData);
	void ToggleUI();
public:
	//AInteractiveArchController();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<USelectionBox> SelectionBoxClass;

	UPROPERTY()
	USelectionBox* SelectionBoxInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UMyWallWidget> WallWidgetClass;
	UPROPERTY()
	UMyWallWidget* WallWidgetInstance;
	UFUNCTION(BlueprintCallable)
	void Switch();
	FVector HitLocation;
	bool bSlateVisible = false;
	AArchMeshActor* MeshActor;
	bool bIsWallBuilder = false;

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* SubSystem;



	UPROPERTY()
	ULocalPlayer* LocalPlayer;

	UPROPERTY()
	UInputMappingContext* MyMapping;
	
	UPROPERTY()
	UInputMappingContext* CameraMapping;

	UPROPERTY()
	class UInputMappingContext* SplineMappingContext;

	/** Action to update location. */
	UPROPERTY()
	class UInputAction* LeftClickAction;

	UPROPERTY()
	class UInputAction* RightClickAction;

	UPROPERTY()
	class UInputAction* DeleteAction;

	UPROPERTY()
	class UInputAction* ClearAction;

	UPROPERTY()
	class UInputAction* UndoAction;

	//FShowTextDelegate ShowText;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowWallWidget(const FString& NewText);
private:
	void SwitchToIsometricView();
	void SwitchToOrthographicView();
	void SwitchToPerspectiveView();
	void SwitchCameraType();
	APawn* IsometricPawn;
	APawn* OrthographicPawn;
	APawn* PerspectivePawn;
	APawn* CurrentPawn;


	void OnLeftMouseButtonPressed();

	void OnRightMouseButtonPressed();

	void UndoWall();

	void DeleteWall();

	void ClearWalls();
	UPROPERTY()
	TArray<AWallSpline*> WallSplineActor;


};
