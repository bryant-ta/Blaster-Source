// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 *   NOTE: this is just container widget, it seems functionality should be placed in Controller (since character can be destroyed?)
 */
UCLASS()
class BLASTER_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) // name must be exact same as widget in BP (WBP_CharacterOverlay)
	class UProgressBar* HealthBar; 

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefeatsAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WeaponAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CarriedAmmoAmount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MatchCountdownText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GrenadesText;

	UPROPERTY(meta = (BindWidget))
	class UImage* HighPingImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)	// Transient: not serialized to disk, settable at runtime
	UWidgetAnimation* HighPingAnimation;			// this is an animation on HighPingImage in engine
};
