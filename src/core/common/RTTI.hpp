//
//  RTTI.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/4/16.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define DECL_RTTI_NOPARENT                      \
public:                                         \
    static const RTTI rtti;                     \
    virtual const RTTI& getRTTI()

#define DECL_RTTI                               \
public:                                         \
    static const RTTI rtti;                     \
    virtual const RTTI& getRTTI() override

#define IMPL_RTTI_NOPARENT(name)                \
    const RTTI name::rtti(#name);               \
    const RTTI& name::getRTTI() { return rtti; }

#define IMPL_RTTI(name,parent)                  \
    const RTTI name::rtti(#name, parent::rtti); \
    const RTTI& name::getRTTI() { return rtti; }

#include <string>

class RTTI
{
public:
    RTTI(const std::string& className);
    RTTI(const std::string& className, const RTTI& parentRTTI);

    const std::string& getClassName() const;
    bool isExactly(const RTTI& rtti) const;
    bool isDerivedFrom(const RTTI& rtti) const;

private:
    RTTI(const RTTI& obj);
    RTTI& operator=(const RTTI& obj);

    const std::string _className;
    const RTTI *_parentRTTI;
};
