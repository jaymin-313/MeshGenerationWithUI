// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "IsometricCameraPawn.generated.h"

UCLASS()
class ASSIGNMENT4_API AIsometricCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AIsometricCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key, bool bNegate, bool bSwizzle, EInputAxisSwizzle SwizzleOrder);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RotateCameraCounterClockwise();

	void RotateCameraClockwise();

	void Zoom(const FInputActionValue& ActionValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Camera")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	float RotationAngleIncrement = 45.0f;
	float TargetYaw = 0.0f;
	float RotationSpeed = 3.0f;
	float TotalRotation = 0.0f;
	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;
	UPROPERTY()
	class UInputAction* RotateClockwise;
	UPROPERTY()
	class UInputAction* RotateCounterClockwise;
	UPROPERTY()
	class UInputAction* ZoomAction;
};
