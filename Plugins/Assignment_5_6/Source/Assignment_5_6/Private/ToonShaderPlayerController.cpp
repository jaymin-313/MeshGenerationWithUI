// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonShaderPlayerController.h"

AToonShaderPlayerController::AToonShaderPlayerController()
{

}

void AToonShaderPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
    if (SelectionAreaClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SelectionAreaInstance = GetWorld()->SpawnActor<ASelectionArea>(SelectionAreaClass, SpawnParams);

    }
}

void AToonShaderPlayerController::Tick(float DeltaTime)
{
    if (SelectionAreaInstance) {
        FVector2D MousePosition;
        if (!GetMousePosition(MousePosition.X, MousePosition.Y))
            return;

        // Perform a trace from the mouse position
        FHitResult HitResult;
        GetHitResultAtScreenPosition(MousePosition, ECollisionChannel::ECC_Visibility, true, HitResult);

        // Set actor location to the hit location if trace is successful
        if (HitResult.IsValidBlockingHit() && bMove)
        {
            if (SelectionAreaInstance->IsSphere())
                HitResult.Location.Z = SelectionAreaInstance->GetActorScale3D().Z * 100.0f;
            else
                HitResult.Location.Z = 0.0f;
            // Set the location of the SelectionAreaInstance
            SelectionAreaInstance->SetActorLocation(HitResult.Location);
        }
    }

}
