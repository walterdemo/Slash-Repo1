// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

/*
#if PLATFORM_WINDOWS
THIRD_PARTY_INCLUDES_START
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"

#include "windows.h"
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_END
#endif
*/
#include "SlashHUD.generated.h"


class USlashOverlay;

UCLASS()
class SLASH_API ASlashHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = Slash)
	TSubclassOf<USlashOverlay> SlashOverlayClass;
	UPROPERTY()
	TObjectPtr<USlashOverlay> SlashOverlay;

public:
	FORCEINLINE USlashOverlay* GetSlashOverlay() const { return SlashOverlay; }//acces SlashOverlay properties on slash character.cpp

};