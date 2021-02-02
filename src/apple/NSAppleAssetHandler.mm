//
//  NSAppleAssetHandler.mm
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NSAppleAssetHandler.hpp"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "PlatformMacros.hpp"

#if IS_IOS
    #import <Foundation/Foundation.h>
#elif IS_MACOS
    #import <Cocoa/Cocoa.h>
#endif

FileData appleLoadAsset(const char *filePath)
{
    assert(filePath != NULL);
    
    const char *bundlePath = getBundlePath(filePath);

    FILE* stream = fopen(bundlePath, "r");
    assert(stream != NULL);

    fseek(stream, 0, SEEK_END);

    long size = ftell(stream);

    fseek(stream, 0, SEEK_SET);

    uint8_t* buffer = (uint8_t*) malloc(size);
    
    rewind(stream);
    fread(buffer, size, 1, stream);

    assert(ferror(stream) == 0);
    fclose(stream);

    return FileData(size, buffer, NULL);
}

void appleReleaseAsset(const FileData& fileData)
{
    assert(fileData._data != NULL);

    free((void *)fileData._data);
}

const char * getBundlePath(const char* filePath)
{
    NSBundle* bundleToUse = [NSBundle mainBundle];
    
    NSString *path = [[NSString alloc] initWithCString:filePath encoding:NSASCIIStringEncoding];
    
    return [[bundleToUse pathForResource:path ofType:nil] fileSystemRepresentation];
}
