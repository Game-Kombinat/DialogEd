#include "CommandNode.h"

UCommandNode::UCommandNode() {
    token = FParsedToken(ETokenType::Command, "", 0, 0, 0);
}
