#include "DialogueCommand.h"


void UDialogueCommand::SetWorld(UWorld* world) { this->activeWorld = world; }

void UDialogueCommand::SetPlayerController(APlayerController* playerController) { controller = playerController; }

void UDialogueCommand::SetStoryThread(UStoryThread* storyThread) { myThread = storyThread; }
