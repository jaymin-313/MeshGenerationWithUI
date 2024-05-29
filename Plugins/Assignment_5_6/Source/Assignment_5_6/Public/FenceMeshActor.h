// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include "VerticalRailActor.h"
#include "FenceMeshActor.generated.h"

USTRUCT(BlueprintType)
struct FFenceProperties {
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Fence Properties")
	float Length;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Fence Properties")
	float Width;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Fence Properties")
	float Height;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Fence Properties")
	float Spacing;


	FFenceProperties() : Length( 100.0f ), Width( 100.0f ), Height( 100.0f ), Spacing( 0.0f ){}
};

UCLASS()
class ASSIGNMENT_5_6_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFenceMeshActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	UMaterialInterface* FenceMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	FFenceProperties FenceProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	TSubclassOf<class AVerticalRailActor> VerticalRailClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence")
	UStaticMesh* PreBuiltMesh = LoadObject<UStaticMesh>(this,TEXT("/Script/Engine.StaticMesh'/Assignment_5_6/ProcMesh.ProcMesh'"));
	UPROPERTY()
	TArray<AVerticalRailActor*> RailArray;
	UPROPERTY()
	TArray<UStaticMeshComponent*> PreBuiltMeshComponents;
	int32 Temp;
private:
	void GenerateFence();
	void AddStaticMeshComponents();
	void DestroyAllPreBuiltComponents();
};