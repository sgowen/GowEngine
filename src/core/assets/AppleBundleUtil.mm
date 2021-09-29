//
//  AppleBundleUtil.mm
//  GowEngine
//
//  Created by Stephen Gowen on 9/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AppleBundleUtil.hpp"

#if IS_APPLE

#if IS_IOS
    #import <Foundation/Foundation.h>
#elif IS_MACOS
    #import <Cocoa/Cocoa.h>
#endif

std::string getBundleRootFilePath(const char* configFilePath)
{
    assert(configFilePath != nullptr);
    
    NSBundle* bundleToUse = [NSBundle mainBundle];
    NSString* dataPath = [[NSString alloc] initWithCString:configFilePath encoding:NSASCIIStringEncoding];
    std::string dataBundlePath = [[bundleToUse pathForResource:dataPath ofType:nil] fileSystemRepresentation];
    std::size_t dataIndex = dataBundlePath.find(configFilePath);
    std::string ret = dataBundlePath.substr(0, dataIndex);
    return ret;
}

#endif /* IS_APPLE */
