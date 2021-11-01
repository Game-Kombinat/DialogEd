#include "DialogueCommand.h"

UClass* UDialogueCommand::TargetActorType() const { return targetActorClass; }

void UDialogueCommand::SetMessageManager(UMessageManager* msgManager) { messageManager = msgManager; }

void UDialogueCommand::SetWorld(UWorld* world) { this->activeWorld = world; }

void UDialogueCommand::SetPlayerController(APlayerController* playerController) { controller = playerController; }

void UDialogueCommand::SetStoryThread(UStoryThread* storyThread) { myThread = storyThread; }

void UDialogueCommand::SetBranches(const TArray<FDialogueBranchId> dialogueBranches) { branches = dialogueBranches; }
