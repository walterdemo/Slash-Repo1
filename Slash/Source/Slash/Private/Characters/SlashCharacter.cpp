// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"

//Enhanced input needed
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


//sringArm and camera
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

//Setting Movement on character
#include "GameFramework/CharacterMovementComponent.h"

//Setting fro groom
#include "GroomComponent.h"

//weapon and item 
#include "Items/Items.h"
#include "Items/Weapons/Weapon.h"

//UAnimMontage https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/Animation/UAnimMontage
#include "Animation/AnimMontage.h"

//Accesing WeaponBox from Overlapping item
#include "Components/BoxComponent.h"

// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setting movement on character, face orientation
	GetCharacterMovement()->bOrientRotationToMovement = true; //checking the checkbox instead of blueprint
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f); //setting value instead of using blueprint


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom); //Need to activate Use Pawn control rotation in camera settings on blueprint
	
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");
	
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
	

}

void ASlashCharacter::MoveChar(const FInputActionValue& Value)
{
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>(); // if i press w x value will get 1 if i press d , y value will get 1, and the opposite will happens

		//const FRotator Rotation = Controller->GetControlRotation();

		/*
		const FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, MovementVector.Y);
		const FVector Right = GetActorRightVector();
		AddMovementInput(Right, MovementVector.X);
		*/

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //Axes is plural
		AddMovementInput(ForwardDirection, MovementVector.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //Axes is plural
		AddMovementInput(RightDirection, MovementVector.X);
	}



}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);


	//check on CharMoveComp ->Orient Rotation to Movement in blueprint it also was done on constructor
	//uncheck use controller rotation yaw and user controller rotation pitch in blueprint
}


// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	//TobjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller);
	//if (PlayerController)
	if(TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))//to save memory if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}

	Tags.Add(FName("SlashCharacter"));//Adding tags to  this char

	
}

void ASlashCharacter::Jump()
{
	Super::Jump();
	/*
	if (IsUnoccupied())
	{
		Super::Jump();
	}*/
}

void ASlashCharacter::EKeyPressed()
{
	//TODO we have to fully understand how this works
	//We are ggonna cast the overlapping item to the A weapon type
	//equal to the result of a cast of Aweapon castin overlappingItem
	TObjectPtr<AWeapon> OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	//OverlappingItem is Set on Item.cpp when item is overlapping the character
	//it will return null pointer if it is not overlapping
	if (OverlappingWeapon)
	{
		//Im gonna ask what weapon type I have
		EWeaponType WeaponType = OverlappingWeapon->GetWeaponType();

		if (WeaponType == EWeaponType::EWT_OneHanded) {
			//the we tell the weapon to get attached
			OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			isWeapon1 = true;
		}
		else if (WeaponType == EWeaponType::EWT_TwoHanded) {
			OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocketWeaponB"), this, this);
			CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
			isWeapon2 = true;
		}
		/*
		//we are gonna set owner and instigator on equip function
		//actors can own others actors
		OverlappingWeapon->SetOwner(this);
		OverlappingWeapon->SetInstigator(this);//instigator is like ownder but always is a pawn
		*/

		
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Unequip")); // here are the names, same names of the animation montage
			
			CharacterState = ECharacterState::ECS_Unequipped;

			ActionState = EActionState::EAS_EquippingWeapon;

		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));// here are the names, same names of the animation montage
			if (isWeapon1) {
				CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			}
			if (isWeapon2) {
				CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
			}
			
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void ASlashCharacter::Attack()
{
	Super::Attack();
	if (CanAttack()) // can Attack is a bool variable which determined the state of the character down below
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}


void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)// EquipMontage variable that comes from blueprint Animation Montage
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}


void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanAttack()
{

	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}



bool ASlashCharacter::CanDisarm()
{
	//if character is not attacking and is not unequipped
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	//if character is not attacking(Unoccupied) and is unequipped and EquippedWeapon is a valid pointer it means that there is an Equipped Weapon already
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&//here the difference with can disarm
		EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		//the we tell the wepon to get attached
		if (isWeapon1) {
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
		}
		else {
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket2"));
		}
		
	}
}

void ASlashCharacter::Arm()
{
	if (EquippedWeapon)
	{
		//the we tell the wepon to get attached
		
		if (isWeapon1) {
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
		}
		else {
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocketWeaponB"));
		}
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}


// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::MoveChar);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);

		//binding actions
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}


}
