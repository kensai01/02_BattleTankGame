// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTanks.h"
#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	/* Drive the tracks. */
	DriveTrack();

	/* Apply a sideways force. */
	ApplySidewaysForce();

	/* Reset throttle back to zero. */
	CurrentThrottle = 0;

}

void UTankTrack::ApplySidewaysForce()
{
	/* Calculate the slippage speed (use dot product)
	dot product between the right vector of the tank track and the tank track velocity. */
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	/* Work-out required acceleration this frame. To correct: accelleration is speed over time 
	and then multiply it by the opposite direction we're slipping in. */
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	/* First get tank root component which is a static mesh, this allows us to access the mass of the tank */
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	/* Calculate and apply sideways force (F = m a) */
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; //Two tracks
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle)
{
	/* Clamp actual throttle value so player can't over-drive. */
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, +1);
}

void UTankTrack::DriveTrack()
{
	/* Calculate the force that will be applied to the tank tracks. */
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;

	/* The force location is the components location itself. */
	auto ForceLocation = GetComponentLocation();

	/* Get the tank root component in order to apply force. */
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	/* Apply force via root component at ForceLocation and ForceApplied. */
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}
