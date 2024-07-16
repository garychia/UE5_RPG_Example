// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::UpdateHealthBar(float NewPercentage)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(NewPercentage);
	}
}

void UPlayerHUD::UpdateStaminaBar(float NewPercentage)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(NewPercentage);
	}
}
