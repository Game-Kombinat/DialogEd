#pragma once

UENUM()
enum class ETokenType : uint8 {
    Invalid,
    // controls
    OpenStory, // >>>>
    CloseStory, // <<<<
    Comment, // // match everything from here to end of line as comment, and discard
    CommentMultiLine, // // match everything from here to end of line as comment, and discard

    // statements
    If, // if
    Else, // else
    Endif, // endif
    BeginBranching, // **** followed by a speak token
    EndBranching, // **** followed by a speak token
    Branch, // -- followed by a text token
    Command, // -> followed by an identifier that is a function name
    Colon, // Speaker Name: followed by a body of text

    // Operators
    And, // &&
    Or, // ||
    Greater, // >
    Less, // <,
    Equal, // ==
    GreaterOrEqual, // >=
    LessOrEqual, // <=
    NotEqual, // !=
    Plus,  // +
    Minus, // -
    Slash, // /
    Asterisk, // *
    SingleEqual, // =
    PlusPlus, // ++
    MinusMinus, // --

    // Expressions (when found as part of a speak statement, these should be interpreted as text)
    Identifier, // [a-zA-Z_]+
    NumberLiteral, // only [0-9] - the underlying data context works in integers only so we don't need other literals or values.
    Whitespace, // we match for whitespace because it has no meaning in its own and can only be part of some other token or get discarded.
    
    Text, // anything between two "
    LParen,
    RParen
    
};

struct FParsedToken {
    ETokenType tokenType;
    FString value;
    int startIndex;
    int endIndex;
    int precedence;

    FParsedToken() {
        tokenType = ETokenType::Invalid;
        value.Empty();
        startIndex = 0;
        endIndex = 0;
        precedence = 100;
        
    }

    FParsedToken(const ETokenType type, const FString val, const int start, const int end, const int prec) {
        tokenType = type;
        value = val;
        startIndex = start;
        endIndex = end;
        precedence = prec;
    }
    
};

struct FToken {
    ETokenType tokenType;
    FString matchPattern;
    int precedence;

    FToken() {
        tokenType = ETokenType::OpenStory;
        matchPattern.Empty();
        precedence = 10;
    }
    FToken(const ETokenType type, const FString pattern) {
        tokenType = type;
        matchPattern = pattern;
        precedence = 10;
    }

    FToken(const ETokenType type, const FString pattern, const int prec) {
        tokenType = type;
        matchPattern = pattern;
        precedence = prec;
    }

    TArray<FParsedToken> FindMatches(FString input) {
        FRegexMatcher commandMatcher(FRegexPattern(matchPattern), *input);
        TArray<FParsedToken> parsedTokens;
        while (commandMatcher.FindNext()) {
            const int matchStart = commandMatcher.GetMatchBeginning();
            const int matchEnd = commandMatcher.GetMatchEnding();
            const FString value = input.Mid(matchStart, matchEnd - matchStart);
            
            parsedTokens.Add(FParsedToken(tokenType, value, matchStart, matchEnd, precedence));
        }

        return parsedTokens;
    }
};


