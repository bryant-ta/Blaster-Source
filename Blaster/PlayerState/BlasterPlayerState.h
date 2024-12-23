// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BlasterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	/*
	 * Replication notifies
	 */
	virtual void OnRep_Score() override;
	UFUNCTION()
	virtual void OnRep_Defeats();
	
	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);

	void PlayerEliminated(ABlasterPlayerState* AttackerPlayerState);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnPlayerEliminated(ABlasterPlayerState* AttackerPlayerState);

private:
	UPROPERTY()
	class ABlasterCharacter* Character;
	UPROPERTY(BlueprintGetter = GetController) // set getter function for use when GetController is called in BP, for exposing private UPROPERTY with specific getter function
	class ABlasterPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ABlasterPlayerController* GetController() const { return Controller; }
};
