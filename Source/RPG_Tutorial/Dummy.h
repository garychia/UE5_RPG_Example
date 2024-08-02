// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AssassinatableInterface.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Dummy.generated.h"

class USoundBase;

UCLASS(Blueprintable)
class RPG_TUTORIAL_API ADummy : public ACharacter, public IAssassinatableInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	UWidgetComponent* AssassinationWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	USphereComponent* AssassinationArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	USkeletalMeshComponent* AssassinationLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* AssassinatedAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<UAnimMontage*> HitAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	USoundBase* GruntSoundWave;

	// Sets default values for this character's properties
	ADummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount,
		FDamageEvent const&		   DamageEvent,
		AController*			   EventInstigator,
		AActor*					   DamageCauser) override;

	void GetAssassinated_Implementation(FVector& Location, FRotator& Rotation) override;

	UFUNCTION()
	void OnAssassinationAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAssassinationAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
