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
	UProgressBar* XPBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StaminaTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* XPTextBlock;

	void UpdateHealthBar(float NewHealth, float NewMaxHealth);

	void UpdateStaminaBar(float NewStamina, float NewMaxStamina);

	void UpdateXPBar(int32 CurrentXP, int32 MaxXP);

	void UpdateLevel(int32 Level);
};
