// Copyright (c) Game Kombinat


#include "FileParser.h"

#include "Logging.h"


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
