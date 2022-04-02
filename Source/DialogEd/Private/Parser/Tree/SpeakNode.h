#pragma once
#include "DialogNode.h"
#include "SpeakNode.generated.h"

UCLASS()
class USpeakNode : public UDialogNode {
    GENERATED_BODY()
public:
    USpeakNode();
    USpeakNode(class UIdentifierNode* speaker, class UTextNode* text);

    void Init(class UIdentifierNode* speaker, class UTextNode* text);
};