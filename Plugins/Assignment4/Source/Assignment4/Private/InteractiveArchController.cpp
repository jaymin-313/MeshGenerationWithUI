// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

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
}

void AInteractiveArchController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AInteractiveArchController::OnLeftMouseClick);
    InputComponent->BindAction("TabkeyClick", IE_Pressed, this, &AInteractiveArchController::ToggleUI);
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
        AArchMeshActor* HitMeshActor = Cast<AArchMeshActor>(HitResult.GetActor());
        if (HitMeshActor) {
            // If there is a MeshActor, select it as the current MeshActor
            MeshActor = HitMeshActor;
            HitLocation = HitMeshActor->GetActorLocation();
            SelectionBoxInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
            SelectionBoxInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
            SelectionBoxInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
        }
        else {

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