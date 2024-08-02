// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_StabKickTrace.h"
#include "RPG_TutorialCharacter.h"

void UAnimNotify_StabKickTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ARPG_TutorialCharacter* Character = Cast<ARPG_TutorialCharacter>(MeshComp->GetOwner()))
	{
		Character->StabOrKick();
	}
}
