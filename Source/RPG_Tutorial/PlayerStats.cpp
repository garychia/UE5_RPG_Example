// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStats.h"
#include "RPG_TutorialCharacter.h"
#include "PlayerHUD.h"


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

void UPlayerStats::ReflectChangedHealthValues()
{
	if (PlayerHUD)
	{
		PlayerHUD->UpdateHealthBar(CurrentHealth, MaxHealth);
	}
}

void UPlayerStats::ReflectChangedStaminaValues()
{
	if (PlayerHUD)
	{
		PlayerHUD->UpdateStaminaBar(CurrentStamina, MaxStamina);
	}
}

void UPlayerStats::ReflectDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	DecreaseHealth(Damage);
}

// Called when the game starts
void UPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	ReflectChangedHealthValues();
	ReflectChangedStaminaValues();
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
	
	ReflectChangedHealthValues();
}

void UPlayerStats::IncreaseMaxStamina(float Amount)
{
	check(Amount >= 0.f);
	MaxStamina += Amount;

	ReflectChangedStaminaValues();
}

void UPlayerStats::DecreaseHealth(float Damage)
{
	check(Damage >= 0.f);
	CurrentHealth = fmaxf(CurrentHealth - Damage, 0.f);

	ReflectChangedHealthValues();

	if (CurrentHealth == 0.f)
	{
		OnReachZeroHealth.Broadcast();
	}
}


void UPlayerStats::IncreaseHealth(float Amount) {
	check(Amount >= 0.f);
	CurrentHealth = fminf(CurrentHealth + Amount, MaxHealth);

	ReflectChangedHealthValues();
}

void UPlayerStats::DecreaseStamina(float Amount)
{
	check(Amount >= 0.f);
	CurrentStamina = fmaxf(CurrentStamina - Amount, 0.f);
	
	ReflectChangedStaminaValues();
	
	if (CurrentStamina == 0.f)
	{
		OnReachZeroStamina.Broadcast();
	}
}

void UPlayerStats::IncreaseStamina(float Amount)
{
	check(Amount >= 0.f);
	CurrentStamina = fminf(CurrentStamina + Amount, MaxStamina);

	ReflectChangedStaminaValues();
}

void UPlayerStats::SetPlayerHUD(UPlayerHUD* HUD)
{
	PlayerHUD = HUD;
	ReflectChangedHealthValues();
	ReflectChangedStaminaValues();
}

void UPlayerStats::SetPlayer(AActor* Player)
{
	Player->OnTakeAnyDamage.AddDynamic(this, &UPlayerStats::ReflectDamage);
}
