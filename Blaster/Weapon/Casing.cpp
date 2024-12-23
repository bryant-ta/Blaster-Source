#include "Casing.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"

ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);

	CasingMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore); // fix camera spazing in bc it collides

	// enable physics
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true); // important! for physics to be responsible for on hit

	ShellEjectionImpulse = FVector2D(15.f, 25.f);
	ShellEjectionAngleMax = 20.f;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::ACasing::OnHit);

	FVector ImpulseVector = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetActorForwardVector(), ShellEjectionAngleMax);
	CasingMesh->AddImpulse(ImpulseVector * FMath::RandRange(ShellEjectionImpulse.X, ShellEjectionImpulse.Y));
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}
	
	Destroy();
}

