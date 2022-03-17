// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TLAttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include <Kismet/GameplayStatics.h>
#include "UI/TLPopUpText.h"
#include "Net/UnrealNetwork.h"
#include "GL_PopupText.h"
#include "Game/GL_GameModeBase.h"
#include "GL_Unit.h"
#include "../TLActionRoguelike.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("tl.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);


// Sets default values for this component's properties
UTLAttributeComponent::UTLAttributeComponent()
{
	currentHealth = 100;
	maxHealth = 100;
	currentMana = 100;
	maxMana = 100;

	maxRotation = 30;
	
	Exp = 0.f;

	Level = 1;

	Team = 255;

	bShowPopupText = true;

	DefaultUnitKey = FName("Unit");

	SetIsReplicatedByDefault(true);
}



// Called when the game starts
void UTLAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UTLAttributeComponent::InitAttribute()
{
	TArray<FName> RowNames;
	FUnitData* UnitItem;
	//FUnitLevelData* UnitLevelItem;

	Level = 0;

	AGL_Unit* UnitActor = Cast<AGL_Unit>(GetOwner());

	if (UnitActor)
	{
		UnitItem = UnitDataTable->FindRow<FUnitData>(UnitActor->UnitKey, "");

		RowNames = UnitItem->LevelDataTable->GetRowNames();

	}
	else
	{
		UnitItem = UnitDataTable->FindRow<FUnitData>(DefaultUnitKey, "");

		RowNames = UnitItem->LevelDataTable->GetRowNames();
	}

	int32 count = 1;
	for (FName Row : RowNames)
	{
		FUnitLevelData* UnitLevelItem = UnitItem->LevelDataTable->FindRow<FUnitLevelData>(Row, "");

		FUnitLevelData data = FUnitLevelData(UnitLevelItem);

		UnitLevelData.Add(count, data);
		count++;
	}

	LevelUp();
}

void UTLAttributeComponent::LevelUp()
{
	FUnitLevelData UnitLevelItem = UnitLevelData.Find(Level + 1);

	ApplyMaxHealthChange(GetOwner(), UnitLevelItem.Health);
	ApplyHealthChange(GetOwner(), UnitLevelItem.Health);
	ApplyManaMaxChange(GetOwner(), UnitLevelItem.Mana);
	ApplyManaChange(GetOwner(), UnitLevelItem.Mana);
	ApplyPhysicalAttackChange(GetOwner(), UnitLevelItem.MeleeAttack);
	ApplyAbilityAttackChange(GetOwner(), UnitLevelItem.AbilityPower);
	ApplyRegenHealthChange(GetOwner(),UnitLevelItem.HPRegen);
	ApplyRegenManaChange(GetOwner(), UnitLevelItem.MPRegen);
	ApplyPhyDefenceChange(GetOwner(), UnitLevelItem.Defence);
	ApplyAbilityDefChange(GetOwner(), UnitLevelItem.AbilityPower);
	ApplySpeedChange(GetOwner(), UnitLevelItem.MovementSpeed);
	ApplyAttackSpeedChange(GetOwner(), UnitLevelItem.AttackSpeed);

		if (Exp >= UnitLevelItem.Exp)
		{
			Level++;

			LevelUp();
		}

}

void UTLAttributeComponent::AddExp(float AddExp)
{
	Exp += AddExp;

	LevelUp();
}

bool UTLAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	float oldHealth = currentHealth;
	float newHealth = FMath::Clamp<float>(currentHealth + delta, 0.f, maxHealth);

	float ActualDelta = newHealth - oldHealth;

	if (IsFriendly(InstigatorActor) && delta > 0)
	{
		currentHealth = newHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChange(InstigatorActor, currentHealth, ActualDelta);
		}

		return true;
	}

	if (!IsAlive())
	{
		return false;
	}

	if (delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		delta *= DamageMultiplier;
	}



	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		currentHealth = newHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChange(InstigatorActor, currentHealth, ActualDelta);
		}

		// if Died
		if (!IsAlive())
		{
			AGL_GameModeBase* GM = GetWorld()->GetAuthGameMode<AGL_GameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}

	}

	return ActualDelta != 0;
}

bool UTLAttributeComponent::ApplyMaxHealthChange(AActor* InstigatorActor, float delta)
{
	maxHealth += delta;

	OnMaxHealthChanged.Broadcast(InstigatorActor, this, maxHealth,delta);

	return true;
}


bool UTLAttributeComponent::ApplyManaChange(AActor* InstigatorActor, float delta)
{
	if (HasEnoughMana(delta))
	{
		//Not Enough Mana to spend to cast the ability
		return false;
	}

	float oldMana = currentMana;
	float newMana = FMath::Clamp<float>(currentMana + delta, 0.f, maxMana);

	float ActualDelta = newMana - oldMana;

	if (GetOwner()->HasAuthority())
	{
		currentMana = newMana;

		if (ActualDelta != 0.0f)
		{
			MulticastManaChanged(InstigatorActor,currentMana,ActualDelta);
		}
	}

	return ActualDelta != 0;
}

bool UTLAttributeComponent::ApplyManaMaxChange(AActor* InstigatorActor, float delta)
{
	float oldMaxMana = maxMana;
	float newMaxMana = FMath::Clamp<float>(maxMana + delta, 0.f, 5.f);

	float ActualDelta = newMaxMana - oldMaxMana;


	if (GetOwner()->HasAuthority())
	{
		maxMana += delta;

		if (ActualDelta != 0.0f)
		{
			MulticastAttributeChanged(InstigatorActor, maxMana, ActualDelta);
		}

	}

	return ActualDelta != 0;
}

bool UTLAttributeComponent::ApplyPhysicalAttackChange(AActor* InstigatorActor, float delta)
{
	float oldAbillityAtk = abillityAttack;
	float newAbillityAtk = FMath::Clamp<float>(abillityAttack + delta, 0.f, 5.f);

	float ActualDelta = newAbillityAtk - oldAbillityAtk;


	if (GetOwner()->HasAuthority())
	{
		abillityAttack += delta;

		if (ActualDelta != 0.0f)
		{
			MulticastAttributeChanged(InstigatorActor, abillityAttack, ActualDelta);
		}

	}

	return ActualDelta != 0;
}

bool UTLAttributeComponent::ApplyAbilityAttackChange(AActor* InstigatorActor, float delta)
{
	float oldAbillityAtk = abillityAttack;
	float newAbillityAtk = FMath::Clamp<float>(abillityAttack + delta, 0.f, 5.f);

	float ActualDelta = newAbillityAtk - oldAbillityAtk;


	if (GetOwner()->HasAuthority())
	{
		abillityAttack += delta;

		if (ActualDelta != 0.0f)
		{
			MulticastAttributeChanged(InstigatorActor, abillityAttack, ActualDelta);
		}

	}

	return ActualDelta != 0;
}

bool UTLAttributeComponent::ApplySpeedChange(AActor* InstigatorActor, float delta)
{
	float oldSpeed = Speed;
	float newSpeed = FMath::Clamp<float>(Speed + delta, 0.f, 5.f);

	float ActualDelta = newSpeed - oldSpeed;


	if (GetOwner()->HasAuthority())
	{
		Speed += delta;

		if (ActualDelta != 0.0f)
		{
			MulticastAttributeChanged(InstigatorActor, Speed, ActualDelta);
		}

	}

	return ActualDelta != 0;
}

bool UTLAttributeComponent::ApplyAttackSpeedChange(AActor* InstigatorActor, float delta)
{
	float oldAtkSpeed = AttackSpeed;
	float newAtkSpeed = FMath::Clamp<float>(AttackSpeed + delta, 0.f, 5.f);

	float ActualDelta = newAtkSpeed - oldAtkSpeed;


	if (GetOwner()->HasAuthority())
	{
		AttackSpeed += delta;

		if (ActualDelta != 0.0f)
		{
			MulticastAttributeChanged(InstigatorActor, AttackSpeed, ActualDelta);
		}

	}

	return ActualDelta != 0;
}

bool UTLAttributeComponent::ApplyPhyDefenceChange(AActor* InstigatorActor, float delta)
{
	float oldPhysicalDef = PhysicalDef;
	float newPhysicalDef = FMath::Clamp<float>(PhysicalDef + delta, 0.f, 5.f);

	float ActualDelta = newPhysicalDef - oldPhysicalDef;


	if (GetOwner()->HasAuthority())
	{
		PhysicalDef += delta;

		if (ActualDelta != 0.0f)
		{
			MulticastAttributeChanged(InstigatorActor, PhysicalDef, ActualDelta);
		}

	}

	return ActualDelta != 0;
}

bool UTLAttributeComponent::ApplyAbilityDefChange(AActor* InstigatorActor, float delta)
{
	float oldAbilityDef = AbilityDef;
	float newAbilityDef = FMath::Clamp<float>(AbilityDef + delta, 0.f, 5.f);

	float ActualDelta = newAbilityDef - oldAbilityDef;


	if (GetOwner()->HasAuthority())
	{
		AbilityDef += delta;

		if (ActualDelta != 0.0f)
		{
			MulticastAttributeChanged(InstigatorActor, AbilityDef, ActualDelta);
		}

	}

	return ActualDelta != 0;
}

bool UTLAttributeComponent::ApplyRegenHealthChange(AActor* InstigatorActor, float delta)
{
	float oldRegenHealth = RegenHealth;
	float newRegenHealth = FMath::Clamp<float>(RegenHealth + delta, 0.f, 9999.f);

	float ActualDelta = newRegenHealth - oldRegenHealth;


	if (GetOwner()->HasAuthority())
	{
		RegenHealth += delta;

		if (ActualDelta != 0.0f)
		{
			MulticastAttributeChanged(InstigatorActor, RegenHealth, ActualDelta);
		}

	}

	return true;
}

bool UTLAttributeComponent::ApplyRegenManaChange(AActor* InstigatorActor, float delta)
{
	float oldRegenMana = RegenMana;
	float newRegenMana = FMath::Clamp<float>(RegenMana + delta, 0.f, 9999.f);

	float ActualDelta = newRegenMana - oldRegenMana;


	if (GetOwner()->HasAuthority())
	{
		RegenMana += delta;

		if (ActualDelta != 0.0f)
		{
			MulticastAttributeChanged(InstigatorActor, RegenMana, ActualDelta);
		}

	}
	return ActualDelta != 0;
}

bool UTLAttributeComponent::IsFriendly(const AActor* OtherActor)
{
	if (OtherActor == nullptr)
	{
		return false;
	}
	UTLAttributeComponent* OtherAttributeComp = Cast<UTLAttributeComponent>(OtherActor->GetComponentByClass(UTLAttributeComponent::StaticClass()));


	if (OtherAttributeComp == nullptr)
	{
		//Assume Not Friendly
		return false;
	}

	return GetTeamID() == OtherAttributeComp->GetTeamID();

}

UTLAttributeComponent* UTLAttributeComponent::GetAttribute(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UTLAttributeComponent>(FromActor->GetComponentByClass(UTLAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UTLAttributeComponent::IsActorAlive(AActor* Actor)
{
	UTLAttributeComponent* AttributeComp = GetAttribute(Actor);

	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

float UTLAttributeComponent::GetAttackerPhysicalAttack(AActor* InsigatorActor)
{
	return 0.f;
}

bool UTLAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

void UTLAttributeComponent::MulticastHealthChange_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	if (bShowPopupText)
	{
		if (ensure(ClassToSpawn))
		{
			AActor* Actor = Cast<AActor>(GetOwner());
			FTransform ActorTransform = FTransform(FRotator(FMath::RandRange(-30, 30), FMath::RandRange(75, 120), FMath::RandRange(-30, 30)),
				Actor->GetActorLocation());
			AGL_PopupText* PopupText = Cast<AGL_PopupText>(UGameplayStatics::BeginDeferredActorSpawnFromClass(Actor->GetWorld(),
				ClassToSpawn,
				ActorTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, GetOwner()));
			PopupText->GivenDelta = Delta;

			UGameplayStatics::FinishSpawningActor((AActor*)PopupText, ActorTransform);
		}
	}

	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);


}

void UTLAttributeComponent::MulticastManaChanged_Implementation(AActor* InstigatorActor, float NewMana, float Delta)
{
	OnManaChanged.Broadcast(InstigatorActor, this, NewMana, Delta);
}

void UTLAttributeComponent::MulticastAttributeChanged_Implementation(AActor* InstigatorActor, float NewAttribute, float Delta)
{
	OnAttributeChange.Broadcast(InstigatorActor,this,NewAttribute,Delta);
}

void UTLAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTLAttributeComponent, currentHealth);
	DOREPLIFETIME(UTLAttributeComponent, maxHealth);
	DOREPLIFETIME(UTLAttributeComponent, lowHealthThreashold);
	DOREPLIFETIME(UTLAttributeComponent, currentMana);
	DOREPLIFETIME(UTLAttributeComponent, maxMana);
	DOREPLIFETIME(UTLAttributeComponent, physicalAttack);
	DOREPLIFETIME(UTLAttributeComponent, abillityAttack);
	DOREPLIFETIME(UTLAttributeComponent, Speed);
	DOREPLIFETIME(UTLAttributeComponent, AttackSpeed);
	DOREPLIFETIME(UTLAttributeComponent, RegenHealth);
	DOREPLIFETIME(UTLAttributeComponent, RegenMana);
	DOREPLIFETIME(UTLAttributeComponent, PhysicalDef);
	DOREPLIFETIME(UTLAttributeComponent, AbilityDef);
	DOREPLIFETIME(UTLAttributeComponent, Exp);
	DOREPLIFETIME(UTLAttributeComponent, Level);
	DOREPLIFETIME(UTLAttributeComponent, Team);
}

