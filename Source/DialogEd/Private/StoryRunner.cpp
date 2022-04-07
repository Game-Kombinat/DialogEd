// Copyright (c) Game Kombinat


#include "StoryRunner.h"

#include "ActorRegister.h"
#include "DialogueCommandRegister.h"
#include "Logging.h"
#include "StoryAsset.h"
#include "VariableTypeHandler.h"
#include "VariableTypeRegister.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Parser/Tree/BinOpNode.h"
#include "Parser/Tree/ChoiceNode.h"
#include "Parser/Tree/CommandNode.h"
#include "Parser/Tree/SpeakNode.h"
#include "Parser/Tree/ThreadNode.h"


// Sets default values for this component's properties
UStoryRunner::UStoryRunner() {
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    // currentThread = emptyThread;
}


// Called when the game starts
void UStoryRunner::BeginPlay() {
    Super::BeginPlay();
    // todo: we will want the data context from the outside.
    dataContext->PrepareRuntimeData();
    actorRegister = NewObject<UActorRegister>(this);
    actorRegister->Init(GetWorld(), actorMapData);

    commandRegister = NewObject<UDialogueCommandRegister>(this);
    commandRegister->Init(GetWorld(), commandMapData);

    variableTypeRegister = NewObject<UVariableTypeRegister>(this);
    variableTypeRegister->Init(this, variableTypeMapData);
    SetComponentTickEnabled(false);
}

void UStoryRunner::HandleActorsInThread() {
    actorsInActiveThread.Empty();
    // todo: this should maybe get baked into the ThreadNode object,
    // which actors are used under it. In a post processing step after parsing and all.
    // auto currentThread = currentThread[currentThread.Num() - 1];
    // if (!currentThread) {
    //     LOG_ERROR("Cannot handle actors in current thread - there is no current thread!");
    //     return;
    // }
    // for (const auto a : currentThread->actorsInThread) {
    //     if (a) {
    //         actorsInActiveThread.Add(a);
    //     }
    // }
}

void UStoryRunner::CountRan(const UThreadNode* thread) const {
    const FString ranKey = FString::Format(TEXT("{0}__ran"), {thread->GetName()});
    int current = dataContext->GetValue(ranKey);
    if (current < 0) {
        current = 0;
    }
    
    dataContext->ForceSetValue(ranKey, current + 1);
}

UGameDataContext* UStoryRunner::GetDataContext() {
    return dataContext;
}

bool UStoryRunner::HasNext() const {
    // returns true if the next node is not a choice node.
    return (currentNode && currentNode->right) || branchNodeStack.Num() > 0;
}

int UStoryRunner::GetVariableValue(const FString& type, const FString& name) const {
    if (type.IsEmpty()) {
        return dataContext->GetValue(name);
    }
    return variableTypeRegister->GetHandler(type)->GetValue(name);
}

void UStoryRunner::SetVariableValue(const FString& type, const FString& name, const int value) const {
    if (type.IsEmpty()) {
        dataContext->ForceSetValue(name, value);
    }
    variableTypeRegister->GetHandler(type)->SetValue(name, value);
}

bool UStoryRunner::HandleIfElseBranching() {
    // left is expression
    // right is node with: left: true branch, right: false branch
    const auto ifNode = static_cast<UBinOpNode*>(currentNode->left->left);
    const auto ifBranches = currentNode->left->right;
    if (!ifNode || !ifBranches) {
        LOG_ERROR("Expected binop node for if branch. didn't get any. this should not go through the parser!");
        return false;
    }
    branchNodeStack.Push(currentNode);
    if (ifNode->Evaluate(this) > 0) {
        currentNode = ifBranches->left;
    }
    else {
        if (ifBranches->right) {
            currentNode = ifBranches->right;
        }
        else {
            LOG_INFO("No Else Branch. Popping from branch stack.")
            currentNode = branchNodeStack.Pop();
        }
    }
    return true;
}

bool UStoryRunner::HandleBranchingNodeTypes() {
    // Check if it's an if token
    if (currentNode->left->token.tokenType == ETokenType::If) {
        if (!HandleIfElseBranching()) {
            return false;
        }
    }

    // check if it's a branching token
    if (currentNode->left->token.tokenType == ETokenType::BeginBranching) {
        // left: speech node and
        // right: choice node
        // on choice node we get:
        // left: branch
        // right: next choice (if any)
        // and it has a label field for the choice on the left

        // we push the current node, so that when whatever chosen branch is finished,
        // we end up up here and can continue.

        // There is nothing else we need to do here.
        LOG_INFO("Found choice. Pushing root node for later.")
        branchNodeStack.Push(currentNode);
    }
    return true;
}

void UStoryRunner::ShiftToNextNode() {
    // right is always a node that has
    // left: its logic
    // right: next node
    // this creates a chain of commands where going down right goes forward.

    // first, enqueue next token.
    if (!currentNode->right) {
        LOG_INFO("Popping from branch stack");
        currentNode = branchNodeStack.Pop();
        currentNode = currentNode->right;
        
    }
    else {
        LOG_INFO("Continue branch flow");
        currentNode = currentNode->right;
        if (!currentNode->left && !currentNode->right && branchNodeStack.Num() > 0) {
            LOG_INFO("Found End Node - Popping from branch stack instead");
            currentNode = branchNodeStack.Pop();
            currentNode = currentNode->right;
        }
    }

    if (!currentNode || (!currentNode->left && !currentNode->right) && branchNodeStack.Num() == 0) {
        // This is the end. But it's okay.
        LOG_INFO("Story end detected. brach stack is empty.");
        currentNode = nullptr;
        return;
    }
    LOG_INFO("Next Node: %s", *UEnum::GetDisplayValueAsText(currentNode->left->token.tokenType).ToString());

    if (!HandleBranchingNodeTypes()) {
        return;
    }

    if (currentNode->left->token.tokenType == ETokenType::BinOp) {
        const auto binOp = static_cast<UBinOpNode*>(currentNode->left);
        if (binOp) {
            // this would be an assignment at this point.
            // technically it could be any binop but it would make no sense.
            binOp->Evaluate(this);
        }
    }

    if (currentNode->left->token.tokenType == ETokenType::Command) {
        // command node:
        // left: command name
        // right: arg:      left:   binop
        //                  right:  next arg
        const UCommandNode* cmdNode = static_cast<UCommandNode*>(currentNode->left);
        if (commandRegister) {
            if (const auto cmd = commandRegister->GetCommand(cmdNode->GetCommandName())) {
                cmd->Execute(cmdNode->GetArgs(), this);
            }
        }
    }
}

void UStoryRunner::GoToNextDialogNode() {
    while (currentNode != nullptr || branchNodeStack.Num() > 0) {
        ShiftToNextNode();
        if (!currentNode) {
            break;
        }
        const auto currentType = currentNode->Statements()->token.tokenType;
        if (currentType == ETokenType::Speech || currentType == ETokenType::BeginBranching) {
            break;
        }
    }
}

ERunnerState UStoryRunner::GetCurrent(FDialogData& dialogData) {
    if (currentNode->Statements()->token.tokenType == ETokenType::Speech ) {
        const auto speech = static_cast<USpeakNode*>(currentNode->left);
        if (actorRegister) {
            dialogData.dialogueActor = actorRegister->GetActorForTag(speech->GetSpeaker());
        }
        dialogData.message = speech->GetText();
        return ERunnerState::Ok;
    }

    if (currentNode->Statements()->token.tokenType == ETokenType::BeginBranching) {
        const auto speech = static_cast<USpeakNode*>(currentNode->Statements()->left);
        const auto choice = static_cast<UChoiceNode*>(currentNode->Statements()->right);
        dialogData.choices = choice->GetChoices();
        if (actorRegister) {
            dialogData.dialogueActor = actorRegister->GetActorForTag(speech->GetSpeaker());
        }
        dialogData.message = speech->GetText();
        return ERunnerState::Choices;
    }

    return ERunnerState::NeedNext;
}

ERunnerState UStoryRunner::Next() {
    GoToNextDialogNode();
    
    if (!currentNode) {
        return ERunnerState::Done;
    }
    return ERunnerState::Ok;
}

ERunnerState UStoryRunner::NextWithChoice(int choice) {
    // This here is a little special.
    // When the last next() returned choices, we didn't advance the current node.
    // so we're still on the choice node that has:
    // left: speech
    // right: first choice
    // and choice has:
    //      left: choice branch
    //      right: next choice
    // so in order to get the chosen branch we iterate x times over the choice node
    // and set currentNode to that and then call and return Next().
    // When this branch ends, the branchNodeStack pops and we can continue from there the root of the choice node where right is node after the branch.
    // This works on infinite amounts of nesting levels (in theory)
    auto choiceNode = static_cast<UChoiceNode*>(currentNode->Statements()->right);
    choiceNode = choiceNode->GetChoice(choice);
    currentNode = choiceNode->GetBranch();
    return ERunnerState::Ok;
}

void UStoryRunner::StartThreadFromAsset(UStoryAsset* asset, FString threadName) {
    if (!asset) {
        LOG_ERROR("No story asset given to run a thread from!");
        return;
    }
    
    UThreadNode* thread = asset->GetStoryThread(threadName);
    if (!thread) {
        LOG_ERROR("There is no thread named %s in the story asset %s", *threadName, *asset->GetName());
        return;
    }

    storyAsset = asset;
    threadNode = thread;
    currentNode = thread;
    HandleBranchingNodeTypes();
}

UDialogueActor* UStoryRunner::GetDialogueActor(const FString& nameOrTag) const {
    return actorRegister->GetActorForTag(nameOrTag);
}

