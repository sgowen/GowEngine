//
//  RTTI.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/4/16.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

RTTI::RTTI(const std::string& className) : _className(className), _parentRTTI(NULL)
{
    // Empty
}

RTTI::RTTI(const std::string& className, const RTTI& parentRTTI) : _className(className), _parentRTTI(&parentRTTI)
{
    // Empty
}

const std::string& RTTI::getClassName() const
{
    return _className;
}

bool RTTI::isExactly(const RTTI& rtti) const
{
    return (this == &rtti);
}

bool RTTI::isDerivedFrom(const RTTI& rtti) const
{
    const RTTI* pRTTI = this;
    
    while (pRTTI != NULL)
    {
        if (pRTTI == &rtti)
        {
            return true;
        }
        
        pRTTI = pRTTI->_parentRTTI;
    }
    
    return false;
}
