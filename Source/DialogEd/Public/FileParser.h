// Copyright (c) Game Kombinat

#pragma once

#include "CoreMinimal.h"

/**
 * Parses a text file into a set of commands.
 */
class DIALOGED_API FileParser {
    TArray<FString> fileLines;
    class UDataTable* targetDataTable;
public:
    FileParser(FString fileName);
    ~FileParser();
};
