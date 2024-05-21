// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicPawnController.h"
#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include <Components/SphereComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "FirstPersonPawn.h"
#include "BaseCharacter.h"
#include "PawnAttributeAsset.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "Kismet/GameplayStatics.h"
#include "../../../../../Source/UE_AssignmentUI/TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "../../../../../Source/UE_AssignmentUI/TP_TopDown/TP_TopDownCharacter.h"


ADynamicPawnController::ADynamicPawnController()
{
	CurrentPawnIndex = 0;
	CurrentRowIndex = -1;
	InputComponent = CreateDefaultSubobject<UInputComponent>(TEXT("InputComponent"));
}


void ADynamicPawnController::BeginPlay()
{
	Super::BeginPlay();


	if (const UDataTable* PawnDataTable = LoadObject<UDataTable>(GetWorld(), TEXT("/Script/Engine.DataTable'/Assignment_1_2/PawnDataTable.PawnDataTable'")))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Datatable found")));
		TArray<FName> RowNames = PawnDataTable->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			FString Temp = RowName.ToString();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Temp);

			FPawnData* PawnData = PawnDataTable->FindRow<FPawnData>(RowName, TEXT(""));
			if (PawnData)
			{
				PawnDataList.Add(*PawnData);
			}
		}
		if (PawnDataList.Num() > 0)
		{
			CurrentRowIndex = 0;
			SpawnPawn();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No rows found in data table."));
		}
	}
}

void ADynamicPawnController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Message")));
		InputComponent->BindAction("ToggleSpawnPawn", IE_Pressed, this, &ADynamicPawnController::ToggleSpawnPawn);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Failed")));
		UE_LOG(LogTemp, Error, TEXT("InputComponent is null."));
	}
}




void ADynamicPawnController::ToggleSpawnPawn()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("ToggleSpawn Called")));

	// If a pawn is already spawned, destroy it and toggle to the next type
	if (SP)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Pawn Is Spawned and pending")));
		DestroySpawnedPawn();
		CurrentPawnIndex++; // Move to the next type in the list
	}

	// Ensure CurrentPawnIndex wraps around if it exceeds the number of items in the list
	if (CurrentPawnIndex >= PawnDataList.Num())
	{
		CurrentPawnIndex = 0;
	}
	UE_LOG(LogTemp, Warning, TEXT("Spawning new pawn of index: %d"), CurrentPawnIndex);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Spawning new pawn of index : %d"),CurrentPawnIndex));
	SpawnPawn();
}

void ADynamicPawnController::SpawnPawn()
{

	FVector SL;
	FRotator SR;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Function::Spawn pawn")));
	if (PawnDataList.Num() == 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("No Data Available")));
		UE_LOG(LogTemp, Warning, TEXT("No pawn data available."));
		return;
	}

	if (CurrentPawnIndex > PawnDataList.Num())
	{
		CurrentPawnIndex = 0;
	}

	// Spawn the pawn based on the current index
	FPawnData& CurrentPawnData = PawnDataList[CurrentPawnIndex];
	if (CurrentPawnData.PawnClass)
	{

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SL = FVector(CurrentPawnIndex * 200, 0, 500); // Set spawn location as needed
		SR = FRotator::ZeroRotator;

		if (CurrentPawnData.PawnType == EPawnType::FirstPerson)
		{
			AFirstPersonPawn* SpawnedPawnActor = GetWorld()->SpawnActor<AFirstPersonPawn>(CurrentPawnData.PawnClass, SL, SR, SpawnParams);
			if (SpawnedPawnActor)
			{
				Possess(SpawnedPawnActor);
				SP = SpawnedPawnActor;

				DisplayAttributes(SpawnedPawnActor->FirstPersonAttributes);
				SetShowMouseCursor(false);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Failed to spawn first person pawn.")));
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn first person pawn."));
			}
		}
		else if (CurrentPawnData.PawnType == EPawnType::ThirdPerson)
		{
			ATP_ThirdPersonCharacter* SpawnedCharacter = GetWorld()->SpawnActor<ATP_ThirdPersonCharacter>(CurrentPawnData.PawnClass, SL, SR, SpawnParams);
			if (SpawnedCharacter)
			{
				Possess(SpawnedCharacter);
				SP = SpawnedCharacter;
				DisplayAttributes(SpawnedCharacter->ThirdPersonAttributes);
				SetShowMouseCursor(false);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Failed to spawn third person character.")));
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn third person character."));
			}
		}
		else if (CurrentPawnData.PawnType == EPawnType::TopDown) // Add this condition for top-down character
		{
			ATP_TopDownCharacter* SpawnedTopDownCharacter = GetWorld()->SpawnActor<ATP_TopDownCharacter>(CurrentPawnData.PawnClass, SL, SR, SpawnParams);
			if (SpawnedTopDownCharacter)
			{
				Possess(SpawnedTopDownCharacter);
				SP = SpawnedTopDownCharacter;
				DisplayAttributes(SpawnedTopDownCharacter->TopDownAttributes);
				SetShowMouseCursor(true);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Failed to spawn top-down character.")));
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn top-down character."));
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Invalid pawn type.")));
			UE_LOG(LogTemp, Error, TEXT("Invalid pawn type."));
		}
	}

	//CurrentPawnIndex++;

}

void ADynamicPawnController::DestroySpawnedPawn()
{
	if (SP)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(TEXT("Destroying this pawn")));
		SP->Destroy();
		SP = nullptr;
	}
}
