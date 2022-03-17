// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCalc/GLCalcDamage.h"
#include "Component/TLAttributeComponent.h"
#include "GameFramework/Actor.h"
#include <Component/TLActionComponent.h>
#include <Kismet/KismetMathLibrary.h>

UGLCalcDamage::UGLCalcDamage()
{

}

void UGLCalcDamage::Initialize(UTLActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

float UGLCalcDamage::CalcDamage_Implementation(float baseDamage)
{

	float modifiedDamage = 0.0f;
	modifiedDamage += baseDamage;
	modifiedDamage += -GetAttackTypeAttribute() - GetAddionalDamage();
	//if Owning Character has a Ability Tag that add/subtract/multiply/divide to the base damage 
	// 
	// modified Damage = Base Damage + Reduce damage + Boosted Damage
	// 
	//if so How many that are the same and is there More ability Tags that modify the base damage?



	return modifiedDamage;
}

AActor* UGLCalcDamage::GetOwner()
{
	return ActionComp->GetOwner();
}

FGameplayTagContainer UGLCalcDamage::GetOwnersGameplayTags()
{
	return ActionComp->ActiveGameplayTags;
}

UTLAttributeComponent* UGLCalcDamage::GetOwnerAttribute()
{
	return Cast<UTLAttributeComponent>(GetOwner()->GetComponentByClass(UTLAttributeComponent::StaticClass()));
}

float UGLCalcDamage::GetOwnerHealth()
{
	return GetOwnerAttribute()->GetCurrentHealth();
}

float UGLCalcDamage::GetAttackTypeAttribute()
{
	if (GetOwnersGameplayTags().HasTag(AbilityTag))
	{
		return	GetOwnerAttribute()->GetAbilityAttack();
	}

	return GetOwnerAttribute()->GetPhysicalAttack();
}

float UGLCalcDamage::GetAddionalDamage_Implementation()
{
	float additionalDmg = 0;
	TArray<FGameplayTag> GameplayTags;
	GetOwnersGameplayTags().GetGameplayTagArray(GameplayTags);

	for (FGameplayTag GPTag : GameplayTags)
	{
		if (BoostedTags.HasTag(GPTag))
		{
			// Add More Damage;
			additionalDmg = UKismetMathLibrary::FClamp(additionalDmg + 10, 0, 9999);
		}

		if (ReduceTags.HasTag(GPTag))
		{
			//Subtract Damage;
			additionalDmg = UKismetMathLibrary::FClamp(additionalDmg - 10, 0, 9999);
		}
	}
	return additionalDmg;
}