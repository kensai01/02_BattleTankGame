// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTanks.h"
#include "Tank.h"
#include "TankAimingComponent.h"


ATank::ATank(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	/* Sets the starting health
	TODO Make Damage class to handle all damage*/
	CurrentHealth = StartingHealth;
}

/* Called by the health bar widget to display the correct fill percent on the progress bar
that is used to display health above */
float ATank::GetHealthPercent() const
{
	return (float)CurrentHealth / (float)StartingHealth;
}



float ATank::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	/* Convert a floating point radial damage to an integer */
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);

	/* Make sure the damage we apply is between 0 and current available health */
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	/* Decrease health accordingly */
	// TODO Implement a better damage handling system, why do I decrease the health here then return the damage amount below? Makes no sense
	CurrentHealth -= DamageToApply;
	
	/* Play damage sound */
	// TODO Create method that handels sound of various kinds rather than hard coding a spawn sound event 
	UGameplayStatics::SpawnSoundAttached(SoundTakeHit, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);

	/* Kill player if health reaches zero */
	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
	}

	return DamageToApply;
}

/* Returns true if the player is alive*/
// TODO Is this needed?
bool ATank::IsAlive() const
{
	return CurrentHealth > 0;
}

/// NOISE HANDLING
/* This function is blueprint callable and is used to make 
Tank type actor create sounds that can be picked up by other 
actors that have a AI that listens for hearing type stimulus. */
void ATank::MakePawnNoise(float Loudness)
{
	if (Role == ROLE_Authority)
	{
		/* Make noise to be picked up by PawnSensingComponent by the enemy pawns */
		MakeNoise(Loudness, this, GetActorLocation());
	}
	LastNoiseLoudness = Loudness;
	LastMakeNoiseTime = GetWorld()->GetTimeSeconds();
}

/* Get the last noise loudness. */
float ATank::GetLastNoiseLoudness()
{
	return LastNoiseLoudness;
}

/* Get the last noise time. */
float ATank::GetLastMakeNoiseTime()
{
	return LastMakeNoiseTime;
}

void ATank::MakeSoundTankFiring()
{
	/* Make sure that we have a sound attached. */
	if (!SoundTankFiring) { return; }

	/* Get the aiming component which has the firing status that we need to query. */
	auto AimingComponent = this->FindComponentByClass<UTankAimingComponent>();

	/* Play sound only when tank isn't reloading. */
	if (AimingComponent->GetFiringState() != EFiringStatus::Reloading) {
		/* Play firing sound */
		UGameplayStatics::SpawnSoundAttached(SoundTankFiring, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
	}
}

// TODO What does this do?
//void ATank::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	/*
//	 Value is already updated locally, skip in replication step
//	DOREPLIFETIME_CONDITION(ATank, bWantsToRun, COND_SkipOwner);
//	DOREPLIFETIME_CONDITION(ATank, bIsTargeting, COND_SkipOwner);
//
//	 Replicate to every client, no special condition required
//	DOREPLIFETIME(ATank, Health);
//	DOREPLIFETIME(ATank, LastTakeHitInfo);
//	*/
//}

