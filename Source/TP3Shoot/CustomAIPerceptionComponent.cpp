#include "CustomAIPerceptionComponent.h"

#include "Perception/AISenseConfig_Sight.h"


UCustomAIPerceptionComponent::UCustomAIPerceptionComponent()
{
	//UAISenseConfig_Sight Default Setting
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SenseConfig_Sight->SightRadius = 3000.0f;
	SenseConfig_Sight->LoseSightRadius = 3500.0f;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = 75.0f;
	SenseConfig_Sight->SetMaxAge(3.0F);

	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	ConfigureSense(*SenseConfig_Sight);
	//No pref sense
	SetDominantSense(nullptr);
}

void UCustomAIPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
	OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnTargetPerceptionChanged);
}

void UCustomAIPerceptionComponent::OnTargetPerceptionChanged(AActor* InActor, FAIStimulus InStimulus)
{
	OnUpdatePerception_Sight(InActor, InStimulus);
	//Other sense added here
}

void UCustomAIPerceptionComponent::OnUpdatePerception_Sight( AActor* InActor, FAIStimulus InStimulus)
{
	RegisterStimulus(InStimulus.Type, InStimulus);

	const bool bIsTargetStimulus = InStimulus.Type == UAISense::GetSenseID<UAISense_Sight>();

	if (bIsTargetStimulus && InActor->IsA(Target_Sight))
	{
		if (FPerceptionData* StimulusData = StimulusMap.Find(InStimulus.Type))
		{
			if (!InStimulus.WasSuccessfullySensed())
			{
				if (SenseConfig_Sight->GetMaxAge() > 0)
				{
					StimulusData->PerceptionDelegateHandle.BindUFunction(this, FName("OnPerceptionHandleUpdate"), InStimulus.Type);
					GetTimerManger().SetTimer(StimulusData->TimerPerceptionHandle,StimulusData->PerceptionDelegateHandle, UpdateStimulusRate, true);
				}

				OnAIPerceptionChanged_Event.Broadcast(InActor,InStimulus.Type, false);
			}
			else
			{
				//Mean we have successfully sensed the target
				StimulusData->ClearTimers(GetTimerManger());
				OnAIPerceptionChanged_Event.Broadcast(InActor,InStimulus.Type, true);
			}
		}
	}
}

void UCustomAIPerceptionComponent::OnPerceptionHandleUpdate(const uint8& InStimulusID,  AActor* InActor)
{
	if (FPerceptionData* StimulusData = StimulusMap.Find(InStimulusID))
	{
		if (!StimulusData->AgeStimulus(UpdateStimulusRate))
		{
			StimulusData->ClearTimers(GetTimerManger());

			OnAIPerceptionAgedPassed_Event.Broadcast(InActor,InStimulusID);
		}
	}
}


void UCustomAIPerceptionComponent::RegisterStimulus(const uint8& InStimulusID, const FAIStimulus& InStimulus)
{
	//Mean we dont want to update the stimulus
	if (!StimulusMap.Contains(InStimulusID))
	{
		StimulusMap.Add(InStimulusID, FPerceptionData(InStimulus));
	}
}
