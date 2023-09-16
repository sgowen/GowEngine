//
//  AppleBundleUtil.mm
//  GowEngine
//
//  Created by Stephen Gowen on 9/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include "AppleBundleUtil.hpp"

#if IS_APPLE

#if IS_IOS
    #import <Foundation/Foundation.h>
#elif IS_MACOS
    #import <Cocoa/Cocoa.h>
#endif

std::string getBundleRootFilePath()
{
    NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
    
    // Okay, back to C++ as fast as possible, love that for me
    std::string ret = [bundlePath fileSystemRepresentation];
#if IS_MACOS
    ret += "/Contents/Resources";
#endif
    ret += "/";
    
    return ret;
}

std::string getBundleFilePath(std::string filePath)
{
    return getBundleRootFilePath() + filePath;
}

#endif /* IS_APPLE */
