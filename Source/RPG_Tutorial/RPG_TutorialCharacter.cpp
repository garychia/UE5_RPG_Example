// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_TutorialCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PlayerHUD.h"
#include "PlayerStats.h"
#include "MotionWarpingComponent.h"
#include "Animation/AnimMontage.h"
#include "AssassinatableInterface.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ARPG_TutorialCharacter

void ARPG_TutorialCharacter::Die()
{
	DisableInput(Cast<APlayerController>(GetController()));
	GetMesh()->SetSimulatePhysics(true);
}

bool ARPG_TutorialCharacter::CanSprint()
{
	return PlayerStats->HasEnoughStamina(SprintStaminaConsumption) && GetVelocity().Size() != 0.f;
}

bool ARPG_TutorialCharacter::FindVaultLocations(FVector& OutStartLocation, FVector& OutMiddleLocation, FVector& OutEndLocation)
{
	FVector ActorLocation = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector TraceStart, TraceEnd;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore{ this };
	bool bFound = false;

	for (int32 i = 0; i < 3 && !bFound; i++)
	{
		TraceStart = ActorLocation + FVector(0, 0, 30 * i);
		TraceEnd = TraceStart + Forward * 180;

		bFound = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(), TraceStart, TraceEnd, .5f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
			ActorsToIgnore, EDrawDebugTrace::Type::ForDuration, HitResult, true
		);
	}

	if (!bFound)
	{
		return false;
	}

	FVector HitLocation = HitResult.Location;
	bool bStartLocationFound = false, EndLocationFound = false;

	for (int32 i = 0; i < 6; i++)
	{
		TraceStart = HitLocation + FVector(0, 0, 100) + Forward * 50 * i;
		TraceEnd = TraceStart - FVector(0, 0, 100);

		bFound = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(), TraceStart, TraceEnd, 10.f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
			ActorsToIgnore, EDrawDebugTrace::Type::ForDuration, HitResult, true
		);

		if (bFound)
		{
			if (i == 0)
			{
				OutStartLocation = HitResult.Location;
				bStartLocationFound = true;
			}
			OutMiddleLocation = HitResult.Location;
		}
		else
		{
			TraceStart = HitResult.TraceStart + Forward * 80;
			TraceEnd = TraceStart - FVector(0, 0, 1000);
			bFound = UKismetSystemLibrary::LineTraceSingle(
				GetWorld(), TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
				EDrawDebugTrace::Type::ForDuration, HitResult, true, FLinearColor::Blue, FLinearColor::Green
			);

			if (bFound)
			{
				OutEndLocation = HitResult.Location;
				EndLocationFound = true;
				break;
			}
		}
	}

	return bStartLocationFound && EndLocationFound;
}

void ARPG_TutorialCharacter::TargetArmLengthTimelineProgress(float Amount)
{
	GetCameraBoom()->TargetArmLength = FMath::Lerp(CameraBoomTargetArmLength, CrouchedCameraBoomTargetArmLength, Amount);
}

ARPG_TutorialCharacter::ARPG_TutorialCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	PlayerStats = CreateDefaultSubobject<UPlayerStats>(TEXT("PlayerStats"));

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	PlayerHUD = nullptr;

	MaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CrouchedMaxSpeed = 350.f;
	SprintMaxSpeed = 750.f;

	CameraBoomTargetArmLength = GetCameraBoom()->TargetArmLength;
	CrouchedCameraBoomTargetArmLength = 550.f;

	SprintStaminaConsumption = 1.f;
}

void ARPG_TutorialCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UPlayerHUD>(Cast<APlayerController>(GetController()), PlayerHUDClass);
	}
	
	if (PlayerHUD)
	{
		PlayerStats->SetPlayerHUD(PlayerHUD);
		PlayerStats->SetPlayer(this);
		PlayerStats->OnReachZeroHealth.AddDynamic(this, &ARPG_TutorialCharacter::Die);
		PlayerHUD->AddToViewport();
	}

	if (TargetArmLengthCurve)
	{
		FOnTimelineFloat OnTimelineFloat;
		OnTimelineFloat.BindUFunction(this, FName("TargetArmLengthTimelineProgress"));
		TargetArmLengthTimeline.AddInterpFloat(TargetArmLengthCurve, OnTimelineFloat);
		TargetArmLengthTimeline.SetLooping(false);
	}
}

void ARPG_TutorialCharacter::Tick(float DeltaTime)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(SprintTimerHandle))
	{
		PlayerStats->IncreaseStamina(SprintStaminaConsumption * DeltaTime);
	}
	TargetArmLengthTimeline.TickTimeline(DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPG_TutorialCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPG_TutorialCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPG_TutorialCharacter::Look);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ARPG_TutorialCharacter::Crouch);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ARPG_TutorialCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARPG_TutorialCharacter::SprintEnd);

		// Assassination
		EnhancedInputComponent->BindAction(AssassinationAction, ETriggerEvent::Started, this, &ARPG_TutorialCharacter::Assassinate);
	
		// Vault
		EnhancedInputComponent->BindAction(VaultAction, ETriggerEvent::Started, this, &ARPG_TutorialCharacter::Vault);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARPG_TutorialCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARPG_TutorialCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARPG_TutorialCharacter::Crouch(const FInputActionValue&)
{
	bCrouched = !bCrouched;
	if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent())) {
		MovementComponent->MaxWalkSpeed = bCrouched ? CrouchedMaxSpeed : MaxSpeed;
	}
	
	if (bCrouched)
	{
		TargetArmLengthTimeline.PlayFromStart();
	}
	else
	{
		TargetArmLengthTimeline.ReverseFromEnd();
	}
}

void ARPG_TutorialCharacter::SprintStart(const FInputActionValue&)
{
	if (!CanSprint())
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = SprintMaxSpeed;
	GetWorld()->GetTimerManager().SetTimer(
		SprintTimerHandle,
		[&]()
		{
			if (CanSprint())
			{
				PlayerStats->DecreaseStamina(SprintStaminaConsumption);
			}
			else
			{
				SprintEnd(FInputActionValue());
			}
		},
		0.25f,
		true
	);
}

void ARPG_TutorialCharacter::SprintEnd(const FInputActionValue&)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
}

void ARPG_TutorialCharacter::Assassinate(const FInputActionValue&)
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	AActor* ActorToAssassinate = nullptr;

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->Implements<UAssassinatableInterface>())
		{
			ActorToAssassinate = Actor;
			break;
		}
	}

	if (!ActorToAssassinate)
	{
		return;
	}

	FVector TargetLocation;
	FRotator TargetRotation;

	IAssassinatableInterface::Execute_GetAssassinated(ActorToAssassinate, TargetLocation, TargetRotation);

	if (AssassinationMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(AssassinationMontage);
	}

	MotionWarping->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("AssassinationWarp")), TargetLocation, TargetRotation);
}

void ARPG_TutorialCharacter::Vault(const FInputActionValue&)
{
	FVector StartLocation, MiddleLocation, EndLocation;
	
	if (!FindVaultLocations(StartLocation, MiddleLocation, EndLocation))
	{
		return;
	}

	FRotator ActorRotation = GetActorRotation();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	SetActorEnableCollision(false);
	MotionWarping->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("VaultStart")), StartLocation, ActorRotation);
	MotionWarping->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("VaultMiddle")), MiddleLocation, ActorRotation);
	MotionWarping->AddOrUpdateWarpTargetFromLocationAndRotation(FName(TEXT("VaultLand")), EndLocation, ActorRotation);
	if (VaultMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &ARPG_TutorialCharacter::FinalizeMontage);
		AnimInstance->Montage_Play(VaultMontage);
	}
}

void ARPG_TutorialCharacter::FinalizeMontage(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == VaultMontage)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		SetActorEnableCollision(true);
	}
}


void ARPG_TutorialCharacter::Jump()
{
	if (bCrouched) {
		Crouch(FInputActionValue());
	}

	ACharacter::Jump();
}
