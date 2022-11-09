// Created by ryula on 27.03.2022.
//

#ifndef IMAGE_PROCESSOR_CPP_BITMAPPIXEL_H
#define IMAGE_PROCESSOR_CPP_BITMAPPIXEL_H

#endif  // IMAGE_PROCESSOR_CPP_BITMAPPIXEL_H

#pragma once
#include <array>

typedef std::array<float, 3> BasicColor;

BasicColor operator+(const BasicColor& t_left, const BasicColor& t_right)
{
    BasicColor res;

    for (unsigned int k = 0; k < 3; k++)
    {
        if (t_left[k] + t_right[k] > 1.f) {
            res[k] = 1.f;
        } else if (t_left[k] + t_right[k] < 0.f) {
            res[k] = 0.f;
        } else {
            res[k] = t_left[k] + t_right[k];
        }
    }

    return res;
}

BasicColor operator*(const BasicColor& t_left, const BasicColor& t_right)
{
    BasicColor res;

    for (unsigned int k = 0; k < 3; k++)
    {

        if (t_left[k] * t_right[k] > 1.f) {
            res[k] = 1.f;

        } else if (t_left[k] * t_right[k] < 0.f) {
            res[k] = 0.f;
        } else {
            res[k] = t_left[k] * t_right[k];
        }
    }

    return res;
}


class BitmapColor
{
private:
    BasicColor m_rgb_;
public:
    BitmapColor();

    void SetChannels(const unsigned char t_new_red, const unsigned char t_new_green, const unsigned char t_new_blue);

    void SetChannels(const BasicColor& t_new_channels);

    void MultiplyChannels(const BasicColor& t_multipliers);

    void ModifyChannels(const BasicColor& t_modifiers);

    BasicColor GetChannels();

    float GetIntensity();
};

inline BitmapColor::BitmapColor()
{
    m_rgb_ = { 0, 0, 0 };
}


void BitmapColor::SetChannels(const unsigned char t_new_red, const unsigned char t_new_green, const unsigned char t_new_blue)
{
    m_rgb_ = { static_cast<float>(t_new_red) / 255.f, static_cast<float>(t_new_green) / 255.f, static_cast<float>(t_new_blue) / 255.f };
}

void BitmapColor::SetChannels(const BasicColor& t_new_channels)
{
    for (unsigned int k = 0; k < 3; k++)
    {
        if (t_new_channels[k] > 1.f) {
            m_rgb_[k] = 1.f;
        } else if (t_new_channels[k] < 0.f) {
            m_rgb_[k] = 0.f;
        } else {
            m_rgb_[k] = t_new_channels[k];
        }
    }
}


inline void BitmapColor::MultiplyChannels(const BasicColor& t_multipliers)
{
    m_rgb_ = m_rgb_ * t_multipliers;
}

inline void BitmapColor::ModifyChannels(const BasicColor& t_modifiers)
{
    m_rgb_ = m_rgb_ + t_modifiers;
}

inline BasicColor BitmapColor::GetChannels()
{
    return m_rgb_;
}

inline float BitmapColor::GetIntensity()
{
    return m_rgb_[0] / 3.f + m_rgb_[1] / 3.f + m_rgb_[2] / 3.f;
}