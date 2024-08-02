// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify_SwordComboNext.h"
#include "RPG_TutorialCharacter.h"

void UAnimNotify_SwordComboNext::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ARPG_TutorialCharacter* Character = Cast<ARPG_TutorialCharacter>(MeshComp->GetOwner()))
	{
		Character->PrepareAttackCombo();
	}
}
