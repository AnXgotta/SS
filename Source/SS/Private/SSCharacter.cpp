// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "SS.h"
#include "SSCharacter.h"
#include "SSProjectile.h"
#include "Animation/AnimInstance.h"


//////////////////////////////////////////////////////////////////////////
// ASSCharacter Constructor

ASSCharacter::ASSCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP){

	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Configure character movement
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->TargetOffset.Z = 30.0f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	CameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
	CameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComponent->bUsePawnControlRotation = true;

	
	Mesh->SetOnlyOwnerSee(false);
	Mesh->AttachParent = CapsuleComponent;
	Mesh->RelativeLocation = FVector(0.f, 0.f, -150.f);
	Mesh->bCastDynamicShadow = true;
	Mesh->CastShadow = true;

	// replication
	bReplicateMovement = true;
	bReplicates = true;




	
	


	

}



////////////////////////////////////////////////////////////////////////
//  Player Initialization

void ASSCharacter::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);

	CrouchImplementation(DeltaSeconds);

}

void ASSCharacter::PostInitializeComponents(){
	Super::PostInitializeComponents();
	InitializePlayer();
}

void ASSCharacter::InitializePlayer(){

	CrouchInterpValue = 0.0f;
	CachedCapsuleHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	
	// character movement setup	
	if (CharacterMovement){

		CharacterMovement->MaxStepHeight = 45.0f;
		CharacterMovement->JumpZVelocity = JUMP_VELOCITY * 100.0f;
		CharacterMovement->SetWalkableFloorAngle(45.0f);
		CharacterMovement->GravityScale = 1.0;
		CharacterMovement->GroundFriction = 8.0f;
		CharacterMovement->MaxWalkSpeed = MAX_WALK_SPEED * 100.0f;
		CharacterMovement->MaxWalkSpeedCrouched = MAX_WALK_SPEED * CROUCH_SPEED_MULTIPLIER;
		CharacterMovement->MaxCustomMovementSpeed = 600.0f;
		CharacterMovement->MaxSwimSpeed = 300.0f;
		CharacterMovement->MaxAcceleration = 2048.0f;
		CharacterMovement->BrakingDecelerationWalking = 2048.0f;
		CharacterMovement->BrakingDecelerationFalling = 0.0f;
		CharacterMovement->BrakingDecelerationSwimming = 0.0f;
		CharacterMovement->AirControl = 0.05f;
		CharacterMovement->FallingLateralFriction = 0.0f;
		CharacterMovement->CrouchedHalfHeight = 40.0f;
		CharacterMovement->Buoyancy = 0.0f;
		CharacterMovement->bOrientRotationToMovement = false;
		CharacterMovement->bCanWalkOffLedges = true;
		CharacterMovement->bCanWalkOffLedgesWhenCrouching = true;
		CharacterMovement->Mass = 100.0f;
		CharacterMovement->bMaintainHorizontalGroundVelocity = true;
		CharacterMovement->bImpartBaseVelocityX = true;
		CharacterMovement->bImpartBaseVelocityY = true;
		CharacterMovement->bImpartBaseVelocityZ = true;
		CharacterMovement->bImpartBaseAngularVelocity = true;
		CharacterMovement->bIgnoreBaseRotation = false;
		CharacterMovement->bAlwaysCheckFloor = true;
		CharacterMovement->JumpOffJumpZFactor = 0.5f;
		CharacterMovement->PerchRadiusThreshold = 0.0f;
		CharacterMovement->PerchAdditionalHeight = 40.0f;
		CharacterMovement->bUseControllerDesiredRotation = false;
		CharacterMovement->bEnableScopedMovementUpdates = true;
		CharacterMovement->bUseFlatBaseForFloorChecks = false;
		CharacterMovement->bRequestedMoveUseAcceleration = true;
		// 4.5 CONVERSION CharacterMovement->bCrouchMovesCharacterDown = true;
		
	}
	// sprint setup
	SprintMultiplier = 1.5f;
	CachedDefaultWalkSpeed = CharacterMovement->MaxWalkSpeed;


}




////////////////////////////////////////////////////////////////////////
//  Player Movement

void ASSCharacter::MoveForward(float Value){	
	if (Value != 0.0f){
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASSCharacter::MoveRight(float Value){
	if (Value != 0.0f){
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASSCharacter::SprintStart(){
	if (Role != ROLE_Authority){
		ServerSprintStart();
		return;
	}
	if (CharacterMovement){
		CharacterMovement->MaxWalkSpeed = CachedDefaultWalkSpeed * SprintMultiplier;
	}	
}

bool ASSCharacter::ServerSprintStart_Validate(){
	return true;
}

void ASSCharacter::ServerSprintStart_Implementation(){
	SprintStart();
}

void ASSCharacter::SprintEnd(){
	if (Role != ROLE_Authority){
		ServerSprintEnd();
		return;
	}
	if (CharacterMovement){
		CharacterMovement->MaxWalkSpeed = CachedDefaultWalkSpeed;
	}	
}

bool ASSCharacter::ServerSprintEnd_Validate(){
	return true;
}

void ASSCharacter::ServerSprintEnd_Implementation(){
	SprintEnd();
}

void ASSCharacter::CrouchStart(){	
	if (Role != ROLE_Authority){
		ServerCrouchStart();		
		return;
	}

	bIsCrouching = true;
	bIsPlayerCrouched = false;
}

bool ASSCharacter::ServerCrouchStart_Validate(){
	return true;
}

void ASSCharacter::ServerCrouchStart_Implementation(){
	CrouchStart();
}

void ASSCharacter::CrouchEnd(){
	if (Role != ROLE_Authority){
		ServerCrouchEnd();		
		return;
	}

	bIsCrouching = false;
	bIsPlayerCrouched = true;
}

bool ASSCharacter::ServerCrouchEnd_Validate(){
	return true;
}

void ASSCharacter::ServerCrouchEnd_Implementation(){
	CrouchEnd();
}

void ASSCharacter::CrouchImplementation(float DeltaSeconds){
	if (bIsCrouching){
		if (bIsPlayerCrouched) return;

		if (CrouchInterpValue >= 1.0f){
			CrouchInterpValue = 1.0f;
			bIsPlayerCrouched = true;
			return;
		}

		if (CharacterMovement && CharacterMovement->IsMovingOnGround()){			
			CrouchInterpValue = FMath::Clamp(CrouchInterpValue + DeltaSeconds * 5.0f, 0.0f, 1.0f);
			// new dimensions
			float const NewHalfHeight = CachedCapsuleHalfHeight - (CrouchInterpValue * (CachedCapsuleHalfHeight - (CachedCapsuleHalfHeight - CrouchedEyeHeight)));
			CapsuleComponent->SetCapsuleSize(CapsuleComponent->GetScaledCapsuleRadius(), NewHalfHeight, true);			
		}
	}else{
		if (!bIsPlayerCrouched) return;

		if (CrouchInterpValue <= 0.0f){
			CrouchInterpValue = 0.0f;
			bIsPlayerCrouched = false;
			return;
		}

		//  used to check if player is trying to stand while under something
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Crouch_Trace")), true, this);
		FHitResult HitResult(ForceInit);
		const FVector StartPos = GetActorLocation();
		const FVector EndPos = StartPos + CapsuleComponent->GetUpVector() * CapsuleComponent->GetScaledCapsuleHalfHeight() + 5.0f;
		// return if player under object
		if (GetWorld()->LineTraceSingle(HitResult, StartPos, EndPos, TraceParams, NULL)){
			return;
		}		

		if (CharacterMovement && CharacterMovement->IsMovingOnGround()){
			CrouchInterpValue = FMath::Clamp(CrouchInterpValue - DeltaSeconds * 5.0f, 0.0f, 1.0f);
			// new dimensions
			float const NewHalfHeight = CachedCapsuleHalfHeight - (CrouchInterpValue * (CachedCapsuleHalfHeight - (CachedCapsuleHalfHeight - CrouchedEyeHeight)));
			CapsuleComponent->SetCapsuleSize(CapsuleComponent->GetScaledCapsuleRadius(), NewHalfHeight, true);
		}
	}
}

void ASSCharacter::ManagePitch(float Val){
	APawn::AddControllerPitchInput(Val);
	/*  TODO:  Add Rotation Restrictions
	if (!Controller) return;

	float ControllerPitch = Controller->GetControlRotation().Pitch;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("CPitch = %f, Val = %f, Camera Higher than Boom = %d"), ControllerPitch, Val, CameraBoom->GetComponentLocation().Z < CameraComponent->GetComponentLocation().Z));

	if (CameraComponent->GetComponentLocation().Z >= CameraBoom->GetComponentLocation().Z){
		if (ControllerPitch < 300.0f){
			APawn::AddControllerPitchInput(FMath::Min(0.0f, Val));
		}else{
			APawn::AddControllerPitchInput(Val);
		}
		return;
	}

	if (CameraComponent->GetComponentLocation().Z <= CameraBoom->GetComponentLocation().Z){
		if (ControllerPitch > 60.0f){
			APawn::AddControllerPitchInput(FMath::Max(0.0f, Val));
		}else{
			APawn::AddControllerPitchInput(Val);
		}
		return;
	}	

	*/
}

////////////////////////////////////////////////////////////////////////
// Player Interaction

void ASSCharacter::TraceForInteraction(){
	if (Role == ROLE_Authority || Role == ROLE_SimulatedProxy) return;
	USSConstants::ScreenMessage("Tracing...", 5.0f, FColor::Green);
	FHitResult HitData(ForceInit);
	
	const FVector Start = this->CameraComponent.Get()->GetComponentLocation();
	const FVector End = Start + GetControlRotation().Vector() * INTERACTION_RANGE;

	if (USSConstants::TraceInteractable(this, Start, End, HitData)){
		if (HitData.GetActor()){
			USSConstants::ScreenMessage(HitData.GetActor()->GetName(), 5.0f, FColor::Red);
		}
	}
}

void ASSCharacter::Interact(){
	TraceForInteraction();
}

bool ASSCharacter::ServerInteract_Validate(){
	return true;
}

void ASSCharacter::ServerInteract_Implementation(){

}

bool ASSCharacter::ServerInteractResponse_Validate(bool bInteract){
	return true;
}

void ASSCharacter::ServerInteractResponse_Implementation(bool bInteract){

}


//////////////////////////////////////////////////////////////////////////
// Player Input Management

void ASSCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ASSCharacter::SprintStart);
	InputComponent->BindAction("Sprint", IE_Released, this, &ASSCharacter::SprintEnd);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &ASSCharacter::CrouchStart);
	InputComponent->BindAction("Crouch", IE_Released, this, &ASSCharacter::CrouchEnd);

	InputComponent->BindAction("Interact", IE_Pressed, this, &ASSCharacter::Interact);

	InputComponent->BindAxis("MoveForward", this, &ASSCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASSCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &ASSCharacter::ManagePitch);
}


/////////////////////////////////////////////////////////////////////////
//  Player Replicated Props

void ASSCharacter::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASSCharacter, PlayerVitals, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(ASSCharacter, PlayerClothes, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASSCharacter, PlayerBelt, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASSCharacter, PlayerPack, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(ASSCharacter, CurrentInteactableItem, COND_OwnerOnly);

	DOREPLIFETIME(ASSCharacter, bIsCrouching);
	DOREPLIFETIME(ASSCharacter, bIsPlayerCrouched);

}

///////////////////////////////////////////////////////
//  Accessor/Mutator

FVitalsStruct ASSCharacter::GetPlayerVitals(){
	return PlayerVitals;
}
