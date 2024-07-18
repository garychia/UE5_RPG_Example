// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TUTORIAL_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StaminaTextBlock;

	void UpdateHealthBar(float NewHealth, float NewMaxHealth);

	void UpdateStaminaBar(float NewStamina, float NewMaxStamina);
};
