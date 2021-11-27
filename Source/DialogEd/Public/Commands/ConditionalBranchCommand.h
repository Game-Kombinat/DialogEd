// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueCommand.h"
#include "GameDataContext.h"
#include "Kismet/KismetStringLibrary.h"
#include "ConditionalBranchCommand.generated.h"
USTRUCT()
struct FOperand {
    GENERATED_BODY()
    int number;
    FString key;
    bool isNumber;
    
    UPROPERTY()
    UGameDataContext* dc;

    FOperand() {
        key = FString();
        dc = nullptr;
        isNumber = true; // cause that's less likely to break fucken everything in case somebody does use a default FOperand
        number = -1;
    }
    
    FOperand(FString input, UGameDataContext* inDc) {
        key = input;
        dc = inDc;
        if (input.IsNumeric()) {
            number = UKismetStringLibrary::Conv_StringToInt(input);
            isNumber = true;
        }
        else {
            isNumber = false;
        }
    }
    
    int GetValue() const {
        return isNumber ? number : dc->GetValue(key);
    }
};

/**
 * 
 */
UCLASS()
class DIALOGED_API UConditionalBranchCommand : public UDialogueCommand {
    GENERATED_BODY()
public:
    UConditionalBranchCommand();
    // this is Lhs [operator] Rhs
    // makes exactly 3 arguments, always.
    virtual int MinArguments() override { return 3; }
    virtual int MaxArguments() override { return 3; }

    virtual void Execute(TSharedRef<FParsedCommand>) override;
    virtual bool IsFinished() override;
    virtual void Cleanup() override;
private:
    FOperand MakeOperand(const FString& keyName, UGameDataContext* dc) const;
};
