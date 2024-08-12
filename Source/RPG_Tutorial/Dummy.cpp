// Fill out your copyright notice in the Description page of Project Settings.

#include "Dummy.h"
#include "RPG_TutorialCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerStats.h"
#include "PlayerHUD.h"

// Sets default values
ADummy::ADummy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Assassination Components
	AssassinationWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Assassination Widget"));
	StatsWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Stats Widget"));
	Stats = CreateDefaultSubobject<UPlayerStats>(TEXT("Stats"));
	AssassinationArea = CreateDefaultSubobject<USphereComponent>(TEXT("Assassination Area"));
	AssassinationLocation = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Assassination Location"));

	AssassinationWidget->SetupAttachment(RootComponent);
	StatsWidget->SetupAttachment(RootComponent);
	AssassinationArea->SetupAttachment(RootComponent);
	AssassinationLocation->SetupAttachment(RootComponent);

	AssassinationArea->OnComponentBeginOverlap.AddDynamic(this, &ADummy::OnAssassinationAreaBeginOverlap);
	AssassinationArea->OnComponentEndOverlap.AddDynamic(this, &ADummy::OnAssassinationAreaEndOverlap);

	Stats->SetPlayer(this);
	Stats->OnReachZeroHealth.AddDynamic(this, &ADummy::Die);
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();

	AssassinationWidget->SetVisibility(false);
	AssassinationLocation->SetVisibility(false);

	if (StatsClass)
	{
		StatsWidget->SetWidgetClass(StatsClass);
		Stats->SetPlayerHUD(Cast<UPlayerHUD>(StatsWidget->GetWidget()));
	}
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ADummy::TakeDamage(float DamageAmount,
	FDamageEvent const&		   DamageEvent,
	AController*			   EventInstigator,
	AActor*					   DamageCauser)
{
	if (!HitAnimations.IsEmpty())
	{
		PlayAnimMontage(HitAnimations[UKismetMathLibrary::RandomInteger(HitAnimations.Num())]);
	}
	Stats->DecreaseHealth(DamageAmount);

	return DamageAmount;
}

void ADummy::GetAssassinated_Implementation(FVector& Location, FRotator& Rotation)
{
	PlayAnimMontage(AssassinatedAnim);

	FTimerHandle DeathTimerHandle, GruntTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DeathTimerHandle,
		[&]() {
			GetMesh()->SetSimulatePhysics(true);
			AssassinationWidget->SetVisibility(false);
		},
		2.5f,
		false);
	GetWorld()->GetTimerManager().SetTimer(
		GruntTimerHandle,
		[&]() {
			if (GruntSoundWave)
			{
				UGameplayStatics::PlaySoundAtLocation(this, GruntSoundWave, GetActorLocation());
				Stats->DecreaseHealth(Stats->CurrentHealth);
			}
		},
		1.5f,
		false);

	Location = AssassinationLocation->GetComponentLocation();
	Rotation = GetActorRotation();
}

void ADummy::OnAssassinationAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ARPG_TutorialCharacter>(OtherActor))
	{
		AssassinationWidget->SetVisibility(true);
	}
}

void ADummy::OnAssassinationAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ARPG_TutorialCharacter>(OtherActor))
	{
		AssassinationWidget->SetVisibility(false);
	}
}

void ADummy::Die()
{
	GetMesh()->SetSimulatePhysics(true);
}
