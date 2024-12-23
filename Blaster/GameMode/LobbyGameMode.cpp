// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Travel to game map (server) when number of players 
	// NOTE: PostLogin() is first place its safe to access the player that just joined
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true; // Unreal seamless travel (vs. nonseamless loading)
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen")); // NOTE: path from Content/Maps/..., replace content with Game. "?listen" specifies listen server open for clients
		}
	}
}