// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"
#include "GameFramework/Actor.h"
#include "IsometricCameraPawn.h"
#include "OrthographicCameraPawn.h"
#include "PerspectiveCameraPawn.h"

#include "Kismet/GameplayStatics.h"
#include "WallBuilderController.h"



void AInteractiveArchController::BeginPlay()
{
    bShowMouseCursor = true;
    // Create an instance of the SelectionWidget
    if (SelectionBoxClass) {
        SelectionBoxInstance = CreateWidget<USelectionBox>(this,SelectionBoxClass);
        WallWidgetInstance = CreateWidget<UMyWallWidget>(this, WallWidgetClass);
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
       OrthographicPawn = GetWorld()->SpawnActor<AOrthographicCameraPawn>(AOrthographicCameraPawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
   }
   if (IsometricPawn) {
       //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("I selected"));
       //IsometricPawn->SetActorLocation(FVector(0.0f, 0.0f, 200.0f));
       SwitchToIsometricView();
   }
   else {
       GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Isometric not Spawned"));

   }

   //For Wall Builder
   WallSplineActor.Add(NewObject<AWallSpline>(this));
}
static void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
    bool bNegate = false,
    bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{
    FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
    UObject* Outer = InputMappingContext->GetOuter();

    if (bNegate) {
        UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
        Mapping.Modifiers.Add(Negate);
    }

    if (bSwizzle) {
        UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
        Swizzle->Order = SwizzleOrder;
        Mapping.Modifiers.Add(Swizzle);
    }
}
void AInteractiveArchController::Switch() {
    if (!bIsWallBuilder) {
        bIsWallBuilder = true;
        SelectionBoxInstance->RemoveFromViewport();
        WallWidgetInstance->AddToViewport();
        SubSystem->RemoveMappingContext(MyMapping);
        SubSystem->AddMappingContext(SplineMappingContext, 0);
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Changing To Wall Builder"));
    }
    else {
        bIsWallBuilder = false;
        WallWidgetInstance->RemoveFromViewport();
        SelectionBoxInstance->AddToViewport();
        SubSystem->RemoveMappingContext(SplineMappingContext);
        SubSystem->AddMappingContext(MyMapping,0);
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Changint To Mesh Generator"));
    }
}

void AInteractiveArchController::SetupInputComponent()
{
    Super::SetupInputComponent();

    NewObject<APlayerController>(this, AInteractiveArchController::StaticClass());

    MyMapping = NewObject<UInputMappingContext>(this);

    UInputAction* ClickAction = NewObject<UInputAction>(this);
    ClickAction->ValueType = EInputActionValueType::Boolean;
    MyMapping->MapKey(ClickAction, EKeys::LeftMouseButton);

    UInputAction* ToggleAction = NewObject<UInputAction>(this);
    ToggleAction->ValueType = EInputActionValueType::Boolean;
    MyMapping->MapKey(ToggleAction, EKeys::Tab);

    /*UInputAction* ChangeViewToI = NewObject<UInputAction>(this);
    ChangeViewToI->ValueType = EInputActionValueType::Boolean;
    MyMapping->MapKey(ChangeViewToI, EKeys::I);
    
    UInputAction* ChangeViewToO = NewObject<UInputAction>(this);
    ChangeViewToO->ValueType = EInputActionValueType::Boolean;
    MyMapping->MapKey(ChangeViewToO, EKeys::O);
    
    UInputAction* ChangeViewToP = NewObject<UInputAction>(this);
    ChangeViewToP->ValueType = EInputActionValueType::Boolean;
    MyMapping->MapKey(ChangeViewToP, EKeys::P);*/

    CameraMapping = NewObject<UInputMappingContext>(this);
    UInputAction* SwitchCamera = NewObject<UInputAction>(this);
    SwitchCamera->ValueType = EInputActionValueType::Boolean;
    CameraMapping->MapKey(SwitchCamera, EKeys::P);

    ////Wall Spline
    SplineMappingContext = NewObject<UInputMappingContext>(this);

    LeftClickAction = NewObject<UInputAction>(this);
    LeftClickAction->ValueType = EInputActionValueType::Boolean;
    KeyMap(SplineMappingContext, LeftClickAction, EKeys::LeftMouseButton);

    RightClickAction = NewObject<UInputAction>(this);
    RightClickAction->ValueType = EInputActionValueType::Boolean;
    KeyMap(SplineMappingContext, RightClickAction, EKeys::RightMouseButton);

    UndoAction = NewObject<UInputAction>(this);
    UndoAction->ValueType = EInputActionValueType::Boolean;
    KeyMap(SplineMappingContext, UndoAction, EKeys::Z);

    DeleteAction = NewObject<UInputAction>(this);
    DeleteAction->ValueType = EInputActionValueType::Boolean;
    KeyMap(SplineMappingContext, DeleteAction, EKeys::Delete);

    ClearAction = NewObject<UInputAction>(this);
    ClearAction->ValueType = EInputActionValueType::Boolean;
    KeyMap(SplineMappingContext, ClearAction, EKeys::C);



    UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

    if (EIC) {
        EIC->BindAction(ClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnLeftMouseClick);
        EIC->BindAction(ToggleAction, ETriggerEvent::Completed, this, &AInteractiveArchController::ToggleUI);
        /*EIC->BindAction(ChangeViewToI, ETriggerEvent::Completed, this, &AInteractiveArchController::SwitchToIsometricView);
        EIC->BindAction(ChangeViewToO, ETriggerEvent::Completed, this, &AInteractiveArchController::SwitchToOrthographicView);
        EIC->BindAction(ChangeViewToP, ETriggerEvent::Completed, this, &AInteractiveArchController::SwitchToPerspectiveView);*/
        EIC->BindAction(SwitchCamera, ETriggerEvent::Completed, this, &AInteractiveArchController::SwitchCameraType);



        EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnLeftMouseButtonPressed);
        EIC->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnRightMouseButtonPressed);
        EIC->BindAction(UndoAction, ETriggerEvent::Completed, this, &AInteractiveArchController::UndoWall);
        EIC->BindAction(DeleteAction, ETriggerEvent::Completed, this, &AInteractiveArchController::DeleteWall);
        EIC->BindAction(ClearAction, ETriggerEvent::Completed, this, &AInteractiveArchController::ClearWalls);
    }

    LocalPlayer = GetLocalPlayer();

    SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    //SubSystem->ClearAllMappings();
    if (!bIsWallBuilder) {
        SubSystem->AddMappingContext(MyMapping, 0); 

    }
    else {
        SubSystem->AddMappingContext(SplineMappingContext, 0);
    }
    SubSystem->AddMappingContext(CameraMapping, 0);
}

void AInteractiveArchController::OnLeftMouseClick()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("LEft mouse button clicked"));
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
            if (CurrentPawn == IsometricPawn) {
                //IsometricPawn = Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AIsometricCameraPawn::StaticClass()));
                if (IsometricPawn) {
                    IsometricPawn->Destroy();
                    FActorSpawnParameters SpawnParams;
                    IsometricPawn = GetWorld()->SpawnActor<AIsometricCameraPawn>(AIsometricCameraPawn::StaticClass(), HitLocation, FRotator::ZeroRotator, SpawnParams);
                    SwitchToIsometricView();
                }
            }
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
    if (IsometricPawn) {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Switching To Isometric View"));
        Possess(IsometricPawn);
        CurrentPawn = IsometricPawn;
        SetupInputComponent();
    }
}

void AInteractiveArchController::SwitchToOrthographicView()
{
    if (OrthographicPawn) {
        
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Switching To Orthographic View"));
        Possess(OrthographicPawn);
        CurrentPawn = OrthographicPawn;
        SetupInputComponent();

    }
}

void AInteractiveArchController::SwitchToPerspectiveView()
{
    if (PerspectivePawn) {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Switchig To Perspective View"));
        Possess(PerspectivePawn);
        CurrentPawn = PerspectivePawn;
        SetupInputComponent();
    }
}
void AInteractiveArchController::SwitchCameraType() {
    if (CurrentPawn == IsometricPawn) {
        SwitchToOrthographicView();
    }
    else if (CurrentPawn == OrthographicPawn) {
        SwitchToPerspectiveView();
    }
    else {
        SwitchToIsometricView();
    }
}

void AInteractiveArchController::OnLeftMouseButtonPressed()
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Click ")));

    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    if (HitResult.bBlockingHit)
    {
        FVector ClickLocation = HitResult.Location;

        if (WallSplineActor[WallSplineActor.Num()-1]->SplineComponent->GetNumberOfSplinePoints() == 1) {
            //ShowText.ExecuteIfBound("Wall Creation started");
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Wall Creation Started"));
        }
        WallSplineActor[WallSplineActor.Num()-1]->OnLeftMouseButtonPressed(ClickLocation);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Click Location: %s"), *ClickLocation.ToString()));

    }
}

void AInteractiveArchController::OnRightMouseButtonPressed()
{   
    if(WallSplineActor.Num()>0){
         GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("Wall Creation Completed"));
        //ShowText.ExecuteIfBound("Wall Creation Completed");

    }
    WallSplineActor.Add(NewObject<AWallSpline>(this));
    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Now you can create new wall segment"));
}

void AInteractiveArchController::UndoWall() {
    int LastIndex = WallSplineActor[WallSplineActor.Num() - 1]->SplineComponent->GetNumberOfSplinePoints() - 1;
    if (LastIndex > 0) {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Undo Logic Triggered"));
        WallSplineActor[WallSplineActor.Num() - 1]->Undo();
    }
    else {
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("No More Points To Undo"));
        //ShowText.ExecuteIfBound("No More Points");
        WallSplineActor[WallSplineActor.Num() - 1]->SplineComponent->ClearSplinePoints();
    }
}
void AInteractiveArchController::DeleteWall() {
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Delete Logic Triggered"));
    WallSplineActor[WallSplineActor.Num() - 1]->Destroy();
    //ShowText.ExecuteIfBound("Deleted Current Wall");
    //WallSplineActor[WallSplineActor.Num() - 1]->SplineComponent->ClearSplinePoints();
    WallSplineActor.RemoveAt(WallSplineActor.Num() - 1);
    if (WallSplineActor.Num() == 0) {
        WallSplineActor.Add(NewObject<AWallSpline>(this));
    }

}
void AInteractiveArchController::ClearWalls() {
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Delete All Triggered"));
    for (int i = 0; i < WallSplineActor.Num(); i++) {
        WallSplineActor[i]->Destroy();
        //WallSplineActor[i]->SplineComponent->ClearSplinePoints();
    }
    //ShowText.ExecuteIfBound("Deleted All Walls");
    WallSplineActor.Empty();
    WallSplineActor.Add(NewObject<AWallSpline>(this));
}