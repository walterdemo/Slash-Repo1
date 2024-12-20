// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

//draw error
#include "Kismet/KismetSystemLibrary.h"

//for editing GetCharacterMovement option (settingmovement)
#include "GameFramework/CharacterMovementComponent.h"


//use sound with hitSound and emitter
#include "Kismet/GameplayStatics.h"

#include "Slash/DebugMacros.h"

//Attribute component
#include "Components/AttributeComponent.h"

//widgetComponent health bar, it also need to be added on modules
//#include "Components/WidgetComponent.h"

//instead of UwidgetComponent we use the UHealthBarComponent which is derived from UwidgetComponent but we added some things
#include "HUD/HealthBarComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	/*
	//we used the widget first then changued to the UHealthBarComponent that we created later
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	*/
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	//character rotates while moving
	GetCharacterMovement()->bOrientRotationToMovement;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(false);
	}

	
}

void AEnemy::Die()
{
	//play die montage
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage) // HitReactMontage variable that comes from blueprint Animation Montage
	{
		AnimInstance->Montage_Play(DeathMontage);

		//this is constant because once setted here it will not change again
		const int32 Selection = FMath::RandRange(0, 5); //Random number from 0 to 1
		//SectionName is not constant because it is expected to change below
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5:
			SectionName = FName("Death6");
			DeathPose = EDeathPose::EDP_Death6;
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);

}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage) // HitReactMontage variable that comes from blueprint Animation Montage
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	//Lower Impact Point to the Enemy's hit

	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);

	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	//Forward * ToHit = |Forward||ToHit| * cos(theta)
	//|Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);//angle in radians
	//radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	//if CrossProduct points down, Theta should be negative, hand right math rule, but in UE in left hand
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}
	


	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);
	/*
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Red, 5.f);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));

	}

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
	*/
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CombatTarget) {
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();//distance from the actor to the enemy
		if (DistanceToTarget > CombatRadius) {
			CombatTarget = nullptr;
			if (HealthBarWidget) {
				HealthBarWidget->SetVisibility(false);
			}

		}

	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(true);//make bar visible when is hit
	}
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	if (Attributes && Attributes->isAlive()){
		DirectionalHitReact(ImpactPoint);
	}
	else {
		Die();
	}
	
	
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);

	}

	if (HitParticles && GetWorld())//if the object was created wetWorld is obvios True but is good practice this time for remember a good codding example
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint

		);

	}
	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//obs: take damage activates itself when UGameplayStatics::ApplyDamage is called in Weapon.cpp

	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);

		if (HealthBarWidget)
		{
			HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		}

	}

	CombatTarget = EventInstigator->GetPawn();//saving who damage the actor
	return DamageAmount;
}

