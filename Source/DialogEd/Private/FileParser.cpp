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



void FileParser::ParseInto(UStoryAsset* storyAsset) {
    // given that each line is one command, lets go
    // I guess this can be done better with a stack but I cba right now.
    bool hasThread = false;
    UStoryThread* currentThread = nullptr;
    for (FString line : fileLines) {
        // first check if line is a comment
        if (IsComment(line)) {
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
        // for now, this is all we support
        else {
            currentThread->AddCommand(ParseWithSpeakCommand(line));
        }
    }
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
        storyAsset->AddStoryThread(t);
        return t;
    }
    LOG_ERROR("Failed to parse story thread header %s", *line);
    return nullptr;
}
