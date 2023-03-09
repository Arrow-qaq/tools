/*************************************************************
Copyright:
File name: singleton.hpp
Description:
Author: hurui
Date: 2023-03-09
History:
**************************************************************/
#pragma once

template <typename T>
class singleton
{
public:
    template <typename... Args>
    static T* getInstance(Args ... agrs)
    {
        static T sInstance{agrs...};
        return &sInstance;
    }

private:
    singleton() = default;
    virtual ~singleton() = default;
    singleton(const singleton&) = default;
    singleton& operator=(const singleton&) = default;
};
