/*!
 * \file BtArgParser.cpp
 * \date 2015/12/22 16:33
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtArgParser
*/

#include "stdafx.h"
#include "BtArgParser.h"

#include "Core/BtCoreUtil.h"

#include "mcr/core/McCorePublic.h"

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

BtArgParser::BtArgParser(const std::string& args)
{
    split(args, ' ', m_args);
}

bool BtArgParser::validate(const std::string& spec)
{
    if (m_args.size() != spec.length())
    {
        BT_INFO("BtArgParser::validate() failed. (number mismatched: %d expected, %d got)", spec.length(), m_args.size());
        return false;
    }

    for (size_t i = 0; i < m_args.size(); ++i)
    {
        if (spec[i] == 'i')
        {
            int val;
            if (!McStringToInt(m_args[i].c_str(), val))
            {
                BT_INFO("BtArgParser::validate() failed. (type mismatched: 'int' expected, actual arg: %s, pos: %d)", m_args[i], i);
                return false;
            }
        }
        else if (spec[i] == 'f')
        {
            float val;
            if (!McStringToFloat(m_args[i].c_str(), val))
            {
                BT_INFO("BtArgParser::validate() failed. (type mismatched: 'float' expected, actual arg: %s, pos: %d)", m_args[i], i);
                return false;
            }
        }
    }

    return true;
}

BtArgParser& BtArgParser::parse(int* val)
{
    if (!MC_ARRAY_OVERRUN_CHECK(m_cursor, (int)m_args.size()))
    {
        BT_INFO("BtArgParser::parse() failed. (overrun occurred)");
        return *this;
    }

    std::string& arg = m_args[m_cursor];
    if (!McStringToInt(arg.c_str(), *val))
    {
        BT_INFO("BtArgParser::parse() failed. (type mismatched: 'int' expected, actual arg: %s)", arg);
        return *this;
    }

    ++m_cursor;
    return *this;
}

BtArgParser& BtArgParser::parse(float* val)
{
    if (!MC_ARRAY_OVERRUN_CHECK(m_cursor, (int)m_args.size()))
    {
        BT_INFO("BtArgParser::parse() failed. (overrun occurred)");
        return *this;
    }

    std::string& arg = m_args[m_cursor];
    if (!McStringToFloat(arg.c_str(), *val))
    {
        BT_INFO("BtArgParser::parse() failed. (type mismatched: 'float' expected, actual arg: %s)", arg);
        return *this;
    }

    ++m_cursor;
    return *this;
}

BtArgParser& BtArgParser::parse(std::string* val)
{
    if (!MC_ARRAY_OVERRUN_CHECK(m_cursor, (int)m_args.size()))
    {
        BT_INFO("BtArgParser::consume() failed. (overrun occurred)");
        return *this;
    }

    *val = m_args[m_cursor];

    ++m_cursor;
    return *this;
}

