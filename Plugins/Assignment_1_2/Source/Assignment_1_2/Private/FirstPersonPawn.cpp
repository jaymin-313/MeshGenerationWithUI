// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonPawn.h"
//#include "PawnPlayerController.h"
#include "ENhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "C:\Program Files\Epic Games\UE_5.2\Engine\Plugins\EnhancedInput\Source\EnhancedInput\Public\EnhancedInputComponent.h"
#include "C:\Program Files\Epic Games\UE_5.2\Engine\Plugins\EnhancedInput\Source\EnhancedInput\Public\EnhancedInputSubsystems.h"
#include "C:\Program Files\Epic Games\UE_5.2\Engine\Plugins\EnhancedInput\Source\EnhancedInput\Public\InputAction.h"
#include "C:\Program Files\Epic Games\UE_5.2\Engine\Plugins\EnhancedInput\Source\EnhancedInput\Public\InputMappingContext.h"
#include "C:\Program Files\Epic Games\UE_5.2\Engine\Plugins\EnhancedInput\Source\EnhancedInput\Public\InputModifiers.h"


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
AFirstPersonPawn::AFirstPersonPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Sphere);

	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Body);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Sphere);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	MoveScale = 1.f;
	RotateScale = 50.f;

	static ConstructorHelpers::FObjectFinder<UPawnAttributeAsset> AssetFinder(TEXT("/Script/Assignment_1_2.PawnAttributeAsset'/Assignment_1_2/FirstPersonAsset.FirstPersonAsset'"));
	if (AssetFinder.Succeeded())
	{
		FirstPersonAttributes = AssetFinder.Object;
	}
}

// Called when the game starts or when spawned
void AFirstPersonPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFirstPersonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

	RotateAction = NewObject<UInputAction>(this);
	RotateAction->ValueType = EInputActionValueType::Axis3D;
	MapKey(PawnMappingContext, RotateAction, EKeys::MouseY);
	MapKey(PawnMappingContext, RotateAction, EKeys::MouseX, false, true);


	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* FPC = Cast<APlayerController>(Controller);
	check(EIC);
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Move);
	EIC->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Rotate);
	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PawnMappingContext, 0);



	/*MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;
	PawnMappingContext->MapKey(MoveAction, EKeys::W);
	PawnMappingContext->MapKey(MoveAction, EKeys::W);
	FEnhancedActionKeyMapping& Mapping = PawnMappingContext->MapKey(MoveAction, EKeys::S);
	UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(this);
	Mapping.Modifiers.Add(Negate);*/


}

void AFirstPersonPawn::Move(const FInputActionValue& ActionValue)
{
	//FVector Input = ActionValue.Get<FInputActionValue::Axis3D>();
	//// UFloatingPawnMovement handles scaling this input based on the DeltaTime for this frame.
	//AddMovementInput(GetActorRotation().RotateVector(Input), MoveScale);

	FVector Input;
	Input.X = ActionValue.Get<FInputActionValue::Axis2D>().X;
	Input.Y = ActionValue.Get<FInputActionValue::Axis2D>().Y;
	FVector Input2 = GetActorRotation().RotateVector(Input);
	Input2.Z = 0;

	AddMovementInput(Input2, MoveScale);
}

void AFirstPersonPawn::Rotate(const FInputActionValue& ActionValue)
{


	FRotator Input(ActionValue[0], ActionValue[1], ActionValue[2]);
	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;
	Input += GetActorRotation();
	Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
	Input.Roll = 0;
	SetActorRotation(Input);
}

       