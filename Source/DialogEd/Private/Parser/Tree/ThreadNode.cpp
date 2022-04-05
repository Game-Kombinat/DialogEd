// Fill out your copyright notice in the Description page of Project Settings.


#include "Parser/Tree/ThreadNode.h"

void UThreadNode::Init(FParsedToken t) {
    token = t;
    // removes the >>>> identifier and trims off possible whitespace
    threadName = t.value.RightChop(4).TrimStartAndEnd();
}

UDialogNode* UThreadNode::GetFirstNode() const {
    return left;
}
