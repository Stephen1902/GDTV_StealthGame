// Copyright 2025 DME Games


#include "Enemies/EnemyCharacterBase.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "Player/StealthCharacter.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TakeDownRadius = CreateDefaultSubobject<USphereComponent>("Takedown Radius");
	TakeDownRadius->SetupAttachment(GetRootComponent());
	TakeDownRadius->SetRelativeLocation(FVector(-48.f, 0.f, 0.f));
	TakeDownRadius->SetSphereRadius(48.f);
	
	RefMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Reference Mesh");
	RefMesh->SetupAttachment(GetRootComponent());
	RefMesh->SetRelativeLocation(FVector(-56.f, 1.f, -89.f));
	RefMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	RefMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RefMesh->SetHiddenInGame(true);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget Comp");
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetVisibility(false);	

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FoundMontage(TEXT("/Game/Player/Animations/Takedown/Takedown_Victim_Montage"));
	if (FoundMontage.Succeeded())
	{
		TakeDownMontage = FoundMontage.Object;
	}

	bCanBeTakenDown = true;
		
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	TakeDownRadius->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacterBase::OnTakeDownOverlapBegin);
	TakeDownRadius->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacterBase::OnTakeDownOverlapEnd);
	
}
// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacterBase::CanTakeDown_Implementation(FVector& ActorLocation, FRotator& ActorRotation, bool& CanTakeDown, AActor*& IgnoredActor)
{
	IGuardInterface::CanTakeDown_Implementation(ActorLocation, ActorRotation, CanTakeDown, IgnoredActor);

	if (bCanBeTakenDown && TakeDownMontage)
	{
		PlayAnimMontage(TakeDownMontage);

		const float AnimLength = TakeDownMontage->GetPlayLength();
		GetWorldTimerManager().SetTimer(RagdollDelayTimer, this, &AEnemyCharacterBase::RagdollEnemy, AnimLength, false, AnimLength);
	}

	WidgetComponent->SetVisibility(false);
	TakeDownRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ActorLocation = RefMesh->GetComponentLocation();
	ActorRotation = GetActorRotation();
	CanTakeDown = bCanBeTakenDown;
	IgnoredActor = this;
}

void AEnemyCharacterBase::RagdollEnemy()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void AEnemyCharacterBase::OnTakeDownOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		WidgetComponent->SetVisibility(true);
	}
}

void AEnemyCharacterBase::OnTakeDownOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		WidgetComponent->SetVisibility(false);
	}
}

