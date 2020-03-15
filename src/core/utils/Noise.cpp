#include "core/utils/Noise.h"

Noise::Noise()
{

}

Noise::~Noise()
{

}

void Noise::clean()
{

}

void Noise::setData(const NoiseData& createInfo)
{
    m_data = createInfo;
}

float Noise::getValue(const float& x, const float& z) const
{
    float totalValue = 0.0f;

    for (unsigned int i = 0; i < m_data.octaves - 1; i++)
    {
        float frequency = pow(2.0f, i) / 2.0f;
        float amplitude = pow(m_data.roughness, i);

        totalValue += noise(x * frequency / m_data.smoother, z / m_data.smoother * frequency) * amplitude;
    }

    int height = (int) ((((totalValue + 1) / 2.0f) * (m_data.amplitudeMultiplier * 1.2)) + m_data.heightOffset) / 1.005;

    return height;
}

float Noise::findNoise1(const int& n) const
{
    int a = n;

    a += m_data.seed;
    a = (a << 13) ^ a;

    float nn = (a * (a * a * 60493 + 19990303) + 1376312589) & 0x7fffffff;
    return 1.0 - ((float) nn / 1073741821.0);
}

float Noise::findNoise2(const float& x, const float& z) const
{
    float n = (int) x + (int) z * 57;
    return findNoise1(n);
}

float Noise::interpolate(const float& a, const float& b, const float &z) const
{
    float ft = z * 3.1415927;
    float f = (1.0f - cos(ft)) * 0.5f;

    return a * (1.0f - f) + b * f;
}

float Noise::noise(const float& x, const float& z) const
{
    float floorX = (int) x;
    float floorZ = (int) z;

    float   s = 0.0,
            t = 0.0,
            u = 0.0,
            v = 0.0;

    s = findNoise2(floorX,      floorZ);
    t = findNoise2(floorX + 1,  floorZ);
    v = findNoise2(floorX,      floorZ + 1);
    u = findNoise2(floorX + 1,  floorZ + 1);

    float rec1 = interpolate(s, t, x - floorX);
    float rec2 = interpolate(u, v, x - floorX);
    float rec3 = interpolate(rec1, rec2, z - floorZ);

    return rec3;
}

int Noise::createNoise(Noise* noise, const NoiseData& createInfo)
{
    noise->setData(createInfo);

    return 0;
}

Noise::NoiseData& Noise::getData()
{
    return m_data;
}
