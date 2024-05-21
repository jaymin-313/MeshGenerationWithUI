// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaircaseActor.generated.h"

UENUM(BlueprintType)
enum class StaircaseType : uint8 {
	OpenStairs UMETA(DisplayName = " Open Stairs"),
	ClosedStairs UMETA(DisplayName = " Close Stairs"),
	BoxStairs UMETA(DisplayName = " Box Stairs")

};
UCLASS()
class ASSIGNMENT_1_2_API AStaircaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaircaseActor();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stairs")
	int32 NumberOfSteps= 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	float Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	float Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	float Depth;

	/*UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stairs")
	UStaticMeshComponent* Stepmesh;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	FVector Dimentions;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	UStaticMesh* StairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rails");
	UStaticMesh* RailsMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	StaircaseType MyStair = StaircaseType::OpenStairs;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterial* StairMaterial;

	UFUNCTION()
	bool CreateStairs(float abc);

	void ClearAllArrays(TArray<UStaticMeshComponent*> StepComponents, TArray<UStaticMeshComponent*> LeftRailingComponents, TArray<UStaticMeshComponent*> RightRailingComponents);

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform);

private:
	TArray<UStaticMeshComponent*>StepComponents;
	TArray<UStaticMeshComponent*>LeftRailingComponents;
	TArray<UStaticMeshComponent*>RightRailingComponents;
	TArray<UStaticMeshComponent*>HandleRailComponents;
	
};
