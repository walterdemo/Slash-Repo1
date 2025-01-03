// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Potion.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API APotion : public AItems
{
	GENERATED_BODY()


protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Health Properties")
	int32 Health;
public:
	FORCEINLINE int32 GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(int32 AmountOfHealth) { Health = AmountOfHealth; }
};
