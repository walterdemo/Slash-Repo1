// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h" // inheritance from another parent
//bringing values like the Enum
#include "Characters/CharacterTypes.h"

#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface // inheritance from another parent
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckPatrolTarget();

	void CheckCombatTarget();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//
	//virtual void GetHit(const FVector& ImpactPoint) override;
	//since GetHit is a Blueprint native event we should use this new which is internally generated
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//die animation montages function
	void Die();
	bool InTargetRange(AActor* Target, double Radius);

	//function of this cause is a repeated action
	void MoveToTarget(AActor* Target);
	TObjectPtr<AActor> ChoosePatrolTarget();

	//AIon seen delegate
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);



	/*
	* Play montage functions - this is called refactoring for avoid having to much code in one space
	*/
	void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;





private:
	/*
	* Components
	*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;//this was UWidget component before, but then we changed to UHealthComponent, it also needed to change on the forward declaration

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensing;

	/*
	*Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	TObjectPtr<UParticleSystem> HitParticles;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	/*
	*Navigation
	*/
	UPROPERTY()
	TObjectPtr<AAIController> EnemyController;
	//class AAIControler* EnemyController;

	//Current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f; // should be a little bit bigger than acceptance radius in BeginPlay function

	FTimerHandle PatrolTimer;//like a timer with an alarma and function attach
	void PatrolTimerFinished();//when the alarm hit, this funciton is called

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

public:	
	

};
