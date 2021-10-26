﻿// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "Commands/RuntimeDialogueData.h"
#include "Ui/MessagingWidget.h"

/**
 * Manages messages and commands that are sent to the dialogue
 */
class DIALOGED_API FMessageManager {
protected:
    
    bool hasAdvanced;
public:
    UMessagingWidget* messaging;
    FMessageManager();
    ~FMessageManager();

    void SetMessagingWidget(UMessagingWidget* widget) {
        messaging = widget;
    }
    
    /** Dispatches a dialogue piece to the dialogue renderer. */
    void Begin(FRuntimeDialogueData data);
    
    /** Attaches the given message to the already existing text on screen. */
    void Attach(FString message);

    // todo: BeginChoices(FDialogueChoiceData)
    // todo: SetSelectedChoice(int)

    /**
     * Forces the message rendering system to advance. When text is still being "typed",
     * this will make the rest of the text appear and move into "waiting" state.
     * Invoking this again, will end the message and IsDone must return true
     */
    void Advance();
    
    /** Returns true when the message rendering is done and the whole text is shown. */
    bool IsDone() const;

    // todo: int GetSelectedChoice(); // to be called after IsDone returns true, at which message manager must have set the selected choice.
};