#pragma once

#include <cstdint>
#include <cmath>

#include "core/utils/logs/Logger.h"

class Noise
{
    public:

        /* static variables */


        /* structure */

        struct NoiseData
        {
            int       octaves             = 8;
            int       amplitudeMultiplier = 240;
            float     roughness           = 0.48f;
            float     smoother            = 190.0f;
            int       heightOffset        = 0;

            uint32_t  seed;
        };

        /* functions */

        Noise();
        ~Noise();

        void                      clean();
        void                      setData(const NoiseData& createInfo);

        float                     getValue(const float& x, const float& z) const;

        static int                createNoise(Noise* noise, const NoiseData& createInfo);

        NoiseData&                getData();

        /* variables */

    private:

        /* structure */

        /* functions */

        float                     findNoise1(const int& n) const;
        float                     findNoise2(const float& x, const float& z) const;
        float                     interpolate(const float& a, const float& b, const float& z) const;
        float                     noise(const float& x, const float& z) const;

        /* variables */

        NoiseData                m_data;
};
