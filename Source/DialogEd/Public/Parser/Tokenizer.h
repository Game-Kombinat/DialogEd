#pragma once
#include "ParsedToken.h"

// need this because TMap doesn't do <int, TArray<>>
struct FTokenGroup {
    TArray<FParsedToken> tokens;
};

class DIALOGED_API FTokenizer {
protected:
    TArray<FParsedToken> parsedTokens;
    TArray<FToken> tokenDefinitions;
public:
    FTokenizer();
    TArray<FParsedToken> Tokenize(const FString input);

};
