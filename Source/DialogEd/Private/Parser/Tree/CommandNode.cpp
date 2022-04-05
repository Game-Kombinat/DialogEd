#include "Parser/Tree/CommandNode.h"

#include "Parser/Tree/BinOpNode.h"
#include "Parser/Tree/IdentifierNode.h"

UCommandNode::UCommandNode() {
    token = FParsedToken(ETokenType::Command, "", 0, 0, 0);
}

FString UCommandNode::GetCommandName() const {
    return static_cast<UIdentifierNode*>(left)->identifierLabel;
}

TArray<UBinOpNode*> UCommandNode::GetArgs() const {
    TArray<UBinOpNode*> args;
    auto arg = right;
    while (arg) {
        args.Add(static_cast<UBinOpNode*>(arg->left));
        arg = arg->right;
    }

    Algo::Reverse(args);
    return args;
}
