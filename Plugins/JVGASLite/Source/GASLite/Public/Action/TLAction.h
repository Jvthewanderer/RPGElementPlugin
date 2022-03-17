// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <GameplayTagContainer.h>
#include "TLAction.generated.h"

class UWorld;
class UTLActionComponent;
class UTexture2D;
class UGLCalcDamage;

UENUM()
enum EActionType
{
	E_Action UMETA(DisplayName = "Action"),
	E_Projectile UMETA(DisplayName = "Projectile"),
	E_Targeting UMETA(DisplayName = "Targeting")
};

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

	public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	bool bIsTargeting;

	UPROPERTY()
	AActor* Instigator;

	FActionRepData() {
		bIsRunning = false;
		bIsTargeting = false;
		Instigator = nullptr;
	}
};

/**
 * 
 */
UCLASS(Blueprintable)
class GASLITE_API UTLAction : public UObject
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
	UTLActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Calculate Damage")
		TSubclassOf<UGLCalcDamage> CalcDamageClass;

	UGLCalcDamage* CalcDamageObj;

	/* Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/*Action can only start if OwingAction has none of these Tags applied*/
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UPROPERTY(ReplicatedUsing = "OnRep_bTargeting")
	bool bTargeting = false;
	
	UPROPERTY(Replicated)
	float TimeStarted;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category= "Action")
	float RecastTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	float ManaCost = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	bool HoldCast = false;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float BaseDamage;

	UFUNCTION()
	void OnRep_RepData();

	UFUNCTION()
	void OnRep_bTargeting();

public:

	void Initialize(UTLActionComponent* NewActionComp);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetTimeRemaining() const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintPure, Category = "Action")
	bool IsTargeting() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void TriggerAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void TriggerActionFromAnimNotify(AActor* Instigator, FName SocketName);

	UFUNCTION(BlueprintCallable, Category= "Action")
	void StartCooldown();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void ClearCooldown();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void ReduceCooldown(float reduceTime);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UTLActionComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintPure, Category = "Targeting")
	float GetBaseDamage();


	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

	/*Action nickname to Start.Stop without a reference to the object*/
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UTLAction();

	UWorld* GetWorld() const override;

	/** IsSupportedForNetworking means an object can be referenced over the network */
	bool IsSupportedForNetworking() const override {return true;}

};
