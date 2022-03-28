#include "TextNode.h"

DialogEd::FTextNode::FTextNode(FParsedToken t) {
    token = t;
    value = t.value.TrimChar('"');
}
