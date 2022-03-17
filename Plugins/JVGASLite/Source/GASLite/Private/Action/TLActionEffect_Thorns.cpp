// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/TLActionEffect_Thorns.h"
#include "Component/TLAttributeComponent.h"
#include "Library/TLGameplayFunctionLibrary.h"
#include "Component/TLActionComponent.h"
#include "Action/TLAction.h"


UTLActionEffect_Thorns::UTLActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	Duration = 0.0f;
	Period = 0.0f;
}

void UTLActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Start listening
	UTLAttributeComponent* Attributes = UTLAttributeComponent::GetAttribute(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &UTLActionEffect_Thorns::OnHealthChanged);
	}

}

void UTLActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop listening
	UTLAttributeComponent* Attributes = UTLAttributeComponent::GetAttribute(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &UTLActionEffect_Thorns::OnHealthChanged);
	}

}

void UTLActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHeatlh, float Detla)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// Damage Only
	if (Detla < 0.0f && OwningActor != InstigatorActor)
	{
		// Round to nearest to avoid 'ugly' damage numbers and tiny reflections
		int32 ReflectedAmount = FMath::RoundToInt(Detla * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// Flip to positive, so we don't end up healing ourselves when passed into damage
		ReflectedAmount = -FMath::Abs(ReflectedAmount);

		// Return damage sender...
		UTLGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}

