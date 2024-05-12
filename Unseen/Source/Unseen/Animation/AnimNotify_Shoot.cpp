// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_Shoot.h"

UAnimNotify_Shoot::UAnimNotify_Shoot()
{
}

FString UAnimNotify_Shoot::GetNotifyName_Implementation() const
{
	return FString();
}

void UAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
