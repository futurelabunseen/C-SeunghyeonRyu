// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ShootEnd.h"

void UAnimNotify_ShootEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnNotified.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}