// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TLGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API UTLGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool GetAllFreindlyActors(AActor* CurrentActor, const TArray<AActor*> Actors, TArray<AActor*>& OutFriendlyActors);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static bool GetAllEnemyActors(AActor* CurrentActor, const TArray<AActor*> Actors, TArray<AActor*>& OutEnemyActors);

	UFUNCTION(BlueprintCallable, Category = "Team")
	static bool SetActorTeamID(AActor* CurrentActor, uint8 TeamID);

	UFUNCTION(BlueprintCallable, Category = "AnimMontage")
	static float PlayActorAnimMontage(AActor* CurrentActor, class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);
};
