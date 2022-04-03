﻿#pragma once
#include "Parser/ParsedToken.h"
#include "DialogNode.generated.h"

UCLASS()
class UDialogNode : public UObject {
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere)
    FParsedToken token;
    UPROPERTY(VisibleAnywhere)
    UDialogNode* left;
    UPROPERTY(VisibleAnywhere)
    UDialogNode* right;
    UDialogNode();
    // ~UDialogNode();
    FString ToString();

    FString ToString(int indent);
};