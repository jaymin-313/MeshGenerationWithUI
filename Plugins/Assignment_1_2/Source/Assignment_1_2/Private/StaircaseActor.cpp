// Fill out your copyright notice in the Description page of Project Settings.


#include "StaircaseActor.h"

// Sets default values
AStaircaseActor::AStaircaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	SetRootComponent(Scene);

	Width = 1;
	Height = 0.3;
	Depth = 4;
	//Stepmesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stair Mesh"));
}

// Called when the game starts or when spawned
void AStaircaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStaircaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStaircaseActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	for (UStaticMeshComponent* StepComponent : StepComponents)
	{
		StepComponent->DestroyComponent();
	}
	StepComponents.Empty();

	for (UStaticMeshComponent* LeftRailComponent : LeftRailingComponents)
	{
		LeftRailComponent->DestroyComponent();
	}
	LeftRailingComponents.Empty();
	for (UStaticMeshComponent* RightRailComponent : RightRailingComponents)
	{
		RightRailComponent->DestroyComponent();
	}
	RightRailingComponents.Empty();
	bool bStatus;
	bool bStatus2;
	switch (MyStair)
	{
	case StaircaseType::OpenStairs:
		bStatus = CreateStairs(0.3);
		if (bStatus) {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Open Stairs Created."), false);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Failed to create stairs."), false);
		}
		break;
	case StaircaseType::ClosedStairs:
		bStatus2 = CreateStairs(0.5);
		if (bStatus2) {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Closed Stairs Created."), false);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Failed to create stairs."), false);
		}
		break;
	case StaircaseType::BoxStairs:
		for (int i = 0; i < NumberOfSteps; i++) {
			FString name = "Stair" + FString::FromInt(i);
			UStaticMeshComponent* Stair = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *name);
			Stair->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Stair->RegisterComponentWithWorld(GetWorld());
			//FVector loc(Width, Depth, Height); // for open stairs
			FVector loc(Width, Depth, (i + 1) * Height);
			Stair->SetRelativeScale3D(loc);
			//FVector NewLocation = FVector(100.0f, 0.0f, 50.0f);// for open stairs
			FVector NewLocation = FVector(i * 100.0f, 0.0f, 50.0f);
			//Stair->SetRelativeLocation({ i * NewLocation }); // for open stairs
			Stair->SetRelativeLocation({ NewLocation });
			Stair->SetStaticMesh(StairMesh);
			Stair->SetMaterial(0, StairMaterial);
			StepComponents.Add(Stair);

			FString RailName = "LeftRail" + FString::FromInt(i);
			UStaticMeshComponent* LeftRails = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *RailName);
			LeftRails->AttachToComponent(Stair, FAttachmentTransformRules::KeepRelativeTransform);
			LeftRails->RegisterComponentWithWorld(GetWorld());
			FVector RailDimentions(2.0f, 0.07f, ((i+1) * 0.1f)+0.3);
			LeftRails->SetWorldScale3D(RailDimentions); //World for box
			FVector NewLocation2 = FVector(0.0f, -50, 0.0f);
			LeftRails->SetRelativeLocation({ NewLocation2 });
			LeftRails->SetStaticMesh(RailsMesh);
			LeftRails->SetMaterial(0, StairMaterial);
			LeftRailingComponents.Add(LeftRails);

			FString RailName2 = "RightRail" + FString::FromInt(i);
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("This is changine RIght rail -- %s"),false);
			UStaticMeshComponent* RightRails = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *RailName2);
			RightRails->AttachToComponent(Stair, FAttachmentTransformRules::KeepRelativeTransform);
			RightRails->RegisterComponentWithWorld(GetWorld());
			FVector RailDimentions2(2.0f, 0.07f, ((i + 1) * 0.1f) + 0.3);
			RightRails->SetWorldScale3D(RailDimentions2);
			FVector NewLocation3 = FVector(0.0f, 50, 0.0f);
			RightRails->SetRelativeLocation({ NewLocation3 });
			RightRails->SetStaticMesh(RailsMesh);
			RightRails->SetMaterial(0, StairMaterial);
			RightRailingComponents.Add(RightRails);

		}
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("No matched input"), false);
		break;
	}
}

bool AStaircaseActor::CreateStairs(float InputHeight) {
	for (int i = 0; i < NumberOfSteps; i++) {
		Height = InputHeight;

		FString name = "Stair" + FString::FromInt(i);
		UStaticMeshComponent* Stair = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *name);
		Stair->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		Stair->RegisterComponentWithWorld(GetWorld());
		FVector loc(Width, Depth, Height); // for open stairs
		//FVector loc(Width, Depth, (i + 1) * Height);
		Stair->SetRelativeScale3D(loc);
		FVector NewLocation = FVector(100.0f, 0.0f, 50.0f);// for open stairs
		//FVector NewLocation = FVector(i * 100.0f, 0.0f, 50.0f);
		Stair->SetRelativeLocation({ i * NewLocation }); // for open stairs
		//Stair->SetRelativeLocation({ NewLocation });
		Stair->SetStaticMesh(StairMesh);
		Stair->SetMaterial(0, StairMaterial);
		StepComponents.Add(Stair);

		FString RailName = "LeftRail" + FString::FromInt(i);
		UStaticMeshComponent* LeftRails = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *RailName);
		LeftRails->AttachToComponent(Stair, FAttachmentTransformRules::KeepRelativeTransform);
		LeftRails->RegisterComponentWithWorld(GetWorld());
		FVector RailDimentions(0.25f, 0.07f, 2.0f);
		LeftRails->SetRelativeScale3D(RailDimentions); //World for box
		//float StairY = StairMesh->GetBounds().GetBox().GetSize().Y;
		FVector NewLocation2 = FVector(0.0f, -46, 0.0f);
		LeftRails->SetRelativeLocation({ NewLocation2 });
		LeftRails->SetStaticMesh(RailsMesh);
		LeftRails->SetMaterial(0, StairMaterial);
		LeftRailingComponents.Add(LeftRails);

		FString RailName2 = "RightRail" + FString::FromInt(i);
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("This is changine RIght rail -- %s"),false);
		UStaticMeshComponent* RightRails = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *RailName2);
		RightRails->AttachToComponent(Stair, FAttachmentTransformRules::KeepRelativeTransform);
		RightRails->RegisterComponentWithWorld(GetWorld());
		FVector RailDimentions2(0.25f, 0.07f, 2.0f);
		RightRails->SetRelativeScale3D(RailDimentions2);
		FVector NewLocation3 = FVector(0.0f, 46, 0.0f);
		RightRails->SetRelativeLocation({ NewLocation3 });
		RightRails->SetStaticMesh(RailsMesh);
		RightRails->SetMaterial(0, StairMaterial);
		RightRailingComponents.Add(RightRails);

	
			
	}

	
	return true;
}