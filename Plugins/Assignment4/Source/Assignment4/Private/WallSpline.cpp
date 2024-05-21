// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpline.h"
#include "Kismet/GameplayStatics.h"
#include "WallBuilderController.h"
#include "Components/SplineMeshComponent.h"

AWallSpline::AWallSpline()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	RootComponent = SplineComponent;
	SplineComponent->ClearSplinePoints();
}


void AWallSpline::OnLeftMouseButtonPressed(const FVector& MouseWorldLocation)
{
	SplineComponent->AddSplinePoint(MouseWorldLocation, ESplineCoordinateSpace::World);
	CreateSplineFromPoints();
}

void AWallSpline::CreateSplineFromPoints()
{
	if (SplineComponent->GetNumberOfSplinePoints() > 1)
	{
		// Create spline mesh component

		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		//SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Y);

		// Set spline mesh properties
		UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Wall_400x400.Wall_400x400'")); // Replace with your own mesh
		if (Mesh)
		{
			SplineMeshComponent->SetStaticMesh(Mesh);

			FVector StartPos = SplineComponent->GetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 2, ESplineCoordinateSpace::World);
			FVector EndPos = SplineComponent->GetLocationAtSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World);

			SplineMeshComponent->SetStartAndEnd(StartPos, SplineComponent->GetTangentAtSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 2, ESplineCoordinateSpace::World), EndPos, SplineComponent->GetTangentAtSplinePoint(SplineComponent->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World));

		}
		UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Basic_Wall.M_Basic_Wall'"));
		SplineMeshComponent->SetMaterial(0, Material);

		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->RegisterComponent();
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Black, TEXT("addedd mesh"));

		SplineMesh.Add(SplineMeshComponent);
	}
}

void AWallSpline::Undo() {
	if (SplineMesh.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("Undoing Wall from wallspline"));
		SplineComponent->RemoveSplinePoint(SplineMesh.Num());
		SplineMesh[SplineMesh.Num() - 1]->DestroyComponent();
		SplineMesh.RemoveAt(SplineMesh.Num() - 1);
	}
	else {
		SplineComponent->ClearSplinePoints();
		SplineMesh.Empty();
	}
}