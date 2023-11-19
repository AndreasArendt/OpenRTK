#pragma once

#include "../SvHealth.hpp"

class GpsSvHealth : public SvHealth
{
public: 
    static GpsSvHealth fromBitfield(double svHealth)
    {
        GpsSvHealth result;
        result._Health = (int)svHealth;
        return result;
    }
};