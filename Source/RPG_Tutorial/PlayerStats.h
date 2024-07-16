// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_TUTORIAL_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float CurrentHealth;

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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Health)
	void IncreaseMaxHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = Health)
	bool GetDamaged(float Damage);

	UFUNCTION(BlueprintCallable, Category = Health)
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable, Category = Stamina)
	void IncreaseMaxStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = Stamina)
	bool DecreaseStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = Stamina)
	void IncreaseStamina(float Amount);
};
