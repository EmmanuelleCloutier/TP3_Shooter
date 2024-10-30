#include "TP3Shoot/Public/AIControllerBase.h"

#include "TP3Shoot/CustomAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Sight.h"
#include "TP3Shoot/TP3ShootCharacter.h"


AAIControllerBase::AAIControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CustomAIPerceptionComponent = CreateDefaultSubobject<UCustomAIPerceptionComponent>(TEXT("CustomAIPerceptionComponent"));

	CustomAIPerceptionComponent->SetTargetSight(ATP3ShootCharacter::StaticClass());
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		CustomAIPerceptionComponent->OnAIPerceptionChanged_Event.AddUniqueDynamic(
			this, &AAIControllerBase::OnPerceptionChanged);
		CustomAIPerceptionComponent->OnAIPerceptionAgedPassed_Event.AddUniqueDynamic(
			this, &AAIControllerBase::OnPerceptionAgedPassed);
		
	}

	
	if(const ATP3ShootCharacter* CastCharacter = Cast<ATP3ShootCharacter>(InPawn))
	{
		SetGenericTeamId(CastCharacter->TeamID);
	}
}

void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
	if (BehaviorTree)
	{
		CleanupBrainComponent();
		CustomAIPerceptionComponent->OnAIPerceptionChanged_Event.RemoveDynamic(
			this, &AAIControllerBase::OnPerceptionChanged);
		CustomAIPerceptionComponent->OnAIPerceptionAgedPassed_Event.RemoveDynamic(
			this, &AAIControllerBase::OnPerceptionAgedPassed);
	}
}

void AAIControllerBase::OnPerceptionChanged(AActor* InActor, uint8 StimulusID, bool bWasSuccessfullySensed)
{
	if (bWasSuccessfullySensed)
	{
		const FName TargetActor{TEXT("TargetActor")};
		Blackboard->SetValueAsObject(TargetActor, InActor);
	}
}

void AAIControllerBase::OnPerceptionAgedPassed(AActor* InActor, uint8 StimulusID)
{
	if (StimulusID == UAISense::GetSenseID<UAISense_Sight>())
	{
		const FName TargetActor{TEXT("TargetActor")};
		Blackboard->ClearValue(TargetActor);
		
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			                                 FString::Printf(TEXT("integer value is: %d"), 0));
	}
}
