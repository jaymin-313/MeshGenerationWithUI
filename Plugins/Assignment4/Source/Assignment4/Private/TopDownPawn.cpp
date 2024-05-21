// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
static void MapKey(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
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
ATopDownPawn::ATopDownPawn()
{

	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->TargetArmLength = -211.599960;
	SpringArm->TargetOffset = FVector(-520, 0, 540);
	SpringArm->SetupAttachment(Scene);
	SpringArm->bDoCollisionTest = false;

	float Angle = FMath::RadiansToDegrees(atan(SpringArm->TargetOffset.Z / SpringArm->TargetArmLength));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(-Angle, 0, 0));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	MoveScale = 1.0f;
	ZoomScale = 100.0f;
}

void ATopDownPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATopDownPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PawnMappingContext = NewObject<UInputMappingContext>(this);
	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;
	MapKey(PawnMappingContext, MoveAction, EKeys::W);
	MapKey(PawnMappingContext, MoveAction, EKeys::S, true);
	MapKey(PawnMappingContext, MoveAction, EKeys::A, true, true);
	MapKey(PawnMappingContext, MoveAction, EKeys::D, false, true);
	MapKey(PawnMappingContext, MoveAction, EKeys::SpaceBar, false, true, EInputAxisSwizzle::ZYX);
	MapKey(PawnMappingContext, MoveAction, EKeys::LeftShift, true, true, EInputAxisSwizzle::ZYX);

	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;
	MapKey(PawnMappingContext, ZoomAction, EKeys::MouseWheelAxis);


	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* FPC = Cast<APlayerController>(Controller);
	check(EIC);
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Move);
	EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ATopDownPawn::Zoom);
	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	//Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PawnMappingContext, 0);
}

void ATopDownPawn::Move(const FInputActionValue& ActionValue)
{
	FVector Input;
	Input.X = ActionValue.Get<FInputActionValue::Axis2D>().X;
	Input.Y = ActionValue.Get<FInputActionValue::Axis2D>().Y;
	FVector Input2 = GetActorRotation().RotateVector(Input);
	Input2.Z = 0;

	AddMovementInput(Input2, MoveScale);
}

void ATopDownPawn::Zoom(const FInputActionValue& ActionValue)
{
	float Value = ActionValue.Get<float>();

	float TargetArmLength = SpringArm->TargetArmLength;
	float TargetOffsetZ = SpringArm->TargetOffset.Z;

	if (TargetArmLength >= -460) {
		SpringArm->TargetOffset.Z -= Value * 20;
	}
	if (TargetOffsetZ <= 1000) {
		SpringArm->TargetArmLength -= Value * 9.2;
	}

	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, -500, -0);
	SpringArm->TargetOffset.Z = FMath::Clamp(SpringArm->TargetOffset.Z, 0, 5000);

	FString FloatAsString = FString::SanitizeFloat(SpringArm->TargetArmLength);
	FString DebugMessage = FString::Printf(TEXT("MyFloatValue: %s"), *FloatAsString);

	FloatAsString = FString::SanitizeFloat(SpringArm->TargetOffset.Z);
	DebugMessage = FString::Printf(TEXT("MyFloatValue: %s"), *FloatAsString);

	float Angle;
	if (SpringArm->TargetArmLength == 0) {
		Angle = -90;
	}
	else {
		Angle = FMath::RadiansToDegrees(atan(SpringArm->TargetOffset.Z / SpringArm->TargetArmLength));
		Angle = FMath::Clamp(Angle, -90, 0);
	}

	if (Angle < -90) {
		Angle = -90;
	}
	if (Angle > 0) {
		Angle = 0;
	}

	FloatAsString = FString::SanitizeFloat(Angle);
	DebugMessage = FString::Printf(TEXT("MyFloatValue: %s"), *FloatAsString);

	Camera->SetRelativeRotation(FRotator(Angle, 0, 0));

}



