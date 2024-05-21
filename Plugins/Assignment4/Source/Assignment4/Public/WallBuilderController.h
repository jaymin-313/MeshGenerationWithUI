// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WallSpline.h"

#include "GameFramework/PlayerController.h"
#include "Components/SplineComponent.h"
#include "WallBuilderController.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FShowTextDelegate, FString);
UCLASS()
class ASSIGNMENT4_API AWallBuilderController : public APlayerController
{
	GENERATED_BODY()
public:

    //void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key, bool bNegate, bool bSwizzle, EInputAxisSwizzle SwizzleOrder);

    AWallBuilderController();
    // Enable the cursor and set input mode to game and UI
    UFUNCTION(BlueprintCallable)
    void EnableCursor();

    void BeginPlay();

    // Disable the cursor and set input mode to game only
    UFUNCTION(BlueprintCallable)
    void DisableCursor();

    // Input bindings for left mouse button press/release
    virtual void SetupInputComponent() override;
    void OnLeftMouseButtonPressed();
    void OnRightMouseButtonPressed();

    void UndoWall();

    void DeleteWall();

    void ClearWalls();

    UPROPERTY()
    class UInputMappingContext* SplineMappingContext;

    /** Action to update location. */
    UPROPERTY()
    class UInputAction* LeftClickAction;

    UPROPERTY()
    class UInputAction* RightClickAction;

    UPROPERTY()
    class UInputAction* DeleteAction;

    UPROPERTY()
    class UInputAction* ClearAction;

    UPROPERTY()
    class UInputAction* UndoAction;
    FShowTextDelegate ShowText;

    UFUNCTION(BlueprintImplementableEvent)
    void ShowWallWidget(const FString& NewText);
private:
    TArray<AWallSpline*> WallSplineActor;
};
