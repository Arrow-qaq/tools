/*************************************************************
Copyright:
File name: serializa.hpp
Description:
Author: hurui
Date: 2023-03-09
History:
**************************************************************/

#pragma once

#include <QVariant>

namespace NSerializaTion
{
    class serializaHelper
    {
    public:

        template <typename T>
        static void serializa(T t, QVariant& var)
        {
            t.setValue(var);
        }

        template <typename T>
        static void deSerializa(T& t, const QVariant& var)
        {
            t.getValue(var);
        }

        template <typename T>
        static void deSerializa(QList<T>& list, const QVariant& varList)
        {
            auto&& listVar = varList.toList();

            for (auto&& var : listVar)
            {
                T t;
                serializa(t, var);
                list.append(t);
            }
        }
    };

    template <typename T>
    int getFromVar(T& t, const QVariant& var)
    {
        if (var.canConvert<T>())
        {
            t = var.value<T>();
        }
        return 0;
    }

    template <typename T>
    int setVar(T& t, const QString& key, QVariantMap& var)
    {
        if (!var.contains(key))
        {
            var.insert(key,QVariant(t));
        }
        return 0;
    }

    template <typename... Args>
    void varToObject(const QStringList& names, const QVariant& var, Args&... args)
    {
        auto mapVar = var.toMap();

        bool containKey = true;

        auto&& list = mapVar.keys();
        for (auto& name : names)
        {
            if (!list.contains(name))
                containKey = false;
        }

        if (containKey)
        {
            int index = 0;
            std::initializer_list<int>{(getFromVar(args, mapVar[names[index++]]), 0)...};
        }
    }

    template <typename... Args>
    void objectToVar(const QStringList& names, QVariant& var, const Args&... args)
    {
        QVariantMap mapVar;

        int index = 0;
        std::initializer_list<int>{(setVar(args, names[index++], mapVar), 0)...};
        //std::initializer_list<int>{(mapVar.insert(names[index++], QVariant(args)), 0)...};

        var = mapVar;
    }
}


#define REG_SERIALZA(...)\
private:\
friend class NSerializaTion::serializaHelper;            \
    QStringList _names = QString(#__VA_ARGS__).remove(QRegExp("\\s")).split(",");                          \
    void getValue(const QVariant& var) { NSerializaTion::varToObject(_names, var, __VA_ARGS__); }\
void setValue(QVariant& var) { NSerializaTion::objectToVar(_names, var, __VA_ARGS__); }
