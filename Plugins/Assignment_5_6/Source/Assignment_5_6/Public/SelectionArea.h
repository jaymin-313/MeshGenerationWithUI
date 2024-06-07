// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "SelectionArea.generated.h"

UCLASS()
class ASSIGNMENT_5_6_API ASelectionArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionArea();

	UFUNCTION(BlueprintCallable)
	void ToggleShape();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float GetRadius();
	bool IsSphere();
	void GenerateSphere(float Radius, int32 Segments, int32 Rings, int SectionIndex);
	void CreateBox(float Dimension);
	FVector GetDimensions() const;

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;

	bool bIsSphere;
	FVector Dimensions;
	float  Radius;

	UMaterial* TranslucentMaterial;
};
