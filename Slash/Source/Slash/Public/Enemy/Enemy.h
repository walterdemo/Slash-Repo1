// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h" //changed for the line down here
#include "Characters/BaseCharacter.h"
//#include "Interfaces/HitInterface.h" // inheritance from another parent
//bringing values like the Enum
#include "Characters/CharacterTypes.h"

#include "Enemy.generated.h"


class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter // inheritance from another parent
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckPatrolTarget();

	void CheckCombatTarget();


	//
	//virtual void GetHit(const FVector& ImpactPoint) override;
	//since GetHit is a Blueprint native event we should use this new which is internally generated
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Destroyed() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//die animation montages function
	virtual void Die() override;
	bool InTargetRange(AActor* Target, double Radius);

	//function of this cause is a repeated action
	void MoveToTarget(AActor* Target);
	TObjectPtr<AActor> ChoosePatrolTarget();

	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 5.f;



	//AIon seen delegate
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;



private:
	

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;//this was UWidget component before, but then we changed to UHealthComponent, it also needed to change on the forward declaration

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensing;

	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 130.f;

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


	/*
	AI behavior
	*/
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();

	/*
	Combat
	*/
	void StartAttackTimer();
	void ClearAttackTimer();

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;
public:	
	

};
