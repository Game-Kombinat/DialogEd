// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "DialogueData.h"

/**
 * Manages messages and commands that are sent to the dialogue
 */
class DIALOGED_API FMessageManager {
public:
    FMessageManager();
    ~FMessageManager();

    /** Dispatches a dialogue piece to the dialogue renderer. */
    void Begin(FDialogueData data);
    
    /** Attaches the given message to the already existing text on screen. */
    void Attach(FString message);

    // todo: BeginChoices(FDialogueChoiceData)

    /**
     * Forces the message rendering system to advance. When text is still being "typed",
     * this will make the rest of the text appear and move into "waiting" state.
     * Invoking this again, will end the message and IsDone must return true
     */
    void Advance();
    
    /** Returns true when the message rendering is done and the whole text is shown. */
    bool IsDone();

    // todo: int GetSelectedChoice(); // to be called after IsDone returns true, at which time manager must have set the selected choice.
};
