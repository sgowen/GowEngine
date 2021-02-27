//
//  NSBundleHelper.mm
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NSBundleHelper.hpp"

#include "PlatformMacros.hpp"

#if IS_IOS
    #import <Foundation/Foundation.h>
#elif IS_MACOS
    #import <Cocoa/Cocoa.h>
#endif

const char* getBundlePath(const char* filePath)
{
    NSBundle* bundleToUse = [NSBundle mainBundle];
    NSString* path = [[NSString alloc] initWithCString:filePath encoding:NSASCIIStringEncoding];
    return [[bundleToUse pathForResource:path ofType:nil] fileSystemRepresentation];
}
