// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../TLActionRoguelike.h"
#include "TLAttributeComponent.generated.h"

class AGL_PopupText;
class UDataTable;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UTLAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnMaxHealthChanged, AActor*, InstigatorActor, UTLAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnManaChanged, AActor*, InstigatorActor, UTLAttributeComponent*, OwningComp, float, NewMana, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UTLAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASLITE_API UTLAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	static UTLAttributeComponent* GetAttribute(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attribute", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	float GetAttackerPhysicalAttack(AActor* InsigatorActor);
	
	UFUNCTION()
	bool Kill(AActor* InstigatorActor);

	// Sets default values for this component's properties
	UTLAttributeComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float currentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float maxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float lowHealthThreashold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float currentMana;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float maxMana;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float physicalAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float abillityAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float AttackSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	float maxRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float RegenHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float RegenMana;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
		float PhysicalDef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
		float AbilityDef;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attribute")
		float Exp;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attribute")
		uint8 Level;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Team")
		uint8 Team;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<AGL_PopupText> ClassToSpawn;

	UPROPERTY(EditAnywhere)
		UDataTable* UnitDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
		FName DefaultUnitKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
		TMap<int32, FUnitLevelData> UnitLevelData;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	bool bShowPopupText;

	UFUNCTION(NetMulticast, Reliable) //ToDO: Mark this as Unrealiable when we move the 'state' our scharacter
	void MulticastHealthChange(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable) // Used for cosmetic changes only
	void MulticastManaChanged(AActor* InstigatorActor, float NewMana, float Delta);

	UFUNCTION(NetMulticast, Unreliable) // Used for cosmetic changes only
		void MulticastAttributeChanged(AActor* InstigatorActor, float NewAttribute, float Delta);

	// Called when the game starts
	virtual void BeginPlay() override;

public:	


	UPROPERTY(BlueprintAssignable)
		FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
		FOnManaChanged OnManaChanged;

	UPROPERTY(BlueprintAssignable)
		FOnMaxHealthChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category= "Attributes")
		FOnAttributeChanged OnAttributeChange;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		void InitAttribute();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsAlive() const { return currentHealth > 0.f;};

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsFullHealth() const { return currentHealth == maxHealth; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsFullMana() const { return currentMana == maxMana; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool HasEnoughMana(float delta) const { return (currentMana + delta) < 0.f; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetMaxHealth(){return maxHealth;};

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetCurrentHealth(){ return currentHealth;};

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetCurrentMana() { return currentMana; };
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetMaxMana() { return maxMana; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetRegenHealth() {return RegenHealth;};

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetRegenMana() { return RegenMana; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetPhysicalAttack() { return physicalAttack; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetAbilityAttack() { return abillityAttack; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetAttackSpeed() { return AttackSpeed; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetExp() { return Exp; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		uint8 GetLevel() { return Level; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool isHealthLow(){return currentHealth < lowHealthThreashold;};

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyHealthChange(AActor* InstigatorActor, float delta);
	
	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyMaxHealthChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyManaChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyManaMaxChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyPhysicalAttackChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyAbilityAttackChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplySpeedChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyAttackSpeedChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyPhyDefenceChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyAbilityDefChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyRegenHealthChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
		bool ApplyRegenManaChange(AActor* InstigatorActor, float delta);

	UFUNCTION(BlueprintPure, Category = "Teams")
	uint8 GetTeamID() { return Team; };

	UFUNCTION(BlueprintCallable, Category = "Teams")
	void SetTeamID(uint8 TeamId) { Team = TeamId; };

	UFUNCTION(BlueprintPure, Category = "Teams")
	bool IsFriendly(const AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void LevelUp();

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void AddExp(float AddExp);
};
