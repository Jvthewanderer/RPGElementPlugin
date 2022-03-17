// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GL_GameInstance.h"


UGL_GameInstance::UGL_GameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


void UGL_GameInstance::LocalUpdatePlayerName(FString PlayerName)
{
	SetLocalClientName(PlayerName);
}

void UGL_GameInstance::SetLocalClientAsHost()
{
	MenuPlayerDesc.IsHost =  true;
}

void UGL_GameInstance::SetLocalClientAsClient()
{
	MenuPlayerDesc.IsClient = true;
}

void UGL_GameInstance::ResetHostClientSettings()
{
	MenuPlayerDesc.IsHost = false;
	MenuPlayerDesc.IsClient = false;
}

void UGL_GameInstance::SetLocalClientName(FString _playerName)
{

	MenuPlayerDesc.PlayerName = _playerName;

}

FMenuPlayerDesc UGL_GameInstance::GetMenuPlayerDesc()
{
	return MenuPlayerDesc;
}

void UGL_GameInstance::SetMenuPlayerDesc(FMenuPlayerDesc newPlayerDesc)
{
	MenuPlayerDesc = newPlayerDesc;
}

FMenuGameDesc UGL_GameInstance::GetMenuGameDesc()
{
	return MenuGameDesc;
}

void UGL_GameInstance::SetMenuGameDesc(FMenuGameDesc newGameDesc)
{
	MenuGameDesc = newGameDesc;
}

