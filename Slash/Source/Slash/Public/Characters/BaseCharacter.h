// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h" // inheritance from another parent

#include "BaseCharacter.generated.h"


class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ABaseCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//translated
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	virtual void Attack();
	virtual void Die();
	
	
	

	/*
	* Play montage functions
	*/

	
	virtual void PlayAttackMontage();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual bool CanAttack();
	bool IsAlive();

	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	
	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<AWeapon> EquippedWeapon; // it comes fro, `rovate but since we translated and gonna use ot , changed to protected

	/*
	*Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> AttackMontage;

	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> DeathMontage;

	/*
	* Components
	*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> Attributes;

	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	TObjectPtr<UParticleSystem> HitParticles;
};
