// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_Shoot.h"

void UAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}
