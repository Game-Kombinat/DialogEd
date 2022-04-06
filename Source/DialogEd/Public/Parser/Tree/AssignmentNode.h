#pragma once
#include "DialogNode.h"
#include "AssignmentNode.generated.h"

UCLASS()
class DIALOGED_API UAssignmentNode : public UDialogNode {
    GENERATED_BODY()
public:
    UAssignmentNode();
    UAssignmentNode(FParsedToken t, UDialogNode* lhs, UDialogNode* rhs);

    void Init(FParsedToken t, UDialogNode* lhs, UDialogNode* rhs);
};
