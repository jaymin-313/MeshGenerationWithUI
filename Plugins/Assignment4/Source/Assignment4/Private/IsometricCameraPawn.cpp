// Fill out your copyright notice in the Description page of Project Settings.


#include "IsometricCameraPawn.h"

// Sets default values
AIsometricCameraPawn::AIsometricCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	SpringArmComponent->SetRelativeRotation(FRotator(-45.0, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 2000.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AIsometricCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIsometricCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentYaw = SpringArmComponent->GetRelativeRotation().Yaw;
	float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, RotationSpeed);

	FRotator NewRelativeRotation = FRotator(SpringArmComponent->GetRelativeRotation().Pitch, NewYaw, SpringArmComponent->GetRelativeRotation().Roll);
	SpringArmComponent->SetRelativeRotation(NewRelativeRotation);


}

// Called to bind functionality to input
void AIsometricCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("RotateClockwise", IE_Pressed, this, &AIsometricCameraPawn::RotateCameraClockwise);
	PlayerInputComponent->BindAction("RotateCounterClockwise", IE_Pressed, this, &AIsometricCameraPawn::RotateCameraCounterClockwise);
	PlayerInputComponent->BindAxis("ZoomInOut", this, &AIsometricCameraPawn::Zoom);
}

void AIsometricCameraPawn::RotateCameraCounterClockwise() {
	//SpringArmComponent->AddRelativeRotation(FRotator(0.0f, RotationAngleIncrement, 0.0f));
	TargetYaw += RotationAngleIncrement;
	if (TargetYaw > 180.0f) {
		TargetYaw -= 360.0f;
	}
}
void AIsometricCameraPawn::RotateCameraClockwise() {
	//SpringArmComponent->AddRelativeRotation(FRotator(0.0f, -RotationAngleIncrement, 0.0f));

	TargetYaw -= RotationAngleIncrement;

	if (TargetYaw < -180.0f) {
		TargetYaw += 360.0f;
	}

}
void AIsometricCameraPawn::MoveCameraToLocation(FVector InLocation)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("moved to new location"));
	//normal working logic
	//FVector DirectionToFocus = (InLocation - GetActorLocation()).GetSafeNormal();

	//// Set the end of the SpringArm to the FocusLocation
	//FVector NewSpringArmEnd = InLocation;

	//// Set the new location of the SpringArmComponent to match the NewSpringArmEnd
	//FVector NewSpringArmLocation = NewSpringArmEnd - DirectionToFocus * SpringArmComponent->TargetArmLength;

	//// Set the target location to the SpringArm end
	//FVector TargetLocation = NewSpringArmEnd;

	//// Set the IsometricPawn's location to the target location
	//SetActorLocation(TargetLocation);

	//// Set the new location of the SpringArmComponent

	//SpringArmComponent->SetWorldLocation(NewSpringArmLocation);

	// Set the end of the SpringArm to the provided location vector
	FVector NewSpringArmEnd = InLocation;

	// Calculate the direction from the current IsometricPawn location to the new SpringArm end
	FVector DirectionToNewSpringArmEnd = NewSpringArmEnd - GetActorLocation();
	DirectionToNewSpringArmEnd.Normalize();

	// Set the new IsometricPawn location to align the SpringArm's end with the provided location
	FVector NewIsometricPawnLocation = NewSpringArmEnd - DirectionToNewSpringArmEnd * SpringArmComponent->TargetArmLength;
	SetActorLocation(NewIsometricPawnLocation);

	// Set the new SpringArm's location to match the SpringArm's end
	SpringArmComponent->SetWorldLocation(NewIsometricPawnLocation);
}
void AIsometricCameraPawn::Zoom(float AxisValue) {

	float ZoomDelta = AxisValue * 50.0f;
	float ArmLength = SpringArmComponent->TargetArmLength;
	float NewArmLength = FMath::Clamp(ArmLength - ZoomDelta, 400.0f, 2000.0f);
	SpringArmComponent->TargetArmLength = NewArmLength;
}
