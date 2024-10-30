#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "CustomAIPerceptionComponent.generated.h"


class UAISenseConfig_Sight;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAIPerceptionChangedSignature,AActor*,InActor, uint8, StimulusID, bool,bWasSuccessfullySensed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIPerceptionAgedPassedSignature,AActor*,InActor, uint8, StimulusID);


USTRUCT()
struct FPerceptionData
{
	GENERATED_BODY()

private:
	FAIStimulus Stimulus;

public:
	FTimerDelegate PerceptionDelegateHandle;
	FTimerHandle TimerPerceptionHandle;


	FPerceptionData(){}
	FPerceptionData(const FAIStimulus& InStimulus): Stimulus(InStimulus){}

	FORCEINLINE float GetAge() const { return Stimulus.GetAge(); }
	/** @return false when this stimulus is no longer valid, when it is Expired */
	bool AgeStimulus(const float InValue)
	{
		return Stimulus.AgeStimulus(InValue);
	}

	void ClearTimers(FTimerManager& TimerManager)
	{
		Stimulus.SetStimulusAge(0);
		if (!TimerManager.TimerExists(TimerPerceptionHandle))return;

		TimerManager.ClearTimer(TimerPerceptionHandle);
		PerceptionDelegateHandle.Unbind();
		TimerPerceptionHandle.Invalidate();
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TP3SHOOT_API UCustomAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	UCustomAIPerceptionComponent();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAIPerceptionChangedSignature OnAIPerceptionChanged_Event;

	UPROPERTY(BlueprintAssignable)
	FOnAIPerceptionAgedPassedSignature OnAIPerceptionAgedPassed_Event;

public:
	FORCEINLINE UAISenseConfig_Sight* GetSightConfig() const { return SenseConfig_Sight; }

	FORCEINLINE void SetTargetSight(const TSubclassOf<AActor>& InTargetSight) { Target_Sight = InTargetSight; }
	FORCEINLINE TSubclassOf<AActor> GetTargetSight() const { return Target_Sight; }

protected:
	UFUNCTION()
	void OnPerceptionHandleUpdate(const uint8& InStimulusID,  AActor* InActor);

	UFUNCTION()
	void OnTargetPerceptionChanged(AActor* InActor, FAIStimulus InStimulus);

	void OnUpdatePerception_Sight( AActor* InActor, FAIStimulus InStimulus);

	FORCEINLINE FTimerManager& GetTimerManger() const { return GetWorld()->GetTimerManager(); }

protected:
	virtual void BeginPlay() override;

	void RegisterStimulus(const uint8& InStimulusID, const FAIStimulus& InStimulus);

private:
	UPROPERTY()
	TSubclassOf<AActor> Target_Sight;

	UPROPERTY()
	UAISenseConfig_Sight* SenseConfig_Sight;

	UPROPERTY()
	TMap<uint8, FPerceptionData> StimulusMap;


	static constexpr float UpdateStimulusRate = 0.15F;
};
