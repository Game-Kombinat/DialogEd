// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TokenizerTester.generated.h"

UCLASS()
class DIALOGED_API ATokenizerTester : public AActor {
    GENERATED_BODY()
protected:
    UPROPERTY(VisibleAnywhere)
    TArray<class UDialogNode*> nodes;
public:
    // Sets default values for this actor's properties
    ATokenizerTester();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
