// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"
#include "StoryAsset.h"

struct FParsedCommand;
/**
 * Parses a text file into a set of commands.
 */
class DIALOGED_API FileParser {
    TArray<FString> fileLines;
public:
    FileParser(FString fileName);
    ~FileParser();

    
    void ParseInto(UStoryAsset* storyAsset);

private:
    static bool IsComment(const FString& line);
    static bool IsCommand(const FString& line);
    static bool IsBeginThread(const FString& line);
    static bool IsEndThread(const FString& line);
    static FParsedCommand ParseWithCommand(const FString& line);
    static FParsedCommand ParseWithSpeakCommand(const FString& line);
    static UStoryThread* ParseThreadHeader(const FString& line, UStoryAsset* storyAsset);
};
