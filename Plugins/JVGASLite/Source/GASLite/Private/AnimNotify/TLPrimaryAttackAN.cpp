// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/TLPrimaryAttackAN.h"
#include "RLCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Component/TLActionComponent.h"


void UTLPrimaryAttackAN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* MyOwner = MeshComp->GetOwner();
	UTLActionComponent* ActionComp = Cast<UTLActionComponent>(MyOwner->GetComponentByClass(UTLActionComponent::StaticClass()));
	ARLCharacter* PCharacter = Cast<ARLCharacter>(MyOwner);

	if (MyOwner->HasAuthority())
	{
		if (ActionComp)
		{
			ActionComp->TriggerActionFromAnimNotifyByName(MyOwner, ActionName, SocketName);

		}
	}
}