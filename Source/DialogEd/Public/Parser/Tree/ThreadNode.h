// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogNode.h"
#include "ThreadNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGED_API UThreadNode : public UDialogNode {
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere)
    FString threadName;

    void Init(FParsedToken t);
    UDialogNode* GetFirstNode() const;

};
