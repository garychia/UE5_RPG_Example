// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStats.h"

// Sets default values for this component's properties
UPlayerStats::UPlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Initialize class members.
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	MaxStamina = 100.f;
	CurrentStamina = MaxStamina;

	MaxXP = 100.f;
	CurrentXP = 0.f;

	Level = 0;
}


// Called when the game starts
void UPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerStats::IncreaseMaxHealth(float Amount)
{
	check(Amount >= 0.f);
	MaxHealth += Amount;
}

void UPlayerStats::IncreaseMaxStamina(float Amount)
{
	check(Amount >= 0.f);
	MaxStamina += Amount;
}

bool UPlayerStats::GetDamaged(float Damage)
{
	check(Damage >= 0.f);
	CurrentHealth = fmaxf(CurrentHealth - Damage, 0.f);
	return CurrentHealth == 0.f;
}


void UPlayerStats::Heal(float Amount) {
	check(Amount >= 0.f);
	CurrentHealth = fminf(CurrentHealth + Amount, MaxHealth);
}

bool UPlayerStats::DecreaseStamina(float Amount)
{
	check(Amount >= 0.f);
	CurrentStamina = fmaxf(CurrentStamina - Amount, 0.f);
	return CurrentStamina == 0.f;
}

void UPlayerStats::IncreaseStamina(float Amount)
{
	check(Amount >= 0.f);
	CurrentStamina = fminf(CurrentStamina + Amount, MaxStamina);
}
