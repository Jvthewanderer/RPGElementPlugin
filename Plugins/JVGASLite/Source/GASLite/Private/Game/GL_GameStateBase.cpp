// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GL_GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "../TLActionRoguelike.h"

AGL_GameStateBase::AGL_GameStateBase()
{
	BaseMatchDesc = FBaseMatchDesc();
	
	GameStateDesc = FGameStateDesc();
}

void AGL_GameStateBase::ServerUpdateTeamMatchScore_Implementation(uint8 TeamID, int32 Score)
{
	switch (TeamID)
	{
	case 0:
		CurrentScoreTeam1 = UpdateTeamScore(CurrentScoreTeam1, Score);
		break;
	case 1:
		CurrentScoreTeam2 = UpdateTeamScore(CurrentScoreTeam2, Score);
		break;
	default:
		break;
	}

	MulticastUpdateTeamMatchScore(CurrentScoreTeam1, CurrentScoreTeam2);
}

void AGL_GameStateBase::MulticastUpdateTeamMatchScore_Implementation(int32 MatchScoreTeam1, int32 MatchScoreTeam2)
{
	CurrentScoreTeam1 = MatchScoreTeam1;
	CurrentScoreTeam2 = MatchScoreTeam2;
}

int32 AGL_GameStateBase::UpdateTeamScore(int32 CurrentTeamScore, int32 AddedScore)
{	
	int32 LocPreNewValue;
	LocPreNewValue = CurrentTeamScore + AddedScore;
	return LocPreNewValue;
}

void AGL_GameStateBase::ServerUpdateWinner_Implementation(EWinner CurrentMatchWinner)
{

}

void AGL_GameStateBase::ServerUpdateMGSMatchState_Implementation(EMatchState MatchState)
{
	CurrentMatchState = MatchState;
	MulticastUpdateMatchState(MatchState);
	ServerHandleMatchState();
}

void AGL_GameStateBase::MulticastUpdateMatchState_Implementation(EMatchState MatchState)
{
	CurrentMatchState = MatchState;
}

void AGL_GameStateBase::ServerHandleMatchState_Implementation()
{

}

void AGL_GameStateBase::ServerInitializeTeamScore_Implementation()
{

}

void AGL_GameStateBase::MulticastInitializeTeamScore_Implementation(int32 MatchScoreTeam1, int32 MatchScoreTeam2)
{
	CurrentScoreTeam1 = MatchScoreTeam1;
	CurrentScoreTeam2 = MatchScoreTeam2;
}

void AGL_GameStateBase::ServerUpdateTimer_Implementation(int32 GameTimer)
{
	CurrentTime = GameTimer;
	MulticastUpdateTimer(GameTimer);
}

void AGL_GameStateBase::MulticastUpdateTimer_Implementation(int32 GameTimer)
{
	CurrentTime = GameTimer;
}

void AGL_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGL_GameStateBase, CurrentRoundCount);
	DOREPLIFETIME(AGL_GameStateBase, CurrentScoreTeam1);
	DOREPLIFETIME(AGL_GameStateBase, CurrentScoreTeam2);
	DOREPLIFETIME(AGL_GameStateBase, BaseMatchDesc);
	DOREPLIFETIME(AGL_GameStateBase, GameStateDesc);
	DOREPLIFETIME(AGL_GameStateBase, CurrentMatchState);
	DOREPLIFETIME(AGL_GameStateBase, CurrentTime);


}