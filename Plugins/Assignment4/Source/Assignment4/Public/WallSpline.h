// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "WallSpline.generated.h"

UCLASS()
class ASSIGNMENT4_API AWallSpline : public AActor
{
	GENERATED_BODY()
	
public:
	AWallSpline();
	void OnLeftMouseButtonPressed(const FVector& MouseWorldLocation);

	void OnRightMouseButtonPressed();


	void CreateSplineFromPoints();
	void Undo();
	//TArray<FVector> SplinePoints;
	TArray<USplineMeshComponent*>SplineMesh;
	USplineComponent* SplineComponent;
protected:
	//virtual void InputTouch(float DeltaTime, FViewport* Viewport, FSceneView* View, FPointerEvent& InputEvent) override;
private:
	UPROPERTY(VisibleAnywhere)
	FVector StartPoint;
	FVector EndPoint;
};
