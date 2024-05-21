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

	FRotator CurrentRotation = SpringArmComponent->GetRelativeRotation();
	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll);
	SpringArmComponent->SetRelativeRotation(FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime));


}
void AIsometricCameraPawn::KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
	bool bNegate = false,
	bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{
	FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
	UObject* Outer = InputMappingContext->GetOuter();

	if (bNegate)
	{
		UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
		Mapping.Modifiers.Add(Negate);
	}

	if (bSwizzle)
	{
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
		Swizzle->Order = SwizzleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}
// Called to bind functionality to input
void AIsometricCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PawnMappingContext = NewObject<UInputMappingContext>(this);

	RotateClockwise = NewObject<UInputAction>(this);
	RotateClockwise->ValueType = EInputActionValueType::Axis3D;
	KeyMap(PawnMappingContext, RotateClockwise, EKeys::E);


	RotateCounterClockwise = NewObject<UInputAction>(this);
	RotateCounterClockwise->ValueType = EInputActionValueType::Axis3D;
	KeyMap(PawnMappingContext, RotateCounterClockwise, EKeys::Q);

	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;
	KeyMap(PawnMappingContext, ZoomAction, EKeys::MouseWheelAxis);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	APlayerController* FPC = Cast<APlayerController>(Controller);

	check(EIC && FPC);

	EIC->BindAction(RotateClockwise, ETriggerEvent::Completed, this, &AIsometricCameraPawn::RotateCameraClockwise);
	EIC->BindAction(RotateCounterClockwise, ETriggerEvent::Completed, this, &AIsometricCameraPawn::RotateCameraCounterClockwise);
	EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AIsometricCameraPawn::Zoom);
	//EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AIsometricCameraPawn::Zoom);

	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PawnMappingContext, 0);

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

void AIsometricCameraPawn::Zoom(const FInputActionValue& ActionValue) {

	float ZoomDelta = ActionValue.Get<float>() * 50.0f;
	float ArmLength = SpringArmComponent->TargetArmLength;
	float NewArmLength = FMath::Clamp(ArmLength - ZoomDelta, 800.0f, 2000.0f);
	SpringArmComponent->TargetArmLength = NewArmLength;
}