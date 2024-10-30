// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

UCLASS()
class TP3SHOOT_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCustomAIPerceptionComponent* CustomAIPerceptionComponent;

public:
	AAIControllerBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	UFUNCTION()
	void OnPerceptionChanged(AActor* InActor, uint8 StimulusID, bool bWasSuccessfullySensed);
	UFUNCTION()
	void OnPerceptionAgedPassed(AActor* InActor, uint8 StimulusID);

public:
	UCustomAIPerceptionComponent* GetAIPerception() const { return CustomAIPerceptionComponent; }
};
