// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items.generated.h"

class USphereComponent;
class UNiagaraComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equiped
};


UCLASS()
class SLASH_API AItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItems();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitud = 0.25f;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	//template function
	template<typename T>
	T Avg(T First, T Second);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Attr")
	float RotationSpeed = 10.f;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();
	
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering;


	//this was moved to protected because now we want to change the colision after weapon is equipped in weapon child
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> ItemEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

	

};

template<typename T>
inline T AItems::Avg(T First, T Second)
{
	return (First + Second)/2;
}
