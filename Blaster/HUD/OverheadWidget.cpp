// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	// NOTE: Lec 42, widget BP call time seems to be before other Players actually join and PlayerState gets replicated
	// so they end up null. This should be triggered by OnRep_PlayerState 
	// This needs null check and other char Player names will not show on yours
	if (InPawn)
	{
		const APlayerState* PlayerState = InPawn->GetPlayerState();
		if (PlayerState)
		{
			SetDisplayText(PlayerState->GetPlayerName());
		}
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString LRole;
	switch (LocalRole)
	{
	case ENetRole::ROLE_Authority:
		LRole = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		LRole = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		LRole = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		LRole = FString("None");
		break;
	}

	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString RRole;
	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		RRole = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		RRole = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		RRole = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		RRole = FString("None");
		break;
	}

	// Use *Role overload to get C-string base of FString to pass to %s
	FString LocalRoleString = FString::Printf(TEXT("Local: %s | Remote: %s"), *LRole, *RRole);
	SetDisplayText(LocalRoleString);
}

// NOTE: Potentially already called by private version of this function that is already in Super
//void UOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
//{
//	RemoveFromParent();
//	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
//}