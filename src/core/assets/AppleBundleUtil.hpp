//
//  AppleBundleUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_APPLE

#include <string>

std::string getBundleRootFilePath();

#endif /* IS_APPLE */
