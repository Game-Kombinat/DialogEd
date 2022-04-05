// Copyright (c) Game Kombinat


#include "StoryRunner.h"

#include "ActorRegister.h"
#include "DialogueCommandRegister.h"
#include "Logging.h"
#include "PreparedCommand.h"
#include "StoryAsset.h"
#include "StoryThread.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Parser/Tree/BinOpNode.h"
#include "Parser/Tree/ChoiceNode.h"
#include "Parser/Tree/CommandNode.h"
#include "Parser/Tree/IdentifierNode.h"
#include "Parser/Tree/SpeakNode.h"
#include "Parser/Tree/ThreadNode.h"
#include "Ui/MessagingWidget.h"


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
    actorRegister->OnBeginPlay(GetWorld());
    commandRegister->OnBeginPlay();
    SetComponentTickEnabled(false);
}

void UStoryRunner::EndPlay(const EEndPlayReason::Type endPlayReason) {
    // todo: this is probably not needed anymore
    // if we stop play mid execution, we need to clean this up
    // if (currentThread.Num() > 0) {
    //     for (auto t : currentThread) {
    //         if (t) {
    //             t->CleanupCommand();
    //         }
    //     }
    // }
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

void UStoryRunner::CountRan(const UStoryThread* thread) const {
    const FString ranKey = FString::Format(TEXT("{0}__ran"), {thread->GetStoryThreadName()});
    int current = dataContext->GetValue(ranKey);
    if (current < 0) {
        current = 0;
    }
    
    dataContext->ForceSetValue(ranKey, current + 1);
}

UGameDataContext* UStoryRunner::GetDataContext() {
    return dataContext;
}

bool UStoryRunner::CanContinue() {
    // returns true if the next node is not a choice node.
    if (!currentNode && !currentNode->left) {
        return false;
    }
    return currentNode->left->token.tokenType == ETokenType::Speech;
}

void UStoryRunner::ShiftToNextNode() {
    // right is always a node that has
    // left: its logic
    // right: next node
    // this creates a chain of commands where going down right goes forward.

    // first, enqueue next token.
    if (!currentNode->right) {
        currentNode = branchNodeStack.Pop();
        currentNode = currentNode->right;
        
    }
    else {
        currentNode = currentNode->right;
    }

    // Forward through the tree until we hit a node that has something to run.
    // Runnable stuff is always left on this top level.
    while (!currentNode->left) {
        currentNode = currentNode->right;
    }

    if (!currentNode) {
        // This is the end. But it's okay.
        return;
    }

    // Check if it's an if token
    if (currentNode->left->token.tokenType == ETokenType::If) {
        // left is expression
        // right is node with: left: true branch, right: false branch
        const auto ifNode = static_cast<UBinOpNode*>(currentNode->left->left);
        const auto ifBranches = currentNode->left->right;
        if (!ifNode || !ifBranches) {
            LOG_ERROR("Expected binop node for if branch. didn't get any. this should not go through the parser!");
            return;
        }
        branchNodeStack.Push(currentNode);
        if (ifNode->Evaluate(this) > 0) {
            currentNode = ifBranches->left;
        }
        else {
            currentNode = ifBranches->right;
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
        branchNodeStack.Push(currentNode);
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
        if (const auto cmd = commandRegister->GetCommand(cmdNode->GetCommandName())) {
            cmd->Execute(cmdNode->GetArgs(), this);
        }
    }
}

void UStoryRunner::GoToNextDialogNode() {
    while (currentNode != nullptr && branchNodeStack.Num() > 0) {
        ShiftToNextNode();
        const auto currentType = currentNode->Statements()->token.tokenType;
        if (currentType == ETokenType::Speech || currentType == ETokenType::BeginBranching) {
            break;
        }
    }
}

ERunnerState UStoryRunner::Next(FDialogData& dialogData, bool skipAdvance) {
    // trouble here is: we always have a root node that has the statement on the left
    // and the next execution on the right.
    // Code here doesn't reflect that. Fix it!
    if (!skipAdvance || !currentNode) {
        GoToNextDialogNode();
    }

    if (!currentNode) {
        return ERunnerState::Done;
    }
    
    if (currentNode->Statements()->token.tokenType == ETokenType::Speech ) {
        const auto speech = static_cast<USpeakNode*>(currentNode->left);
        dialogData.dialogueActor = actorRegister->GetActorForTag(speech->GetSpeaker());
        dialogData.message = speech->GetText();
        return ERunnerState::Ok;
    }
    
    if (currentNode->Statements()->token.tokenType == ETokenType::BeginBranching) {
        const auto speech = static_cast<USpeakNode*>(currentNode->Statements()->left);
        const auto choice = static_cast<UChoiceNode*>(currentNode->Statements()->right);
        dialogData.choices = choice->GetChoices();
        dialogData.dialogueActor = actorRegister->GetActorForTag(speech->GetSpeaker());
        dialogData.message = speech->GetText();
        return ERunnerState::Choices;
    }
    return ERunnerState::Done;
}

ERunnerState UStoryRunner::NextWithChoice(int choice, FDialogData& dialogData) {
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
    // When this branch ends, the branchNodeStack pops and we can continue from there.
    // This works on infinite amounts of nesting levels (in theory)
    auto choiceNode = static_cast<UChoiceNode*>(currentNode->Statements()->right);
    choiceNode = choiceNode->GetChoice(choice);
    currentNode = choiceNode->GetBranch();

    // We find ourselves the next dialog node manually ...
    if (currentNode->left->token.tokenType != ETokenType::Speech || currentNode->left->token.tokenType != ETokenType::BeginBranching) {
        GoToNextDialogNode();
    }
    // ... and skip advancing in Next() because we're already there.
    return Next(dialogData, true);
}

void UStoryRunner::StartThreadFromAsset(UStoryAsset* asset, FString threadName) {
    if (!asset) {
        LOG_ERROR("No story asset given to run a thread from!");
        return;
    }
    
    UThreadNode* thread = asset->GetStoryThread(threadName);
    if (!thread) {
        LOG_ERROR("There is no thread named %S in the story asset %s", *threadName, *asset->GetName());
        return;
    }

    storyAsset = asset;
    threadNode = thread;
    currentNode = thread->GetFirstNode();
}

UDialogueActor* UStoryRunner::GetDialogueActor(const FString& nameOrTag) const {
    return actorRegister->GetActorForTag(nameOrTag);
}

