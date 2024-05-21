// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.


#include "WallBuilderController.h"
#include "Kismet/GameplayStatics.h"
#include "WallSpline.h"
//#include "WallWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "Engine/Engine.h"


//void AWallBuilderController::KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
//    bool bNegate = false,
//    bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
//{
//    FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
//    UObject* Outer = InputMappingContext->GetOuter();
//
//    if (bNegate) {
//        UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
//        Mapping.Modifiers.Add(Negate);
//    }
//
//    if (bSwizzle) {
//        UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
//        Swizzle->Order = SwizzleOrder;
//        Mapping.Modifiers.Add(Swizzle);
//    }
//}
AWallBuilderController::AWallBuilderController()
{
    bShowMouseCursor = true;
}

void AWallBuilderController::EnableCursor()
{
    bShowMouseCursor = true;
    FInputModeGameAndUI InputMode;
    SetInputMode(InputMode);
}

void AWallBuilderController::BeginPlay()
{
    Super::BeginPlay();
    //UWallWidget* WallWidget = NewObject<UWallWidget>();

    //// Add the widget to the viewport
    //if (WallWidget)
    //{
    //    WallWidget->AddToViewport();
    //}
    // Spawn or get a reference to the WallSplineActor
    WallSplineActor.Add(NewObject<AWallSpline>(this));
    if (!WallSplineActor[0])
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Failed to Spawn WallSplineActor"));
    }

    ShowText.BindUFunction(this,"ShowWallWidget");
}
void AWallBuilderController::DisableCursor()
{
    bShowMouseCursor = false;
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
}

void AWallBuilderController::SetupInputComponent()
{
    Super::SetupInputComponent();

    SplineMappingContext = NewObject<UInputMappingContext>(this);

    LeftClickAction = NewObject<UInputAction>(this);
    LeftClickAction->ValueType = EInputActionValueType::Boolean;
    //KeyMap(SplineMappingContext, LeftClickAction, EKeys::LeftMouseButton);
    SplineMappingContext->MapKey(LeftClickAction, EKeys::LeftMouseButton);

    RightClickAction = NewObject<UInputAction>(this);
    RightClickAction->ValueType = EInputActionValueType::Boolean;
    //KeyMap(SplineMappingContext, RightClickAction, EKeys::RightMouseButton);
    SplineMappingContext->MapKey(RightClickAction, EKeys::RightMouseButton);


    UndoAction = NewObject<UInputAction>(this);
    UndoAction->ValueType = EInputActionValueType::Boolean;
    //KeyMap(SplineMappingContext, UndoAction, EKeys::Z);
    SplineMappingContext->MapKey(UndoAction, EKeys::Z);


    DeleteAction = NewObject<UInputAction>(this);
    DeleteAction->ValueType = EInputActionValueType::Boolean;
    //KeyMap(SplineMappingContext, DeleteAction, EKeys::Delete);
    SplineMappingContext->MapKey(DeleteAction, EKeys::Delete);


    ClearAction = NewObject<UInputAction>(this);
    ClearAction->ValueType = EInputActionValueType::Boolean;
    //KeyMap(SplineMappingContext, ClearAction, EKeys::C);
    SplineMappingContext->MapKey(ClearAction, EKeys::C);


    UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);


    if (EIC) {

        EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AWallBuilderController::OnLeftMouseButtonPressed);
        EIC->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AWallBuilderController::OnRightMouseButtonPressed);
        EIC->BindAction(UndoAction, ETriggerEvent::Completed, this, &AWallBuilderController::UndoWall);
        EIC->BindAction(DeleteAction, ETriggerEvent::Completed, this, &AWallBuilderController::DeleteWall);
        EIC->BindAction(ClearAction, ETriggerEvent::Completed, this, &AWallBuilderController::ClearWalls);



        ULocalPlayer* LocalPlayer = GetLocalPlayer();
        check(LocalPlayer);

        UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
        check(Subsystem);
        //Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(SplineMappingContext, 0);
    }
}

void AWallBuilderController::OnLeftMouseButtonPressed()
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Click ")));

    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    if (HitResult.bBlockingHit)
    {
        FVector ClickLocation = HitResult.Location;

        if (WallSplineActor[WallSplineActor.Num() - 1]->SplineComponent->GetNumberOfSplinePoints() == 1) {
            ShowText.ExecuteIfBound("Wall Creation started");
        }
        WallSplineActor[WallSplineActor.Num()-1]->OnLeftMouseButtonPressed(ClickLocation);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Click Location: %s"), *ClickLocation.ToString()));

    }
}

void AWallBuilderController::OnRightMouseButtonPressed()
{   
    if(WallSplineActor.Num()>0){
        ShowText.ExecuteIfBound("Wall Creation Completed");

    }
    WallSplineActor.Add(NewObject<AWallSpline>(this));
    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Now you can create new wall segment"));
}

void AWallBuilderController::UndoWall() {
    int LastIndex = WallSplineActor[WallSplineActor.Num() - 1]->SplineComponent->GetNumberOfSplinePoints() - 1;
    if (LastIndex > 0) {
        //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Undo Logic Triggered"));
        WallSplineActor[WallSplineActor.Num() - 1]->Undo();
    }
    else {
        //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("No More Points To Undo"));
        ShowText.ExecuteIfBound("No More Points");
        WallSplineActor[WallSplineActor.Num() - 1]->SplineComponent->ClearSplinePoints();
    }
}
void AWallBuilderController::DeleteWall() {
    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Delete Logic Triggered"));
    WallSplineActor[WallSplineActor.Num() - 1]->Destroy();
    ShowText.ExecuteIfBound("Deleted Current Wall");
    //WallSplineActor[WallSplineActor.Num() - 1]->SplineComponent->ClearSplinePoints();
    WallSplineActor.RemoveAt(WallSplineActor.Num() - 1);
    if (WallSplineActor.Num() == 0) {
        WallSplineActor.Add(NewObject<AWallSpline>(this));
    }

}
void AWallBuilderController::ClearWalls() {
    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, TEXT("Delete All Triggered"));
    for (int i = 0; i < WallSplineActor.Num(); i++) {
        WallSplineActor[i]->Destroy();
        //WallSplineActor[i]->SplineComponent->ClearSplinePoints();
    }
    ShowText.ExecuteIfBound("Deleted All Walls");
    WallSplineActor.Empty();
    WallSplineActor.Add(NewObject<AWallSpline>(this));
}
