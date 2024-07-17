// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::UpdateHealthBar(float NewHealth, float NewMaxHealth)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(NewHealth / NewMaxHealth);
	}
}

void UPlayerHUD::UpdateStaminaBar(float NewStamina, float NewMaxStamina)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(NewStamina / NewMaxStamina);
	}
}
