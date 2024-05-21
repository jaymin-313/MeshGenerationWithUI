// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "TopDownPawn.generated.h"

UCLASS()
class ASSIGNMENT4_API ATopDownPawn : public APawn
{
	GENERATED_BODY()

public:
	void MapKey(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key, bool bNegate, bool bSwizzle, EInputAxisSwizzle SwizzleOrder);
	// Sets default values for this pawn's properties
	ATopDownPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const struct FInputActionValue& ActionValue);

	void Zoom(const struct FInputActionValue& ActionValue);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;

private:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;


	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* Movement;


	UPROPERTY(EditAnywhere)
	float MoveScale;

	UPROPERTY(EditAnywhere)
	float ZoomScale;

	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;

	/**Action to update location. */
	UPROPERTY()
	class UInputAction* MoveAction;

	UPROPERTY()
	class UInputAction* ZoomAction;

};
