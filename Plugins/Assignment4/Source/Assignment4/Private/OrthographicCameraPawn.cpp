// Fill out your copyright notice in the Description page of Project Settings.


#include "OrthographicCameraPawn.h"


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
// Sets default values
AOrthographicCameraPawn::AOrthographicCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 1000.0f;
	//SpringArm->TargetOffset = FVector(-520, 0, 540);
	SpringArm->SetupAttachment(Scene);
	SpringArm->bDoCollisionTest = false;

	//float Angle = FMath::RadiansToDegrees(atan(SpringArm->TargetOffset.Z / SpringArm->TargetArmLength));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Camera->SetRelativeRotation(FRotator(-90.0f, 0, 0));
	Camera->SetupAttachment(SpringArm,USpringArmComponent::SocketName);
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	MoveScale = 10.0f;
	ZoomScale = 100.0f;
}

// Called when the game starts or when spawned
void AOrthographicCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrthographicCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOrthographicCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrthographicCameraPawn::Move);
	EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AOrthographicCameraPawn::Zoom);
	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PawnMappingContext, 0);
}

void AOrthographicCameraPawn::Move(const FInputActionValue& ActionValue)
{
	FVector MovementValue = ActionValue.Get<FVector>();
	FVector DeltaLocation = FVector(MovementValue.X, MovementValue.Y, 0.0f) * MoveScale;
	AddActorLocalOffset(DeltaLocation, true);
}

void AOrthographicCameraPawn::Zoom(const FInputActionValue& ActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("zoom inout"));
	float ZoomValue = ActionValue.Get<float>();
	float NewOrthoWidth = Camera->OrthoWidth - (ZoomValue * ZoomScale);
	Camera->OrthoWidth = FMath::Clamp(NewOrthoWidth, 512.0f, 4096.0f); // Adjust these values to your desired min/max zoom levels
}

