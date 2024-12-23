// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "GameFramework/Character.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "BlasterCharacter.generated.h"

class AWeapon;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlayElimMontage();
	void PlayThrowGrenadeMontage();
	virtual void OnRep_ReplicatedMovement() override;
	
	void Elim();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();
	virtual void Destroyed() override;

	UPROPERTY(Replicated)
	bool bDisableGameplay = false; // flag disabling most inputs besides movements

	// custom non course stuff
	void ShowPlayerName();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	// end custom stuff

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);
	
	void UpdateHUDHealth();
	void UpdateHUDAmmo();

	void SpawnDefaultWeapon();

	UPROPERTY()
	TMap<FName, class UBoxComponent*> HitCollisionBoxes;

protected:
	virtual void BeginPlay() override;

	// Input functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AimOffset(float DeltaTime); // called in Tick, not an input function
	void CalculateAO_Pitch(); // not input
	float CalculateSpeed() const; // not input
	// void SimProxiesTurn(); // also not input he shouldn't put it here smh
	virtual void Jump() override;
	void FireButtonPressed();
	void FireButtonReleased();
	
	void PlayHitReactMontage();

	void GrenadeButtonPressed();
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void DropOrDestroyWeapons();

	// (binding) callback signature for OnTakeAnyDamage unreal event
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void PollInit(); // poll for classes that aren't ready on first frame
	void RotateInPlace(float DeltaTime);

	/*
	 * Hit Boxes (Server-side Rewind)
	 */

	// variables are named same as bone name for convenience
	UPROPERTY(EditAnywhere)
	class UBoxComponent* head;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* pelvis;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* spine_02;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* spine_03;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* upperarm_l;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* upperarm_r;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* lowerarm_l;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* lowerarm_r;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* hand_l;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* hand_r;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* backpack;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* blanket;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* thigh_l;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* thigh_r;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* calf_l;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* calf_r;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* foot_l;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* foot_r;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true)) // need meta for using BlueprintReadOnly attribute on this private c++ var
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon) // create replicated var, link to rep notify
	class AWeapon* OverlappingWeapon;

	UFUNCTION()						// func to call whenever OverlappingWeapon is replicated, must be UFUNCTION
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon); // convention is OnRep_ prefix, NOTE: this type of func can have up to 1 param which must match replicated var, contains previous var value before rep

	/*
	 * Blaster Components
	 */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
	class UBuffComponent* Buff;

	UPROPERTY(VisibleAnywhere)
	class ULagCompensationComponent* LagCompensation;

	UFUNCTION(Server, Reliable) // declare Server RPC (reliable in this case) (always on server only)
	void ServerEquipButtonPressed();

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	/*
	 * Animation Montages
	 */
	
	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* ThrowGrenadeMontage;

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	// bool bRotateRootBone;
	// float TurnThreshold = 0.5f;
	// FRotator ProxyRotationLastFrame;
	// FRotator ProxyRotation;
	// float ProxyYaw;
	// float TimeSinceLastMovementReplication;

	/*
	 * Player Health
	 */

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health(float LastHealth);

	UPROPERTY() // UPROPERTY() ensures this == nullptr, so that the pointer is not left uninitialized! (prob also related to GC too)
	class ABlasterPlayerController* BlasterPlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;
	UPROPERTY(EditDefaultsOnly) // EditDefaultsOnly -> restricts so every character instance will end up having same value
	float ElimDelay = 3.f;
	void ElimTimerFinished();

	/*
	 * Dissolve Effect
	 */

	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack; // equivalent of track in BP Timeline node, a track on the timeline

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue); // callback (binding, notice UFUNCTION) for DissolveTimeline
	void StartDissolve();

	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance; // set at runtime

	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance; // set on BP, to be used as Dynamic

	/*
	 * Elim Bot
	 */

	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect; // the particle system

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComponent; // the actual particle system instance in level

	UPROPERTY(EditAnywhere)
	class USoundCue* ElimBotSound;

	UPROPERTY() 
	class ABlasterPlayerState* BlasterPlayerState;

	/*
	 * Grenade
	 */

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttachedGrenade;

	/*
	 * Default Weapon
	 */

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;
	
public:	
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	ECombatState GetCombatState() const;
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
	bool IsLocallyReloading();
	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }
};
