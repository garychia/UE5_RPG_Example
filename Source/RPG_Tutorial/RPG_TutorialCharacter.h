// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/TimelineComponent.h"
#include "RPG_TutorialCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMotionWarpingComponent;
class UAttackSystemComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UPlayerHUD;
class UPlayerStats;
class UAnimMontage;
class UArrowComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ARPG_TutorialCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	UAttackSystemComponent* AttackSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SwordMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sword, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SwordStartArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sword, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SwordEndArrow;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Assassination Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AssassinationAction;

	/** Vault Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VaultAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AssassinationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* VaultMontage;

	/** Player HUD Widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPlayerHUD> PlayerHUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	UPlayerStats* PlayerStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MotionWarping, meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarping;

	UPlayerHUD* PlayerHUD;

	FTimeline TargetArmLengthTimeline;

	UPROPERTY(EditAnywhere, Category = Timeline)
	UCurveFloat* TargetArmLengthCurve;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Speed, meta = (AllowPrivateAccess = "true"))
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed, meta = (AllowPrivateAccess = "true"))
	float CrouchedMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed, meta = (AllowPrivateAccess = "true"))
	float SprintMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CameraBoomTargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CrouchedCameraBoomTargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sprint, meta = (AllowPrivateAccess = "true"))
	float SprintStaminaConsumption;

	/** Whether the character is crouched */
	UPROPERTY(BlueprintReadOnly, Category = Variables, meta = (AllowPrivateAccess = "true"))
	bool bCrouched;

	FTimerHandle SprintTimerHandle;

	// A handle to the timer that controls the frequency of the sword trace
	FTimerHandle SwordTraceTimerHandle;

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void Die();

	UFUNCTION(BlueprintCallable, Category = Sprint)
	bool CanSprint();

	UFUNCTION(BlueprintCallable, Category = Sprint)
	bool FindVaultLocations(FVector& OutStartLocation, FVector& OutMiddleLocation, FVector& OutEndLocation);

	UFUNCTION()
	void TargetArmLengthTimelineProgress(float Amount);

	UFUNCTION()
	void FinalizeMontage(UAnimMontage* Montage, bool bInterrupted);

public:
	ARPG_TutorialCharacter();
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for crouching input */
	void Crouch(const FInputActionValue&);

	void SprintStart(const FInputActionValue&);

	void SprintEnd(const FInputActionValue&);

	void Assassinate(const FInputActionValue&);

	void Vault(const FInputActionValue&);

	void Attack(const FInputActionValue&);
	
	void Jump() override;
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void PrepareAttackCombo();

	void StopAttack();
};

