// Copyright MB

#pragma once

#include "Player/Tank.h"
#include "TankSentry.generated.h"

/**
 * 
 */

UCLASS()
class BATTLETANKS_API ATankSentry : public ATank
{
	GENERATED_BODY()

	///CURRENTLY UNUSED
	/* Last time we attacked something */
	//float LastRangeAttackTime;


private:
	/// SENSES
	/* Last time the player was spotted */
	float LastSeenTime;

	/* Last time the player was heard */
	float LastHeardTime;

	/* Resets after sense time-out to avoid unnecessary clearing of target each tick */
	bool bSensedTarget;

	/* Time-out value to clear the sensed position of the player. Should be higher than Sense interval in the PawnSense component not never miss sense ticks. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SenseTimeOut;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void TakeAimAndFireOnSensedTarget();

protected:
	// How close the AI can get
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 8000;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	/// TODO Implement along with sound
	///* Update the vocal loop of the zombie (idle, wandering, hunting) */
	//UFUNCTION(Reliable, NetMulticast)
	//	void BroadcastUpdateAudioLoop(bool bNewSensedTarget);

	//void BroadcastUpdateAudioLoop_Implementation(bool bNewSensedTarget);

	//UAudioComponent* PlayCharacterSound(USoundCue* CueToPlay);

	//UPROPERTY(EditDefaultsOnly, Category = "Sound")
	//	USoundCue* SoundPlayerNoticed;

	//UPROPERTY(EditDefaultsOnly, Category = "Sound")
	//	USoundCue* SoundHunting;

	//UPROPERTY(EditDefaultsOnly, Category = "Sound")
	//	USoundCue* SoundIdle;

	//UPROPERTY(EditDefaultsOnly, Category = "Sound")
	//	USoundCue* SoundWandering;

	//UPROPERTY(EditDefaultsOnly, Category = "Sound")
	//	USoundCue* SoundAttackMelee;
	
	/// TODO Implement along with sound
	///* Plays the idle, wandering or hunting sound */
	//UPROPERTY(VisibleAnywhere, Category = "Sound")
	//	UAudioComponent* AudioLoopComp;

	//virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled) override;

public:
	ATankSentry(const class FObjectInitializer& ObjectInitializer);

	/* The bot behavior we want this bot to execute, (passive/patrol) by specifying EditAnywhere we can edit this value per-instance when placed on the map. */
	UPROPERTY(EditAnywhere, Category = "AI")
	EBotBehaviorType BotType;

	/* The thinking part of the brain, steers our zombie and makes decisions based on the data we feed it from the Blackboard */
	/* Assigned at the Character level (instead of Controller) so we may use different zombie behaviors while re-using one controller. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/* Change default bot type during gameplay */
	void SetBotType(EBotBehaviorType NewType);
	
};