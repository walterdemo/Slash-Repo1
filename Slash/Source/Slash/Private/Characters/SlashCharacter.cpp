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

//for changing collision type
#include "Components/StaticMeshComponent.h"

//Setting fro groom
#include "GroomComponent.h"

//being able to acces attribute health values for input in HUD in initialize function
#include "Components/AttributeComponent.h"

//weapon and item 
#include "Items/Items.h"
#include "Items/Weapons/Weapon.h"

//UAnimMontage https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/Animation/UAnimMontage
#include "Animation/AnimMontage.h"

//being able o access HUD properties so I can access to its functions and set health and other values
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"


#include "Items/Soul.h"
#include "Items/Treasure.h"

//Accesing WeaponBox from Overlapping item
//#include "Components/BoxComponent.h" //NOT NEEDED casue its used in BaseCharacter from now on

// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true; //not using tick

	//Setting movement on character, face orientation
	GetCharacterMovement()->bOrientRotationToMovement = true; //checking the checkbox instead of blueprint
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f); //setting value instead of using blueprint

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

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

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)//no needed
{
	//Super::Tick(DeltaTime);
	if (Attributes && SlashOverlay)
	{
		Attributes->RegenStamina(DeltaTime);//every tick
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());//update the overlay
	}
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
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
	}


}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision); // in case is playing the animation montage and gets interrupted, notification about ending attack never happens and get stcuk
	//ActionState = EActionState::EAS_HitReaction;
	if (Attributes && Attributes->GetHealthPercent() > 0.f) // is Alive
	{
		
		ActionState = EActionState::EAS_HitReaction;//call hit reaction only if character us are alive
	}

}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();//updating HUD
	return DamageAmount;
}

void ASlashCharacter::SetOverlappingItem(AItems* Item)
{
	OverlappingItem = Item;
}

void ASlashCharacter::AddSouls(ASoul* Soul) // this function is overriding function in PickupInterface and is called in overlap function inside soul
{
	//UE_LOG(LogTemp, Warning, TEXT("ASlashCharacter::AddSouls"));
	if (Attributes && SlashOverlay)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AddingSouls"));
		Attributes->AddSouls(Soul->GetSouls()); //Get souls it gets the value of the soul
		SlashOverlay->SetSouls(Attributes->GetSouls());//get soul it get the acumulated amount of soul
	}
}

void ASlashCharacter::AddGold(ATreasure* Treasure)// this function is overriding function in PickupInterface and is called in overlap function inside soul
{
	if (Attributes && SlashOverlay)
	{	
		//UE_LOG(LogTemp, Warning, TEXT("AddingGold"));
		Attributes->AddGold(Treasure->GetGold());
		SlashOverlay->SetGold(Attributes->GetGold());
	}
}

void ASlashCharacter::AddHealth(APotion* Potion)
{
	if (Attributes && SlashOverlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddingHealth"));
		Attributes->AddHealth(Potion->GetHealth());
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}

}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	//TobjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller);
	//if (PlayerController)
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))//to save memory if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}

	Tags.Add(FName("EngageableTarget"));//Adding tags to  this char

	InitializeSlashOverlay();
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


void ASlashCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
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
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();

		}
		else if (CanArm())
		{
			Arm();
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

void ASlashCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;//if is occupied or doenst have enough stamine it doesnt make anything

	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;

	if (Attributes && SlashOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}

}

void ASlashCharacter::EquipWeapon(TObjectPtr<AWeapon>& Weapon)
{
	//Im gonna ask what weapon type I have

	EWeaponType WeaponType = Weapon->GetWeaponType();

	if (WeaponType == EWeaponType::EWT_OneHanded) {
		//the we tell the weapon to get attached
		Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		isWeapon1 = true;
	}
	else if (WeaponType == EWeaponType::EWT_TwoHanded) {
		Weapon->Equip(GetMesh(), FName("RightHandSocketWeaponB"), this, this);
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
	EquippedWeapon = Weapon;
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

void ASlashCharacter::Arm()
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

void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip")); // here are the names, same names of the animation montage

	CharacterState = ECharacterState::ECS_Unequipped;

	ActionState = EActionState::EAS_EquippingWeapon;
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

void ASlashCharacter::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, "Default");
}

void ASlashCharacter::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

bool ASlashCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}
bool ASlashCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}


void ASlashCharacter::AttachWeaponToBack()
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

void ASlashCharacter::AttachWeaponToHand()
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

void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::DodgeEnd()
{
	//Super::DodgeEnd();//on in the lesson this functioin comes  from BaseCharacter
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void ASlashCharacter::InitializeSlashOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			{
				SlashOverlay = SlashHUD->GetSlashOverlay();//here if we dont set an slashoverlay type hud into our setup, we wont be able to cast it here
				if (SlashOverlay && Attributes)
				{
					SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
					SlashOverlay->SetStaminaBarPercent(1.f);
					SlashOverlay->SetGold(0);
					SlashOverlay->SetSouls(0);
				}
			}
		}
	}
}



void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}