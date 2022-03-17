// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/TLActionEffect.h"
#include "Component/TLActionComponent.h"
#include <GameFramework/GameStateBase.h>


float UTLActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;

}

UTLActionEffect::UTLActionEffect()
{
	Duration = 0.0f;

	Period = 0.0f;

	bAutoStart = true;
}

void UTLActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if(Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void UTLActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UTLActionComponent* Comp = GetOwningComponent();
	if (Comp) 
	{
		Comp->RemoveAction(this);
	}
}

void UTLActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Execute Effect: %s"), *GetNameSafe(this));
}

