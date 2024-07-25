// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSystemComponent.h"
#include "GameFramework/Character.h"

bool UAttackSystemComponent::StartAttack()
{
	bool bCanStart = true;

	CriticalSection.Lock();

	if (bIsAttacking)
	{
		bCanStart = false;
	}
	else
	{
		bIsAttacking = true;
	}

	CriticalSection.Unlock();

	return bCanStart;
}

void UAttackSystemComponent::PlayAnimation()
{
	if (AttackMontages.IsEmpty())
	{
		return;
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[&]()
		{
			if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
			{
				UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(AttackMontages[AttackIndex]);
			}
		},
		0.1f,
		false
	);
}

void UAttackSystemComponent::Attack()
{
	if (!StartAttack())
	{
		return;
	}

	PlayAnimation();
}

void UAttackSystemComponent::StopAttack()
{
	CriticalSection.Lock();
	AttackIndex = 0;
	bIsAttacking = false;
	CriticalSection.Unlock();
}

void UAttackSystemComponent::PrepareAttackCombo()
{
	CriticalSection.Lock();
	bIsAttacking = false;
	if (!AttackMontages.IsEmpty())
	{
		AttackIndex = (AttackIndex + 1) % AttackMontages.Num();
	}
	CriticalSection.Unlock();
}

// Sets default values for this component's properties
UAttackSystemComponent::UAttackSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AttackIndex = 0;
	bIsAttacking = false;
}


// Called when the game starts
void UAttackSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttackSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

