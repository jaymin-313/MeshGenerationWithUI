// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"
#include "GameFramework/Actor.h"
#include "IsometricCameraPawn.h"
#include "OrthographicCameraPawn.h"
#include "PerspectiveCameraPawn.h"
#include "Kismet/GameplayStatics.h"


void AInteractiveArchController::BeginPlay()
{
    bShowMouseCursor = true;
    // Create an instance of the SelectionWidget
    if (SelectionBoxClass) {
        SelectionBoxInstance = CreateWidget<USelectionBox>(this,SelectionBoxClass);

        if (SelectionBoxInstance)
        {
            SelectionBoxInstance->AddToViewport();
            SelectionBoxInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
            SelectionBoxInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
            SelectionBoxInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
            SelectionBoxInstance->MeshSelectionScrollBoxWidget->MeshSelected.BindUObject(this, &AInteractiveArchController::SpawnMesh);
            SelectionBoxInstance->MaterialSelectionScrollBoxWidget->MaterialSelected.BindUObject(this, &AInteractiveArchController::ApplyMaterial);
            SelectionBoxInstance->TextureSelectionScrollBoxWidget->TextureSelected.BindUObject(this, &AInteractiveArchController::ApplyTexture);
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Added"));
        }
    }
   IsometricPawn = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AIsometricCameraPawn::StaticClass()));
   if (!IsometricPawn)
   {
       FActorSpawnParameters SpawnParams;
       IsometricPawn = GetWorld()->SpawnActor<AIsometricCameraPawn>(AIsometricCameraPawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
   }
   
   PerspectivePawn = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APerspectiveCameraPawn::StaticClass()));
   if (!PerspectivePawn)
   {
      /* PerspectivePawn = SpawnCameraPawn<APerspectiveCameraPawn>();*/
       FActorSpawnParameters SpawnParams;
       PerspectivePawn =  GetWorld()->SpawnActor<APerspectiveCameraPawn>(APerspectiveCameraPawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
   }
   OrthographicPawn = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AOrthographicCameraPawn::StaticClass()));
   if (!OrthographicPawn)
   {
       FActorSpawnParameters SpawnParams;
       PerspectivePawn = GetWorld()->SpawnActor<AOrthographicCameraPawn>(AOrthographicCameraPawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
   }
   if (IsometricPawn) {
       //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("I selected"));
       //IsometricPawn->SetActorLocation(FVector(0.0f, 0.0f, 200.0f));
       Possess(IsometricPawn);
       CurrentPawn = IsometricPawn;
   }
   else {
       GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Isometric not Spawned"));

   }
}

void AInteractiveArchController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AInteractiveArchController::OnLeftMouseClick);
    InputComponent->BindAction("TabkeyClick", IE_Pressed, this, &AInteractiveArchController::ToggleUI);
    InputComponent->BindAction("SwitchToIsometric", IE_Pressed, this, &AInteractiveArchController::SwitchToIsometricView);
    InputComponent->BindAction("SwitchToOrthographic", IE_Pressed, this, &AInteractiveArchController::SwitchToOrthographicView);
    InputComponent->BindAction("SwitchToPerspective", IE_Pressed, this, &AInteractiveArchController::SwitchToPerspectiveView);
}
void AInteractiveArchController::OnLeftMouseClick()
{
    FVector2D MousePosition;
    if (GetMousePosition(MousePosition.X, MousePosition.Y)) {
        FHitResult HitResult;
        FVector WorldLocation, WorldDirection;
        DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
        GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
        HitLocation = HitResult.ImpactPoint;
        if (IsometricPawn) {
            MoveCameraToLocation(HitLocation);
        }
        AArchMeshActor* HitMeshActor = Cast<AArchMeshActor>(HitResult.GetActor());
        if (HitMeshActor) {
            // If there is a MeshActor, select it as the current MeshActor
            MeshActor = HitMeshActor;
            HitLocation = HitMeshActor->GetActorLocation();
            if (!bSlateVisible)
                ToggleUI();
            SelectionBoxInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
            SelectionBoxInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
            SelectionBoxInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
        }
        else {
            if (!bSlateVisible)
                ToggleUI();
            SelectionBoxInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
            SelectionBoxInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
            SelectionBoxInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

}
void AInteractiveArchController::SpawnMesh(const FMeshData& MeshData) {
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("Spwaning mesh"));
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (MeshActor && (MeshActor->GetActorLocation() == HitLocation)) {
        MeshActor->Destroy();
    }
    MeshActor = GetWorld()->SpawnActor<AArchMeshActor>(HitLocation, FRotator::ZeroRotator, SpawnParams);
    if (MeshActor) {
        MeshActor->SetMobility(EComponentMobility::Movable);
        MeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.MeshReference);
        SelectionBoxInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
        SelectionBoxInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
    }
}
void AInteractiveArchController::ApplyMaterial(const FMaterialData& MaterialData) {
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("Applying Material"));
    if (MeshActor) {
        MeshActor->GetStaticMeshComponent()->SetMaterial(0,MaterialData.MaterialReference);
    }
}void AInteractiveArchController::ApplyTexture(const FTextureData& TextureData) {
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("Applying Texture"));
    if (MeshActor) {
        UMaterialInstanceDynamic* DynamicMaterialInstance = MeshActor->GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0);
        if (DynamicMaterialInstance) {
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("Applying Texture"));
            DynamicMaterialInstance->SetTextureParameterValue(FName("ParamTexture"), TextureData.TextureReference);
        }
    }
}

void AInteractiveArchController::ToggleUI() {
    if (bSlateVisible) {
        SelectionBoxInstance->SetVisibility(ESlateVisibility::Hidden);
        bSlateVisible = false;  
    }
    else {
        SelectionBoxInstance->SetVisibility(ESlateVisibility::Visible);
        bSlateVisible = true;
    }
}

void AInteractiveArchController::SwitchToIsometricView()
{
    if (IsometricPawn)
        Possess(IsometricPawn);
}

void AInteractiveArchController::SwitchToOrthographicView()
{
    if (OrthographicPawn) {
        
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("switch to orthograpic view"));
        Possess(OrthographicPawn);

    }
}

void AInteractiveArchController::SwitchToPerspectiveView()
{
    if (PerspectivePawn) {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("switch to perspective view"));
        Possess(PerspectivePawn);
    }
}

void AInteractiveArchController::MoveCameraToLocation(FVector Location)
{
    if (CurrentPawn && Cast<AIsometricCameraPawn>(CurrentPawn))
    {
        Cast<AIsometricCameraPawn>(CurrentPawn)->MoveCameraToLocation(Location);
    }
}