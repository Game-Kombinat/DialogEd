#include "DialogueCommand.h"


void UDialogueCommand::SetStoryRunner(UStoryRunner* runner) { storyRunner = runner; }

void UDialogueCommand::SetWorld(UWorld* world) { this->activeWorld = world; }

void UDialogueCommand::SetPlayerController(APlayerController* playerController) { controller = playerController; }

void UDialogueCommand::SetStoryThread(UStoryThread* storyThread) { myThread = storyThread; }
