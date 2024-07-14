// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssassinatableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UAssassinatableInterface : public UInterface
{
	GENERATED_BODY()
};

class IAssassinatableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Interaction)
	void GetAssassinated(FVector& Location, FRotator& Rotation);
};
