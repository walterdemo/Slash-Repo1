// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
//Attribute component
#include "Components/AttributeComponent.h"
//AI sensingcomponent, need module
#include "Perception/PawnSensingComponent.h"


//draw error
#include "Kismet/KismetSystemLibrary.h"

//for editing GetCharacterMovement option (settingmovement)
#include "GameFramework/CharacterMovementComponent.h"


//use sound with hitSound and emitter
//#include "Kismet/GameplayStatics.h"

#include "Slash/DebugMacros.h"


//widgetComponent health bar, it also need to be added on modules
//#include "Components/WidgetComponent.h"

//instead of UwidgetComponent we use the UHealthBarComponent which is derived from UwidgetComponent but we added some things
#include "HUD/HealthBarComponent.h"

//weapon
#include "Items/Weapons/Weapon.h"

//AI patrol of enemy
#include "AIController.h" //it requires module


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

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);



}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HideHealthBar();

	EnemyController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);
	//this is called in tick function
	// GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 5.f);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	//attaching the sword
	TObjectPtr<UWorld> World = GetWorld();
	if (World && WeaponClass)
	{
		TObjectPtr<AWeapon> DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
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

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false; //added later for not needing to check if pointer exists
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();//distance from the actor to the enemy
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	
	return DistanceToTarget <=  Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	
	if (EnemyController == nullptr || Target == nullptr) return;
	
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);//goal actor
	MoveRequest.SetAcceptanceRadius(50.f);//enemy willl stop at 15 u
	//FNavPathSharedPtr NavPath;//use for draw navigation path

	EnemyController->MoveTo(MoveRequest);// , & NavPath);//NavPath is optional parameter
	
	/*Navigatin path
	TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
	for (auto& Point : PathPoints)
	{
		const FVector& Location = Point.Location;
		DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
	}
	*/
}

TObjectPtr<AActor> AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;

	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target); // to avoid repet actual target
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];

	}
	return nullptr;
}

void AEnemy::Attack()
{
	Super::Attack();
	PlayAttackMontage();
}

void AEnemy::PlayAttackMontage() 
{
	Super::PlayAttackMontage();
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		//this is constant because once setted here it will not change again
		const int32 Selection = FMath::RandRange(0, 2); //Random number from 0 to 2
		//SectionName is not constant because it is expected to change below
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}


}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{	
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}



void AEnemy::PawnSeen(APawn* SeenPawn)//it wil cal l frecuently (tick)
{
	const bool bShouldChaseTarget =

		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("SlashCharacter"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;//actual target
		ClearPatrolTimer();
		ChaseTarget();
	}

	
}



void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget) {
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	//Outside combat radius , lose interest
	CombatTarget = nullptr;
	HideHealthBar();//refactored
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
	//Outside attack range, chase character
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
	//UE_LOG(LogTemp, Warning, TEXT("Chase Player"));
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);//when timer counts and if its chasing it will change to patrolling again, so ww are deactivating timer, clear?
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);

}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}



// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/* old way A
	if (CombatTarget) {
		//doing this in InTargetRange function
		//const double DistanceToTarget = (CombatTarget->GetActorLocation() - GetActorLocation()).Size();//distance from the actor to the enemy
		//if (DistanceToTarget > CombatRadius) {
		if(!InTargetRange(CombatTarget, CombatRadius))
		{
			CombatTarget = nullptr;
			if (HealthBarWidget) {
				HealthBarWidget->SetVisibility(false);
			}

		}

	}*/
	
	if (IsDead()) return;

	if (EnemyState > EEnemyState::EES_Patrolling) //Patrolling is state 0 so any other state will be more than 0
	{
		CheckCombatTarget(); // new way A
	}
	else
	{
		CheckPatrolTarget();//ChoosePatrolTarget
	}


	/*
	if (PatrolTarget && EnemyController)
	{
		if (InTargetRange(PatrolTarget, PatrolRadius))
		{
			TArray<AActor*> ValidTargets;

			for (AActor* Target : PatrolTargets) 
			{
				if (Target != PatrolTarget)
				{
					ValidTargets.AddUnique(Target); // to avoid repet actual target
				}

			}

			const int32 NumPatrolTargets = ValidTargets.Num();
			if (NumPatrolTargets > 0)
			{
				const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
				AActor* Target = ValidTargets[TargetSelection];
				PatrolTarget = Target;

				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalActor(PatrolTarget);//goal actor
				MoveRequest.SetAcceptanceRadius(15.f);//enemy willl stop at 15 u

				EnemyController->MoveTo(MoveRequest);
			}


		}
	}
	*/
}

void AEnemy::CheckPatrolTarget()
{
	
	
	
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();

		//MoveToTarget(PatrolTarget);
		//this one is for having a delay between targets

		//random value for the time
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);



		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		UE_LOG(LogTemp, Warning, TEXT("Outside Combat radiuds"));
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
		
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hello World"));
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hello World"));
		StartAttackTimer();
	}


}


void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	ShowHealthBar();
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	if (IsAlive()){
		DirectionalHitReact(ImpactPoint);
	}
	else Die();
	
	PlayHitSound(ImpactPoint);

	SpawnHitParticles(ImpactPoint);
	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//obs: take damage activates itself when UGameplayStatics::ApplyDamage is called in Weapon.cpp

	HandleDamage(DamageAmount);

	CombatTarget = EventInstigator->GetPawn();//saving who damage the actor
	ChaseTarget();
	
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

