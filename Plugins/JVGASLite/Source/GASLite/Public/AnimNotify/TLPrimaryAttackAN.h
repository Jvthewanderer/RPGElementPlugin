// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TLPrimaryAttackAN.generated.h"


class UParticleEmitter;
/**
 * 
 */
UCLASS()
class GASLITE_API UTLPrimaryAttackAN : public UAnimNotify
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere)
	FName SocketName;

	UPROPERTY(EditAnywhere)
	FName ActionName;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
