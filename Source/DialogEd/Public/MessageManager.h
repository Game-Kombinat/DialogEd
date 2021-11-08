// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "Commands/RuntimeDialogueData.h"
#include "MessageManager.generated.h"

/**
 * Manages messages and commands that are sent to the dialogue.
 * Also provides an interface to listen to input actions.
 */
UCLASS(Blueprintable)
class DIALOGED_API UMessageManager : public UObject {
    GENERATED_BODY()
protected:
    UPROPERTY()
    class UMessagingWidget* messaging;
    UPROPERTY()
    class UStoryThread* currentStoryThread;
public:
    
    UMessageManager();
    virtual ~UMessageManager() override;

    void SetMessagingWidget(UMessagingWidget* widget);

    /** Dispatches a dialogue piece to the dialogue renderer. */
    void Begin(FRuntimeDialogueData data) const;
    
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

    void SetStoryThread(UStoryThread* story);

    void SetRelevantActors(TArray<UDialogueActor*> actors) const;
    
    void RemoveFromViewport() const;
    void AddToViewport() const;
};
