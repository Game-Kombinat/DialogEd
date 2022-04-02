// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TokenizerTester.h"

#include "Logging.h"
#include "Parser/DialogModel.h"
#include "Parser/Tokenizer.h"


// Sets default values
ATokenizerTester::ATokenizerTester() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATokenizerTester::BeginPlay() {
    Super::BeginPlay();

    FString testFile = FPaths::ProjectDir();
    testFile.Append("/AssetSrc/Data/Test.story");
    UE_LOG(LogTemp, Display, TEXT("%s"), *testFile);
    
    
    FTokenizer t;
    FString input;
    if (!FFileHelper::LoadFileToString(input, *testFile)) {
        LOG_ERROR("Failed reading test story");
        return;
    }
    auto tokens = t.Tokenize(input);
    TArray<FString> tokenStrings;
    for (auto token : tokens) {
        FString format = "{Type: {0}, Start: {1}, End: {2}, Value: {3}, Precedence: {4}}";
        FString result = FString::Format(*format, {UEnum::GetDisplayValueAsText(token.tokenType).ToString(), token.startIndex, token.endIndex, token.value, token.precedence});
        tokenStrings.Add(result);
    }

    FString outputFile = FPaths::ProjectDir();
    
    outputFile.Append("/AssetSrc/Data/Test.output");
    FFileHelper::SaveStringArrayToFile(tokenStrings, *outputFile);

    FDialogModel* dm = new FDialogModel(tokens);
    dm->Make();
    
    const FString tree = dm->ToString();
    outputFile = FPaths::ProjectDir();
    
    outputFile.Append("/AssetSrc/Data/Test_Tree.output");
    FFileHelper::SaveStringToFile(tree, *outputFile);


    delete dm;
}
