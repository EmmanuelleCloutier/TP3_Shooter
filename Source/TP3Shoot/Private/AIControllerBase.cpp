// Fill out your copyright notice in the Description page of Project Settings.


#include "TP3Shoot/Public/AIControllerBase.h"


// Sets default values
AAIControllerBase::AAIControllerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

