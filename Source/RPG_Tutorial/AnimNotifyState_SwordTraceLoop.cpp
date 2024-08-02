// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_SwordTraceLoop.h"
#include "RPG_TutorialCharacter.h"

void UAnimNotifyState_SwordTraceLoop::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (ARPG_TutorialCharacter* Character = Cast<ARPG_TutorialCharacter>(MeshComp->GetOwner()))
	{
		Character->EnableSword();
	}
}

void UAnimNotifyState_SwordTraceLoop::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ARPG_TutorialCharacter* Character = Cast<ARPG_TutorialCharacter>(MeshComp->GetOwner()))
	{
		Character->DisableSword();
	}
}
