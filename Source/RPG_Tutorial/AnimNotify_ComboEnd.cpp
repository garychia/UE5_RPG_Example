// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ComboEnd.h"
#include "RPG_TutorialCharacter.h"

void UAnimNotify_ComboEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ARPG_TutorialCharacter* Character = Cast<ARPG_TutorialCharacter>(MeshComp->GetOwner()))
	{
		Character->StopAttack();
	}
}