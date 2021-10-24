// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"

struct FParsedCommand;
/**
 * Parses a text file into a set of commands.
 */
class DIALOGED_API FileParser {
    TArray<FString> fileLines;
    class UDataTable* targetDataTable;
public:
    FileParser(FString fileName);
    ~FileParser();

    
    TArray<struct FStoryThread> Parse();

private:
    static bool IsComment(const FString& line);
    static bool IsCommand(const FString& line);
    static bool IsBeginThread(const FString& line);
    static bool IsEndThread(const FString& line);
    static FParsedCommand ParseWithCommand(const FString& line);
    static FParsedCommand ParseWithSpeakCommand(const FString& line);
    static FStoryThread ParseThreadHeader(const FString& line);
};
