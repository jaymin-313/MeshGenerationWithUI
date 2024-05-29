// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceMeshActor.h"

// Sets default values
AFenceMeshActor::AFenceMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

// Called when the game starts or when spawned
void AFenceMeshActor::BeginPlay()
{
	Super::BeginPlay();

	DestroyAllPreBuiltComponents();
	GenerateFence();
	
}

void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	DestroyAllPreBuiltComponents();
	AddStaticMeshComponents();
}

// Called every frame
void AFenceMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFenceMeshActor::GenerateFence()
{

	DestroyAllPreBuiltComponents();

	int32 NumSplinePoints = SplineComponent->GetNumberOfSplinePoints();
	for (int32 i = 0; i < NumSplinePoints - 1; ++i)
	{
		FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);
		SplineComponent->SetSplinePointType(i, ESplinePointType::Linear);
		float SegmentLength = FVector::Dist(StartLocation, EndLocation);
		int32 NumMeshes = FMath::CeilToInt(SegmentLength / (FenceProperties.Length + FenceProperties.Spacing));

		for (int32 j = 0; j < NumMeshes; ++j)
		{
			float T = (j * (FenceProperties.Length + FenceProperties.Spacing)) / SegmentLength;
			FVector Location = FMath::Lerp(StartLocation, EndLocation, T);
			Location.Z += j * 0.001f;
			Location.X += j * 0.001f;
			Location.Y += j * 0.001f;
			FRotator Rotation = (EndLocation - StartLocation).Rotation();
			if (VerticalRailClass) {
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("VERTICAL RAIL CLASS FOUND"));
				AVerticalRailActor* VerticalRail = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailClass, Location, Rotation);
				VerticalRail->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepWorldTransform);
				VerticalRail->SetActorRelativeScale3D({ FenceProperties.Length/100, FenceProperties.Width / 100, FenceProperties.Height / 100 });
				//VerticalRail->SetMaterial(FenceMaterial);
				UMaterialInstanceDynamic* VerticalDynamicMaterial = UMaterialInstanceDynamic::Create(FenceMaterial, this);
				if (VerticalDynamicMaterial) {
					float TileX = FenceProperties.Length / 100.0;
					float TileY = FenceProperties.Height / 10.0;
					VerticalDynamicMaterial->SetScalarParameterValue(FName("TileX"), TileX);
					VerticalDynamicMaterial->SetScalarParameterValue(FName("TileY"), TileY);
					VerticalDynamicMaterial->SetScalarParameterValue(FName("Roughness"), 0.5);
					VerticalDynamicMaterial->SetScalarParameterValue(FName("Metalic"), 0.5);
					VerticalRail->SetMaterial(0, VerticalDynamicMaterial);
				}
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(FenceMaterial, this);
				if (DynamicMaterial) {
					float TileX = FenceProperties.Length / 10.0;
					float TileY = FenceProperties.Height / 100.0;
					DynamicMaterial->SetScalarParameterValue(FName("TileX"), TileX);
					DynamicMaterial->SetScalarParameterValue(FName("TileY"), TileY);
					DynamicMaterial->SetScalarParameterValue(FName("Roughness"), 0.5);
					DynamicMaterial->SetScalarParameterValue(FName("Metalic"), 0.5);
					VerticalRail->SetMaterial(1, DynamicMaterial);
					VerticalRail->SetMaterial(2, DynamicMaterial);
				}
				RailArray.Add(VerticalRail);
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("VERTICAL RAIL CLASS FOUND"));

			}
		}
	}
}

void AFenceMeshActor::AddStaticMeshComponents()
{

	DestroyAllPreBuiltComponents();

	int32 NumSplinePoints = SplineComponent->GetNumberOfSplinePoints();
	for (int32 i = 0; i < NumSplinePoints - 1; ++i)
	{
		FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);
		SplineComponent->SetSplinePointType(i, ESplinePointType::Linear);
		float SegmentLength = FVector::Dist(StartLocation, EndLocation);
		int32 NumMeshes = FMath::CeilToInt(SegmentLength / (FenceProperties.Length + FenceProperties.Spacing));

		for (int32 j = 0; j < NumMeshes; ++j)
		{
			float T = (j * (FenceProperties.Length + FenceProperties.Spacing)) / SegmentLength;
			FVector Location = FMath::Lerp(StartLocation, EndLocation, T);
			FRotator Rotation = (EndLocation - StartLocation).Rotation();
			Location.Z += j * 0.001f;
			Location.X += j * 0.001f;
			Location.Y += j * 0.001f;

			FString name = "MeshComponent_" + FString::FromInt(i) + "_" + FString::FromInt(j);
			UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, *name);
			if (MeshComponent) {
				MeshComponent->SetStaticMesh(PreBuiltMesh);
				MeshComponent->SetWorldLocation(Location);
				MeshComponent->SetWorldRotation(Rotation);
				MeshComponent->SetWorldScale3D(FVector(FenceProperties.Length / 100.0f, FenceProperties.Width / 100.0f, FenceProperties.Height / 100.0f));
				MeshComponent->SetMobility(EComponentMobility::Movable);
				MeshComponent->RegisterComponent();
				MeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepWorldTransform);

				PreBuiltMeshComponents.Add(MeshComponent);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("PreBuiltMeshComponents.Num() after AddStaticMeshComponents: %d"), PreBuiltMeshComponents.Num());
}


void AFenceMeshActor::DestroyAllPreBuiltComponents() {
	for (int i = 0 ; i < PreBuiltMeshComponents.Num(); i++)
	{
		if (PreBuiltMeshComponents[i]) {
			PreBuiltMeshComponents[i]->DestroyComponent();
			PreBuiltMeshComponents[i]= nullptr;
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Destroying Static Mesh"));
				UE_LOG(LogTemp, Warning, TEXT("Destroying Mesh Component IN FUNCTION:"));		
			
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Destroying Static Mesh"));
				UE_LOG(LogTemp, Warning, TEXT("Destroying Mesh Component IN FUNCTION:"));		

		}
	}
	PreBuiltMeshComponents.Empty();
	//SplineComponent->ClearSplinePoints();
	UE_LOG(LogTemp, Warning, TEXT("PreBuiltMeshComponents.Num() after DestroyAllPreBuiltComponents: %d"), PreBuiltMeshComponents.Num());
}