// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.generated.h"


class UPlayerHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReachZeroHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReachZeroStaminaSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_TUTORIAL_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = Health)
	FReachZeroHealthSignature OnReachZeroHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float CurrentHealth;

	UPROPERTY(BlueprintAssignable, Category = Stamina)
	FReachZeroStaminaSignature OnReachZeroStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XP)
	float MaxXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XP)
	float CurrentXP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = XP)
	int Level;

	// Sets default values for this component's properties
	UPlayerStats();

private:
	UPlayerHUD* PlayerHUD;

	// Called when health-related values have been changed
	void ReflectChangedHealthValues();

	// Called when stamina-related values have been changed
	void ReflectChangedStaminaValues();

	// Called when the player is damanged
	UFUNCTION()
	void ReflectDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Health)
	void IncreaseMaxHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = Health)
	void DecreaseHealth(float Damage);

	UFUNCTION(BlueprintCallable, Category = Health)
	void IncreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = Stamina)
	void IncreaseMaxStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = Stamina)
	void DecreaseStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = Stamina)
	void IncreaseStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = Stamina)
	bool HasEnoughStamina(float AmountRequired) const { return CurrentStamina >= AmountRequired; }

	// Assigns the player's HUD to this PlayerStats
	void SetPlayerHUD(UPlayerHUD* HUD);

	// Assigns the player to this PlayerStats.
	void SetPlayer(AActor* Player);
};
