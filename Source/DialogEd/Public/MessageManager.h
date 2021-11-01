// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "Logging.h"
#include "Ui/SpeechbubbleWidget.h"
#include "Commands/RuntimeDialogueData.h"
#include "MessageManager.generated.h"

/**
 * Manages messages and commands that are sent to the dialogue
 */
UCLASS(Blueprintable)
class DIALOGED_API UMessageManager : public UObject {
    GENERATED_BODY()
protected:
    FName advanceActionName;
    
public:
    UPROPERTY()
    class UMessagingWidget* messaging;
    UMessageManager();
    virtual ~UMessageManager() override;

    void SetMessagingWidget(UMessagingWidget* widget);

    void SetActionName(FName name) {
        advanceActionName = name;
    }

    FName GetActionName() const {
        return advanceActionName;
    }
    
    /** Dispatches a dialogue piece to the dialogue renderer. */
    void Begin(FRuntimeDialogueData data) const;
    

    void Begin(FRuntimeDialogueData data, FChoiceCallback receiveChoice) const;
    
    /** Attaches the given message to the already existing text on screen. */
    void Attach(FString message);
    
    /**
     * Forces the message rendering system to advance. When text is still being "typed",
     * this will make the rest of the text appear and move into "waiting" state.
     * Invoking this again, will end the message and IsDone must return true
     */
    void Advance() const;
    
    /** Returns true when the message rendering is done and the whole text is shown. */
    bool IsDone() const;
    void Close() const;
};
