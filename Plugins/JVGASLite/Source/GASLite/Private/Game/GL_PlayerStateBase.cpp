// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GL_PlayerStateBase.h"
#include "Net/UnrealNetwork.h"
#include "../TLActionRoguelike.h"
#include "Game/GL_GameModeBase.h"
#include <Game/GL_GameStateBase.h>


AGL_PlayerStateBase::AGL_PlayerStateBase()
{
	BasePlayerStat = FBasePlayerStat();
	BasePlayerDesc = FBasePlayerDesc();
}

void AGL_PlayerStateBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
	
		AGL_GameStateBase* GS = Cast<AGL_GameStateBase>(GetWorld()->GetGameState());

		if (GS->GetCurrentMatchState() == EMatchState::PreMatchInitialize)
		{
			ClientMatchInitializeState();
		}

		ClientInitPlayerProperties();
	}

}

void AGL_PlayerStateBase::ClientMatchInitializeState_Implementation()
{

	// Add Code Here..

	InitMatchInitializeFromInstance();
}



void AGL_PlayerStateBase::InitPlayerPropertiesFromInstance_Implementation()
{

}

void AGL_PlayerStateBase::InitMatchInitializeFromInstance_Implementation()
{

}

float AGL_PlayerStateBase::GetKDRatio()
{
	float Ratio;
	if (BasePlayerStat.DeathCount <= 0)
	{
		Ratio = BasePlayerStat.KillCount / 1.0;
	}
	else
	{
		Ratio = BasePlayerStat.KillCount / BasePlayerStat.DeathCount;
	}
	return Ratio;
}

void AGL_PlayerStateBase::ServerUpdateKillCount_Implementation(int32 addedValue)
{
	BasePlayerStat.KillCount += addedValue;
	MulticastUpdateKillCount(BasePlayerStat.KillCount);
}

void AGL_PlayerStateBase::ServerUpdateDeathCount_Implementation(int32 addedValue)
{
	BasePlayerStat.DeathCount += addedValue;
	MulticastUpdateDeathCount(BasePlayerStat.DeathCount);
}

void AGL_PlayerStateBase::MulticastUpdateLevel_Implementation(int32 addedValue)
{
	BasePlayerStat.Level += addedValue;
}

void AGL_PlayerStateBase::MulticastUpdateMinionKillCount_Implementation(int32 addedValue)
{
	BasePlayerStat.MinionKillCount += addedValue; 
}

void AGL_PlayerStateBase::MulticastUpdateKillCount_Implementation(int32 addedValue)
{
	BasePlayerStat.KillCount += addedValue;
}


void AGL_PlayerStateBase::MulticastUpdateDeathCount_Implementation(int32 addedValue)
{
	BasePlayerStat.DeathCount += addedValue;
}

void AGL_PlayerStateBase::MulticastUpdateGoldAdded_Implementation(int32 addedValue)
{
	BasePlayerStat.Gold = addedValue;
}

void AGL_PlayerStateBase::SetTeam(uint8 NewTeamID)
{
	BasePlayerDesc.TeamID = NewTeamID;
}

void AGL_PlayerStateBase::InitFromGameInst()
{

}

void AGL_PlayerStateBase::ServerInitPlayerProperties_Implementation(FBasePlayerDesc newPlayerDesc)
{
	BasePlayerDesc = newPlayerDesc;

	MulticastInitPlayerProperties(BasePlayerDesc);
}

void AGL_PlayerStateBase::MulticastInitPlayerProperties_Implementation(FBasePlayerDesc newPlayerDesc)
{
	BasePlayerDesc = newPlayerDesc;
}

void AGL_PlayerStateBase::ClientInitPlayerProperties_Implementation()
{
	// Add Code Here...
	InitPlayerPropertiesFromInstance();
}

void AGL_PlayerStateBase::ServerWaitEndMatch_Implementation()
{
	ClientWaitEndMatch();
}

void AGL_PlayerStateBase::ClientWaitEndMatch_Implementation()
{
	InitWaitEndMatchFromInstance();
}

void AGL_PlayerStateBase::InitWaitEndMatchFromInstance_Implementation()
{
	// Add Code Here...
}

void AGL_PlayerStateBase::ServerWaitingStart_Implementation()
{
	ClientWaitingStart();
}

void AGL_PlayerStateBase::ClientWaitingStart_Implementation()
{
	InitWaitingStartFromInstance();
}

void AGL_PlayerStateBase::InitWaitingStartFromInstance_Implementation()
{
	 // Add Code Here..
}

void AGL_PlayerStateBase::ServerPlayerLogin_Implementation(AGL_PlayerStateBase* PS)
{
	FColor TextColor = FColor::Blue;
	FString LoginMsg = FString::Printf(TEXT("[%s] User Logging in: %s"),
		*GetNameSafe(this),
		*PS->GetPlayerNameDesv());
	LogOnScreen(this, LoginMsg, TextColor, 5.0f);
	
	//OnTriggerClientOpenScoreboard.BindUFunction(this, "TriggerClientOpenScoreboard");

	ClientPlayerLogin(PS);

}

void AGL_PlayerStateBase::ClientPlayerLogin_Implementation(AGL_PlayerStateBase* PS)
{
	//OnTriggerClientOpenScoreboard.BindUFunction(this, "TriggerClientOpenScoreboard");
}

void AGL_PlayerStateBase::ServerPlayerLogOut_Implementation(AGL_PlayerStateBase* PS)
{
	FColor TextColor = FColor::Blue;
	FString LogoutMsg = FString::Printf(TEXT("[%s] User Logging in: %s "),
		*GetNameSafe(this),
		*PS->GetPlayerNameDesv());
	LogOnScreen(this, LogoutMsg, TextColor, 5.0f);

	ClientPlayerLogOut(PS);
}

void AGL_PlayerStateBase::ClientPlayerLogOut_Implementation(AGL_PlayerStateBase* PS)
{

}

void AGL_PlayerStateBase::ServerSetTeamForPlayer_Implementation(uint8 TeamId)
{
	SetTeam(TeamId);
	MuliticastSetTeamForPlayer(TeamId);
}

void AGL_PlayerStateBase::MuliticastSetTeamForPlayer_Implementation(uint8 TeamId)
{
	SetTeam(TeamId);
	//OnTriggerClientOpenScoreboard.BindUFunction(this, "TriggerClientOpenScoreboard");
	
}

void AGL_PlayerStateBase::ServerMatchStart_Implementation()
{
	ClientMatchStart();
}

void AGL_PlayerStateBase::ClientMatchStart_Implementation()
{
	InitMatchStartFromInstance();
}

void AGL_PlayerStateBase::InitMatchStartFromInstance_Implementation()
{
	// Add Code Here...
}

void AGL_PlayerStateBase::ServePreMatchInitialize_Implementation()
{
	ClientPreMatchInitialize();
}

void AGL_PlayerStateBase::ClientPreMatchInitialize_Implementation()
{
	InitPreMatchInitializeFromInstance();
}

void AGL_PlayerStateBase::InitPreMatchInitializeFromInstance_Implementation()
{
	// Add Code Here...
}

void AGL_PlayerStateBase::TriggerClientOpenScoreboard()
{
	OnTriggerClientScoreboard.Broadcast();
}

void AGL_PlayerStateBase::ServerPlayerKilled_Implementation()
{
	ClientPlayerKilled();
}

void AGL_PlayerStateBase::ClientPlayerKilled_Implementation()
{
	InitPlayerKilledFromInstance();
}

void AGL_PlayerStateBase::InitPlayerKilledFromInstance_Implementation()
{
	// Add Code Here...
}

void AGL_PlayerStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGL_PlayerStateBase, BasePlayerStat);
	DOREPLIFETIME(AGL_PlayerStateBase, BasePlayerDesc);
	DOREPLIFETIME(AGL_PlayerStateBase, bIsDead);
	DOREPLIFETIME(AGL_PlayerStateBase, RespawnTimerValue);
	DOREPLIFETIME(AGL_PlayerStateBase, CanRespawn);
}