// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <GameplayTagContainer.h>
#include "TLActionComponent.generated.h"


class UTLAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UTLActionComponent*, OwningComp, UTLAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASLITE_API UTLActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tag")
	FGameplayTag StunnedTag;

	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(AActor* Instigator, TSubclassOf<UTLAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(UTLAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UTLAction* GetAction(TSubclassOf<UTLAction> ActionClass) const;

	UFUNCTION(BlueprintPure, Category = "Action")
	UTLAction* GetActionByName(FName ActionName) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool TriggerActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool TriggerActionFromAnimNotifyByName(AActor* Instigator, FName ActionName,FName SocketName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool ConfirmAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool CancleAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void SetTargeting( bool NewTargeting);

	UFUNCTION(BlueprintPure, Category = "Action")
	bool IsTargeting(){ return bIsTargeting;}

	UFUNCTION(BlueprintPure, Category = "Action")
		bool IsStunned() {
		return ActiveGameplayTags.HasTag(StunnedTag);
	}

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	static UTLActionComponent* GetActionComponent(AActor* FromActor);

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionTriggered;


	UTLActionComponent();

protected:
	
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerTriggerAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerTriggerActionFromAnimNotify(AActor* Instigator, FName ActionName, FName SocketName);

	UFUNCTION(Server, Reliable)
	void ServerConfirmAction(AActor* Instigator);

	UFUNCTION(Server, Reliable)
	void ServerCancleAction(AActor* Instigator);

	UPROPERTY(EditAnywhere, Category = "Actions")
	bool bIsTargeting;
	
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UTLAction>> DefaultActions;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UTLAction*> Actions;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	

	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
