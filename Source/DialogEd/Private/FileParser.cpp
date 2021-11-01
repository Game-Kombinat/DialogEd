// Copyright (c) Game Kombinat


#include "FileParser.h"

#include "Logging.h"
#include "ParsedCommand.h"
#include "StoryThread.h"


FileParser::FileParser(FString fileName) {
    // this could potentially become a little ugly with super large files.
    // But then again. In this day and age loading 4-5 MB into memory is nothing anyway.
    if (FPaths::FileExists(fileName)) {
        FFileHelper::LoadFileToStringArray(fileLines, *fileName);
    }
    else {
        LOG_WARNING("The given file does not exist! %s", *fileName);
    }
}

FileParser::~FileParser() {
}

void FileParser::ParseInto(UStoryAsset* storyAsset, TArray<FString>& lines) {
    FParsedCommand::ResetParsingIndex();
    // given that each line is one command, lets go
    // I guess this can be done better with a stack but I cba right now.
    bool hasThread = false;
    UStoryThread* currentThread = nullptr;
    for (int i = 0; i < lines.Num(); ++i) {
        FString line = lines[i];
        // first check if line is a comment or empty
        // todo: implement multi line comments
        if (IsComment(line) || line.TrimStartAndEnd().IsEmpty()) {
            continue;
        }

        // Check for thread header and end
        if (IsBeginThread(line)) {
            if (hasThread) {
                LOG_ERROR("Parsing error: New open thread while thread is already open. Line: %s", *line);
                continue;
            }
            hasThread = true;
            currentThread = ParseThreadHeader(line, storyAsset);
            continue;
        }

        if (IsEndThread(line)) {
            if (!hasThread) {
                LOG_ERROR("Parsing error: Closing thread found but no thread is open. Line: %s", *line);
            }
            hasThread = false;
            continue;
        }
        if (!currentThread) {
            LOG_ERROR("Logic error: Attempting to read new command but we have no reference to a story thread. Line: %s", *line);
            continue;
        }
        // then check if this is an explicit command
        if (IsCommand(line)) {
            currentThread->AddCommand(ParseWithCommand(line));
        }
        else if (BeginsChoiceSegment(line)) {
            i = ParseChoiceCommand(storyAsset, currentThread, lines, i);
        }
        // for now, this is all we support
        else {
            currentThread->AddCommand(ParseWithSpeakCommand(line));
        }
    }
}

void FileParser::ParseInto(UStoryAsset* storyAsset) {
    ParseInto(storyAsset, fileLines);
}

bool FileParser::IsComment(const FString& line) {
    return line.TrimStart().StartsWith("//");
}

bool FileParser::IsCommand(const FString& line) {
    return line.TrimStart().StartsWith("->");
}

bool FileParser::IsBeginThread(const FString& line) {
    return line.TrimStart().StartsWith(">>>>");
}

bool FileParser::IsEndThread(const FString& line) {
    return line.TrimStart().StartsWith("<<<<");
}

bool FileParser::BeginsChoiceSegment(const FString& line) {
    return line.TrimStart().StartsWith("****");
}

bool FileParser::EndsChoiceSegment(const FString& line) {
    return line.TrimStart().Equals("****");
}

bool FileParser::BeginsBranch(const FString& line) {
    return line.TrimStart().StartsWith("--");
}

FParsedCommand FileParser::ParseWithCommand(const FString& line) {
    // -> commandName actorName arg1 arg2 arg3 ...
    const FRegexPattern commandPattern(TEXT("->\\s*(\\w+)\\s(\\w+)\\s(.*)"));
    FRegexMatcher commandMatcher(commandPattern, *line);
    if (commandMatcher.FindNext()) {
        const FString commandName = commandMatcher.GetCaptureGroup(1);
        const FString actorName = commandMatcher.GetCaptureGroup(2);
        const FString arguments = commandMatcher.GetCaptureGroup(3);
        return FParsedCommand(commandName, actorName, arguments);
    }
    LOG_ERROR("Failed to parse command %s", *line);
    return FParsedCommand();
    
}

FParsedCommand FileParser::ParseWithSpeakCommand(const FString& line) {
    // ellen: you have to match all of this!
    const FRegexPattern commandPattern(TEXT("(\\w+):\\s*(.+)"));
    FRegexMatcher commandMatcher(commandPattern, *line);
    if (commandMatcher.FindNext()) {
        const FString speakerName = commandMatcher.GetCaptureGroup(1);
        const FString text = commandMatcher.GetCaptureGroup(2);
        return FParsedCommand("speak", speakerName, text);
    }
    LOG_ERROR("Failed to parse command %s", *line);
    return FParsedCommand();
}

UStoryThread* FileParser::ParseThreadHeader(const FString& line, UStoryAsset* storyAsset) {
    // >>>> Thread Name
    const FRegexPattern commandPattern(TEXT(">>>>\\s*(.*)"));
    FRegexMatcher commandMatcher(commandPattern, *line);
    if (commandMatcher.FindNext()) {
        const FString threadName = commandMatcher.GetCaptureGroup(1);
        const auto t = NewObject<UStoryThread>(storyAsset, UStoryThread::StaticClass(), FName(threadName));
        t->SetThreadName(threadName);
        t->SetStoryAsset(storyAsset);
        storyAsset->AddStoryThread(t);
        return t;
    }
    LOG_ERROR("Failed to parse story thread header %s", *line);
    return nullptr;
}

UStoryThread* FileParser::ParseChoiceHeader(const FString& line, UStoryAsset* storyAsset, FParsedCommand& cmd) {
    // -- Option Title
    const FRegexPattern commandPattern(TEXT("--\\s*(.*)"));
    FRegexMatcher commandMatcher(commandPattern, *line);
    if (commandMatcher.FindNext()) {
        const FString threadName = commandMatcher.GetCaptureGroup(1);
        const auto t = NewObject<UStoryThread>(storyAsset, UStoryThread::StaticClass(), FName(threadName));
        t->SetThreadName(threadName);
        t->SetStoryAsset(storyAsset);
        int idx = cmd.AddBranch(threadName);
        storyAsset->AddSubThread(cmd.GetBranchId(idx), t);
        return t;
    }
    LOG_ERROR("Failed to parse choice header %s", *line);
    return nullptr;
}

int FileParser::ParseChoiceCommand(UStoryAsset* storyAsset, UStoryThread* outerThread, TArray<FString>& lines, int lineNum) {
    FString headerLine = lines[lineNum];
    // **** actor: question
    const FRegexPattern commandPattern(TEXT("\\*\\*\\*\\*\\s*(.*):\\s*(.+)"));
    FRegexMatcher commandMatcher(commandPattern, *headerLine);
    if (commandMatcher.FindNext()) {
        const FString speaker = commandMatcher.GetCaptureGroup(1);
        // parsed commands can now have branches.
        // threads have a map containing subthreads, keyed by an id (commandName_branchName).
        // This way a unique address is known and accessible for each possible branch.
        const FString message = commandMatcher.GetCaptureGroup(2);
        FParsedCommand cmd = FParsedCommand("choice", speaker, message);
        return ParseChoiceSubThreads(storyAsset, outerThread, lines, lineNum+1, cmd);
    }
    return lineNum;
}

int FileParser::ParseChoiceSubThreads(UStoryAsset* story, UStoryThread* outerThread, TArray<FString>& lines, int lineNum, FParsedCommand& branchingCommand) {
    UStoryThread* currentThread = nullptr;
    for (int i = lineNum; i < lines.Num(); ++i) {
        // this will have to break when a line starting with **** is found
        FString line = lines[i];
        
        if (EndsChoiceSegment(line)) {
            outerThread->AddCommand(branchingCommand);
            return i;
        }
        // note there is no explicit "end branch". We auto end the branch when a new one is created.
        if (BeginsBranch(line)) {
            currentThread = ParseChoiceHeader(line, story, branchingCommand);
            continue;
        }

        if (!currentThread) {
            LOG_ERROR("No branch thread is open. Got nothing to put the line %s", *line);
            continue;
        }

        // then check if this is an explicit command
        if (IsCommand(line)) {
            currentThread->AddCommand(ParseWithCommand(line));
        }

        // nested choices, ho boi! todo: test if nested choices _actually_ work lol
        else if (BeginsChoiceSegment(line)) {
            i = ParseChoiceCommand(story, currentThread, lines, i);
        }
        // for now, this is all we support
        else {
            currentThread->AddCommand(ParseWithSpeakCommand(line));
        }
    }
    LOG_ERROR("There is an unterminated choice segment starting on line %i", lineNum);
    // this whole thing is trash, this will cause the whole operation to end
    return lines.Num();
}



