// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackSystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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
		[&]() {
			if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
			{
				UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(AttackMontages[AttackIndex]);
			}
		},
		0.1f,
		false);
}

void UAttackSystemComponent::DamageActor(AActor* ActorToDamage)
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		UGameplayStatics::ApplyDamage(ActorToDamage, 10.f, Character->GetController(), Character, UDamageType::StaticClass());
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), SwordHitCameraShake, ActorToDamage->GetActorLocation(), 0.f, 2000.f);
		if (ACharacter* CharacterToDamage = Cast<ACharacter>(ActorToDamage))
		{
			FVector Force = Character->GetActorForwardVector() * 700.f;
			CharacterToDamage->LaunchCharacter(Force, false, false);
		}
	}
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

	SwordStartArrowReference = nullptr;
	SwordEndArrowReference = nullptr;
	StabKickArrowReference = nullptr;
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

void UAttackSystemComponent::AttachAttackArrows(UArrowComponent* SwordStartArrow, UArrowComponent* SwordEndArrow, UArrowComponent* StabKickArrow)
{
	SwordStartArrowReference = SwordStartArrow;
	SwordEndArrowReference = SwordEndArrow;
	StabKickArrowReference = StabKickArrow;
}

void UAttackSystemComponent::StartSwordTrace()
{
	if (!SwordStartArrowReference || !SwordEndArrowReference)
	{
		return;
	}

	// Sword Trace
	GetWorld()->GetTimerManager().SetTimer(
		SwordTraceTimerHandle,
		[&]() {
			FVector	   TraceStart = SwordStartArrowReference->GetComponentLocation();
			FVector	   TraceEnd = SwordEndArrowReference->GetComponentLocation();
			FHitResult HitResult;
			bool	   bFound = UKismetSystemLibrary::SphereTraceSingle(
				  GetWorld(), TraceStart, TraceEnd, 12.f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
				  TArray<AActor*>{ GetOwner() }, EDrawDebugTrace::Type::ForDuration, HitResult, true,
				  FLinearColor::Red, FLinearColor::Green, 0.2f);

			if (bFound && HitResult.GetActor()->ActorHasTag(TEXT("Damageable")) && !DamagedActor.Contains(HitResult.GetActor()))
			{
				DamagedActor.Add(HitResult.GetActor());
				DamageActor(HitResult.GetActor());
			}
		},
		0.001f,
		true);
}

void UAttackSystemComponent::EndSwordTrace()
{
	DamagedActor.Empty();
	GetWorld()->GetTimerManager().ClearTimer(SwordTraceTimerHandle);
}

void UAttackSystemComponent::StartStabKickTrace()
{
	if (!StabKickArrowReference)
	{
		return;
	}

	FVector	   TraceStart = StabKickArrowReference->GetComponentLocation();
	FHitResult HitResult;
	bool	   bFound = UKismetSystemLibrary::SphereTraceSingle(
		  GetWorld(), TraceStart, TraceStart, 25.f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
		  TArray<AActor*>{ GetOwner() }, EDrawDebugTrace::Type::ForDuration, HitResult, true,
		  FLinearColor::Red, FLinearColor::Green, 0.2f);

	if (bFound && HitResult.GetActor()->ActorHasTag(TEXT("Damageable")))
	{
		DamageActor(HitResult.GetActor());
	}
}
