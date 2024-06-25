// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageWidgetActor.h"
#include "UI/AttackDamageWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

void ADamageWidgetActor::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.0f);
}

// Sets default values
ADamageWidgetActor::ADamageWidgetActor()
{
	DamageComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Damage"));
	static ConstructorHelpers::FClassFinder<UAttackDamageWidget> DamageRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WB_Damage.WB_Damage_C'"));

	FVector RelativeLocation(40.f, 0.f, 0.f);
	FRotator RelativeRotation(0.f, -90.f, 0.f);

	DamageComponent->SetRelativeLocation(RelativeLocation);
	DamageComponent->SetRelativeRotation(RelativeRotation);

	if (DamageRef.Class)
	{
		DamageComponent->SetWidgetSpace(EWidgetSpace::Screen);
		DamageComponent->SetDrawSize(FVector2D(250.0f, 250.0f));
		DamageComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DamageComponent->SetWidgetClass(DamageRef.Class);
	}


}

void ADamageWidgetActor::SetDamage(const float& InDamage, FLinearColor Color)
{
	ShowDamage(InDamage, Color);
}

void ADamageWidgetActor::ShowDamage(float InDamage, FLinearColor Color)
{
	UAttackDamageWidget* Widget = Cast<UAttackDamageWidget>(DamageComponent->GetWidget());

	if (Widget)
	{
		Widget->SetDamage(InDamage, Color);
	}
}

