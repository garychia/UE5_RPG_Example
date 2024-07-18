// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::UpdateHealthBar(float NewHealth, float NewMaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(NewHealth / NewMaxHealth);
		HPTextBlock->SetText(FText::FromString(FString::SanitizeFloat(NewHealth)));
	}
}

void UPlayerHUD::UpdateStaminaBar(float NewStamina, float NewMaxStamina)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(NewStamina / NewMaxStamina);
		StaminaTextBlock->SetText(FText::FromString(FString::SanitizeFloat(NewStamina)));
	}
}
