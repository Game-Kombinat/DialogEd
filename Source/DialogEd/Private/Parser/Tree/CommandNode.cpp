#include "CommandNode.h"

DialogEd::FCommandNode::FCommandNode() {
    token = FParsedToken(ETokenType::Command, "", 0, 0, 0);
}
