// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <PawnAttributeAsset.h>
#include "Engine/DataTable.h"
#include "GameFramework/Pawn.h"
#include "FirstPersonPawn.generated.h"


class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;

UENUM(BlueprintType)
enum class EPawnType : uint8
{
	ThirdPerson,
	FirstPerson,
	TopDown
};

USTRUCT(BlueprintType)
struct FPawnData : public FTableRowBase
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPawnType PawnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> PawnClass;
};


UCLASS()
class ASSIGNMENT_1_2_API AFirstPersonPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFirstPersonPawn();
	UPROPERTY(EditDefaultsOnly, Category = "Pawn Attributes")
	UPawnAttributeAsset* FirstPersonAttributes;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const struct FInputActionValue& ActionValue);

	void Rotate(const struct FInputActionValue& ActionValue);
	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Head;


	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* Movement;

	UPROPERTY(EditAnywhere)
	float MoveScale;

	UPROPERTY(EditAnywhere)
	float RotateScale;

	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;

	/**Action to update location. */
	UPROPERTY()
	class UInputAction* MoveAction;

	UPROPERTY()
	class UInputAction* RotateAction;

};
