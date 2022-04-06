#include "Parser/Tokenizer.h"

#include "Parser/ParsedToken.h"

FTokenizer::FTokenizer() {
    tokenDefinitions.Add(FToken(ETokenType::OpenStory, ">>>>\\s*.+", 3));
    tokenDefinitions.Add(FToken(ETokenType::CloseStory, "<<<<", 3));
    tokenDefinitions.Add(FToken(ETokenType::Comment, "//.*", 1));
    tokenDefinitions.Add(FToken(ETokenType::CommentMultiLine, "\\/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/", 1));/* */
    tokenDefinitions.Add(FToken(ETokenType::If, "if", 2));
    tokenDefinitions.Add(FToken(ETokenType::Else, "else", 2));
    tokenDefinitions.Add(FToken(ETokenType::Endif, "endif", 2));
    tokenDefinitions.Add(FToken(ETokenType::BeginBranching, "choice", 2));
    tokenDefinitions.Add(FToken(ETokenType::EndBranching, "endchoice", 2));
    tokenDefinitions.Add(FToken(ETokenType::Branch, "=>", 1)); // should be a text token after --
    tokenDefinitions.Add(FToken(ETokenType::Command, "->", 1));
    tokenDefinitions.Add(FToken(ETokenType::Colon, ":", 1));
    tokenDefinitions.Add(FToken(ETokenType::And, "&&"));
    tokenDefinitions.Add(FToken(ETokenType::Or, "\\|\\|"));
    tokenDefinitions.Add(FToken(ETokenType::Greater, ">"));
    tokenDefinitions.Add(FToken(ETokenType::Less, "<"));
    tokenDefinitions.Add(FToken(ETokenType::Equal, "==", 2));
    tokenDefinitions.Add(FToken(ETokenType::GreaterOrEqual, ">=", 3));
    tokenDefinitions.Add(FToken(ETokenType::LessOrEqual, "<=", 3));
    tokenDefinitions.Add(FToken(ETokenType::NotEqual, "!=", 3));
    tokenDefinitions.Add(FToken(ETokenType::Plus, "\\+", 3));
    tokenDefinitions.Add(FToken(ETokenType::Minus, "\\-", 3));
    tokenDefinitions.Add(FToken(ETokenType::Slash, "\\/", 3));
    tokenDefinitions.Add(FToken(ETokenType::Asterisk, "\\*", 3));
    tokenDefinitions.Add(FToken(ETokenType::SingleEqual, "\\=", 3));
    tokenDefinitions.Add(FToken(ETokenType::PlusPlus, "\\+\\+", 2));
    tokenDefinitions.Add(FToken(ETokenType::MinusMinus, "--", 2));
    tokenDefinitions.Add(FToken(ETokenType::Identifier, "[a-zA-Z_$][a-zA-Z_$0-9]+", 3));
    tokenDefinitions.Add(FToken(ETokenType::NumberLiteral, "\\d+", 3));
    tokenDefinitions.Add(FToken(ETokenType::Text, "\"[^\"]*\"", 2));
    tokenDefinitions.Add(FToken(ETokenType::LParen, "\\(", 2));
    tokenDefinitions.Add(FToken(ETokenType::RParen, "\\)", 2));
}

TArray<FParsedToken> FTokenizer::Tokenize(const FString input) {
    parsedTokens.Reset();
    TArray<FParsedToken> allTokens;
    // blindly generate every possible token combination
    for (auto t : tokenDefinitions) {
        allTokens.Append(t.FindMatches(input));
    }

    // Sort tokens into groups by their starting index (same starting index, deals with same text piece)
    TMap<int, FTokenGroup> groupedTokens;
    for (auto t : allTokens) {
        if (!groupedTokens.Contains(t.startIndex)) {
            groupedTokens.Add(t.startIndex, FTokenGroup());
        }
        groupedTokens[t.startIndex].tokens.Add(t);
    }
    // clear out memory, we don't need this anymore.
    allTokens.Empty();
    
    // sort by starting index
    groupedTokens.KeySort([](const int& a, const int& b) {
        return a < b;
    });
    // run through groups, find matches with highest precedence (being the lowest value)
    FParsedToken lastToken;
    for (auto kvp : groupedTokens) {
        kvp.Value.tokens.Sort([](const FParsedToken& a, const FParsedToken& b) {
            return a.precedence < b.precedence;
        });
        auto bestMatch = kvp.Value.tokens[0];
        
        // prevent overlap from matching an overlap with the last token.
        if (lastToken.tokenType != ETokenType::Invalid && bestMatch.startIndex < lastToken.endIndex) {
            continue;
        }
        lastToken = bestMatch;
        parsedTokens.Add(bestMatch);
    }
    return parsedTokens;
}
