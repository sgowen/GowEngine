//
//  AppleMacros.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/28/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "BuildMacros.hpp"

#if IS_APPLE
    #if IS_IOS
        #import <Foundation/Foundation.h>
    #elif IS_MACOS
        #import <Cocoa/Cocoa.h>
    #endif

    #define APPLE_INIT_BUNDLE_ROOT_FILE_PATH                                                                            \
        NSBundle* bundleToUse = [NSBundle mainBundle];                                                                  \
        NSString* dataPath = [[NSString alloc] initWithCString:"data/json/config.json" encoding:NSASCIIStringEncoding]; \
        std::string dataBundlePath = [[bundleToUse pathForResource:dataPath ofType:nil] fileSystemRepresentation];      \
        std::size_t dataIndex = dataBundlePath.find("data/json/config.json");                                           \
        std::string bundleRootFilePath = dataBundlePath.substr(0, dataIndex);                                           \
        void* data1 = (void*)&bundleRootFilePath
    
    #define APPLE_BUNDLE_ROOT_FILE_PATH data1
#endif
