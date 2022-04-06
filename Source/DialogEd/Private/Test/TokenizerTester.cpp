// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TokenizerTester.h"

#include "Logging.h"
#include "StoryRunner.h"
#include "Parser/DialogModel.h"
#include "Parser/Tokenizer.h"


// Sets default values
ATokenizerTester::ATokenizerTester() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    runner = CreateDefaultSubobject<UStoryRunner>("Story Runner");
}

void ATokenizerTester::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);
    if (runner->CanContinue()) {
        
        FDialogData d;
        auto retval = runner->GetCurrent(d);
        LOG_INFO("%s", *d.message);
        if (retval == ERunnerState::Choices) {
            runner->NextWithChoice(0);
            LOG_INFO("%s", *d.message);
        }
        else {
            runner->Next();
        }
    }
    else {
        LOG_INFO("Story done, disbaling actor tick");
        SetActorTickEnabled(false);
    }
    
}

// Called when the game starts or when spawned
void ATokenizerTester::BeginPlay() {
    Super::BeginPlay();
    runner->StartThreadFromAsset(storyAsset, storyName);
}
