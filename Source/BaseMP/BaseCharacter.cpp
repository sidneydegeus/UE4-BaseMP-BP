// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Engine.h"

// NOTE:
// All inputs from players should be handled with Server (e.g. shooting, weapon swaps etc) NOT NetMulticast.
// Events that happen on the server (for example a landmine on the floor explodes. This needs a NetMulticast.

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, Health);
	DOREPLIFETIME(ABaseCharacter, EquipedWeapon);
	DOREPLIFETIME(ABaseCharacter, EquipedWeaponClass);
}

// Sets default values
ABaseCharacter::ABaseCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::UpdateCharacterStatus_Implementation() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
}

#pragma region Weapon

void ABaseCharacter::ChangeWeapon(TSubclassOf<ABaseWeapon> WeaponClass, ACharacter* playerCharacter) {
	if (Role < ROLE_Authority) {
		ChangeWeaponServer(WeaponClass, playerCharacter);
	}
	EquipedWeaponClass = WeaponClass;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("a"));
	ChangeWeaponEvent(playerCharacter);
	//GetPlayerControllerFromNetId();
}

void ABaseCharacter::ChangeWeaponEvent_Implementation(ACharacter* playerCharacter) {
	// empty so that it can be overwritten in blueprint
}

void ABaseCharacter::ChangeWeaponServer_Implementation(TSubclassOf<ABaseWeapon> WeaponClass, ACharacter* playerCharacter) {
	ChangeWeapon(WeaponClass, playerCharacter);
}

bool ABaseCharacter::ChangeWeaponServer_Validate(TSubclassOf<ABaseWeapon> WeaponClass, ACharacter* playerCharacter) {
	return true;
}
#pragma endregion Weapon

#pragma region Health
void ABaseCharacter::InitializeHealth() {
	if (Role == ROLE_Authority) {
		InitializeHealthServer();
		UpdateCharacterStatus();
	}
}

void ABaseCharacter::InitializeHealthServer_Implementation() {
	Health = 100;
}

bool ABaseCharacter::InitializeHealthServer_Validate(void) {
	return true;
}

void ABaseCharacter::IncreaseHealth(float amount) {
	if (Role == ROLE_Authority) {
		float actualAmount = amount;
		CalculateHealthServer(actualAmount);
		UpdateCharacterStatus();
	}
}

void ABaseCharacter::DecreaseHealth(float amount) {
	if (Role == ROLE_Authority) {
		float actualAmount = amount * -1;
		CalculateHealthServer(actualAmount);
		UpdateCharacterStatus();
	}
}

void ABaseCharacter::CalculateHealthServer_Implementation(float amount) {
	Health += amount;
}

bool ABaseCharacter::CalculateHealthServer_Validate(float delta) {
	return true;
}
#pragma endregion Health

void ABaseCharacter::OnTakeDamage(float Damage) {
	if (Role == ROLE_Authority) {
		DecreaseHealth(Damage);
	}
}
