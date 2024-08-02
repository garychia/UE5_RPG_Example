// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SwordTraceLoop.generated.h"

/**
 *
 */
UCLASS()
class RPG_TUTORIAL_API UAnimNotifyState_SwordTraceLoop : public UAnimNotifyState
{
	GENERATED_BODY()

	void NotifyBegin(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase*					 Animation,
		float								 TotalDuration,
		const FAnimNotifyEventReference&	 EventReference) override;

	void NotifyEnd(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase*				   Animation,
		const FAnimNotifyEventReference&   EventReference) override;
};
