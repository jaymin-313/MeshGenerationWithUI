// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Camera/CameraComponent.h"
#include "SelectionBox.h"
#include "ArchMeshActor.h"
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

	FVector HitLocation;
	bool bSlateVisible = false;
	AArchMeshActor* MeshActor;
private:
	void SwitchToIsometricView();
	void SwitchToOrthographicView();
	void SwitchToPerspectiveView();
	APawn* IsometricPawn;
	APawn* OrthographicPawn;
	APawn* PerspectivePawn;
	APawn* CurrentPawn;

	void MoveCameraToLocation(FVector Location);


};
