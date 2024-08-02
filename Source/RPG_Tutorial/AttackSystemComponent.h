// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackSystemComponent.generated.h"

class UAnimMontage;
class UArrowComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_TUTORIAL_API UAttackSystemComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackMontages;

	// Actors that have been damaged.
	TSet<AActor*> DamagedActor;

	FCriticalSection CriticalSection;

	// The index of the current attack movement
	uint8 AttackIndex;

	// A handle to the timer that controls the frequency of the sword trace
	FTimerHandle SwordTraceTimerHandle;

	UArrowComponent* SwordStartArrowReference;

	UArrowComponent* SwordEndArrowReference;

	UArrowComponent* StabKickArrowReference;

	// Whether the player is attacking
	bool bIsAttacking;

	bool StartAttack();

	void PlayAnimation();

	void DamageActor(AActor* ActorToDamage);

public:
	// Sets default values for this component's properties
	UAttackSystemComponent();

	UFUNCTION(BlueprintCallable, Category = Attack)
	void Attack();

	UFUNCTION(BlueprintCallable, Category = Attack)
	void StopAttack();

	UFUNCTION(BlueprintCallable, Category = Attack)
	void PrepareAttackCombo();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AttachAttackArrows(UArrowComponent* SwordStartArrow, UArrowComponent* SwordEndArrow, UArrowComponent* StabKickArrow);

	void StartSwordTrace();

	void EndSwordTrace();

	void StartStabKickTrace();
};
