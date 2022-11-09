//
// Created by ryula on 27.03.2022.
//

#ifndef IMAGE_PROCESSOR_CPP_BITMAPIMAGE_H
#define IMAGE_PROCESSOR_CPP_BITMAPIMAGE_H

#endif  // IMAGE_PROCESSOR_CPP_BITMAPIMAGE_H

#pragma once
#include <vector>
#include <map>
#include <fstream>
#include <functional>

#include "bitmapPixel.h"

struct BitmapReadingCheck
{
    bool Flag;
    std::string Description;
};

struct BitmapHeaderParameter
{
    unsigned int Value;
    unsigned int Size;
    unsigned int Position;
    int Status;
};


class BitmapImage
{
private:
    std::map<std::string, BitmapHeaderParameter> m_header_parameters_;

    std::map<std::string, BitmapReadingCheck> m_checks_;
    , индекс строки)
    std::vector<std::vector<BitmapColor>> m_image_;
    int m_endian_ = -1;
    int m_image_read_status_ = 0;
    unsigned int m_padding_;
    unsigned int ConvertBitsToUInt(const std::vector<unsigned char>& t_from, const unsigned int& t_start, const unsigned int& t_end);
    std::vector<unsigned char> ConvertUIntToBits(unsigned int t_what, const unsigned int& t_length);
    void ExtractParameterFromImage(const std::vector<unsigned char>& t_from, BitmapHeaderParameter& t_param);
    void CheckParameterFromImage(const std::vector<unsigned char>& t_from, BitmapHeaderParameter& t_param);
    void ExtractChannel(const std::vector<unsigned char>& t_from, const unsigned int& t_pos, const unsigned int& t_y_coord, const unsigned int& t_x_coord);
    void OutputHeaderParameter(std::ofstream& t_where, const BitmapHeaderParameter& t_what);
    void OutputChannel(std::ofstream& t_where, const unsigned int& t_y_coord, const unsigned int& t_x_coord);
    bool CompleteCheck();

public:
    BitmapImage();
    void ResetImage();
    std::map<std::string, BitmapHeaderParameter> GetHeaderParameters();
    std::map<std::string, BitmapReadingCheck> GetReadingChecks();
    int GetImageReadStatus();
    void SetPixelColor(BasicColor t_new_color, unsigned int t_y_coord, unsigned int t_x_coord);
    void MultiplyPixel(BasicColor t_multipliers, unsigned int t_y_coord, unsigned int t_x_coord);
    void ModifyPixel(BasicColor t_modifiers, unsigned int t_y_coord, unsigned int t_x_coord);
    std::pair<unsigned int, unsigned int> GetImageSize();
    BasicColor GetPixelColor(unsigned int t_y_coord, unsigned int t_x_coord);
    BitmapColor GetPixel(unsigned int t_y_coord, unsigned int t_x_coord);

    void Crop(unsigned int t_new_width, unsigned int t_new_height);

    void ReadBitmapImage(std::ifstream& t_from);
    void OutputBitmapImage(std::ofstream& t_where);
};

inline unsigned int BitmapImage::ConvertBitsToUInt(const std::vector<unsigned char>& t_from, const unsigned int& t_start, const unsigned int& t_end)
{
    unsigned int res = 0;

    if (m_endian_ > 0)
    {
        for (size_t k = t_start; k <= t_end; k++) {
            res |= static_cast<unsigned int>(t_from[k]) << ((t_end - k) * 8);
        }
    }
    else if (m_endian_ < 0)
    {
        for (int k = t_end; k >= static_cast<int>(t_start); k--) {
            res |= static_cast<unsigned int>(t_from[k]) << ((k - t_start) * 8);
        }
    }

    return res;
}

inline std::vector<unsigned char> BitmapImage::ConvertUIntToBits(unsigned int t_what, const unsigned int& t_length)
{
    std::vector<unsigned char> res(t_length);
    for (unsigned int k = 0; k < t_length; k++)
    {
        if (m_endian_ < 0) {
            res[k] = static_cast<unsigned char>(t_what);
        } else {
            res[t_length - k - 1] = static_cast<unsigned char>(t_what);
        }

        t_what >>= 8;
    }

    return res;
}


inline void BitmapImage::ExtractParameterFromImage(const std::vector<unsigned char>& t_from, BitmapHeaderParameter& t_param)
{
    unsigned int end_pos = t_param.Position + (t_param.Size - 1);

    t_param.Value = ConvertBitsToUInt(t_from, t_param.Position, end_pos);
    t_param.Status = true;
}

inline void BitmapImage::CheckParameterFromImage(const std::vector<unsigned char>& t_from, BitmapHeaderParameter& t_param)
{
    unsigned int end_pos = t_param.Position + (t_param.Size - 1);

    unsigned int value_from_file = ConvertBitsToUInt(t_from, t_param.Position, end_pos);

    t_param.Status = (value_from_file == t_param.Value ? 1 : -1);
}

inline void BitmapImage::ExtractChannel(const std::vector<unsigned char>& t_from, const unsigned int& t_pos, const unsigned int& t_y_coord, const unsigned int& t_x_coord)
{
    if (m_endian_ == -1) {
        m_image_[t_y_coord][t_x_coord].SetChannels( t_from[t_pos + 2], t_from[t_pos + 1], t_from[t_pos] );
    } else {
        m_image_[t_y_coord][t_x_coord].SetChannels( t_from[t_pos], t_from[t_pos + 1], t_from[t_pos + 2] );
    }
}

inline void BitmapImage::OutputHeaderParameter(std::ofstream& t_where, const BitmapHeaderParameter& t_what)
{
    std::vector<unsigned char> char_vect = ConvertUIntToBits(t_what.Value, t_what.Size);

    for (unsigned int k = 0; k < char_vect.size(); k++) {
        t_where << char_vect[k];
    }
}

void BitmapImage::OutputChannel(std::ofstream& t_where, const unsigned int& t_y_coord, const unsigned int& t_x_coord)
{
    BasicColor rgb = m_image_[t_y_coord][t_x_coord].GetChannels();

    if (m_endian_ == -1) {
        t_where << static_cast<unsigned char>(rgb[2] * 255) << static_cast<unsigned char>(rgb[1] * 255) << static_cast<unsigned char>(rgb[0] * 255);
    } else {
        t_where << static_cast<unsigned char>(rgb[0] * 255) << static_cast<unsigned char>(rgb[1] * 255) << static_cast<unsigned char>(rgb[2] * 255);
    }
}

inline bool BitmapImage::CompleteCheck()
{
    for (auto check : m_checks_) {
        if (!check.second.Flag)
        {
            m_image_read_status_ = -1;
            return false;
        }
    }
    for (auto parameter : m_header_parameters_) {
        if (parameter.second.Status != 1)
        {
            m_image_read_status_ = -1;
            return false;
        }
    }

    m_image_read_status_ = 1;
    return true;
}


BitmapImage::BitmapImage()
{
    ResetImage();
}

inline void BitmapImage::ResetImage()
{
    m_header_parameters_["FileType"] = { 19778, 2, 0, 0 };
    m_header_parameters_["FileSize"] = { 0, 4, m_header_parameters_["FileType"].Position + m_header_parameters_["FileType"].Size, 0 };

    m_header_parameters_["Reserved1"] = { 0, 2, m_header_parameters_["FileSize"].Position + m_header_parameters_["FileSize"].Size, 0 };
    m_header_parameters_["Reserved2"] = { 0, 2, m_header_parameters_["Reserved1"].Position + m_header_parameters_["Reserved1"].Size, 0 };

    m_header_parameters_["PixelDataOffset"] = { 54, 4, m_header_parameters_["Reserved2"].Position + m_header_parameters_["Reserved2"].Size, 0 };
    m_header_parameters_["HeaderSize"] = { 40, 4, m_header_parameters_["PixelDataOffset"].Position + m_header_parameters_["PixelDataOffset"].Size, 0 };

    m_header_parameters_["ImageWidth"] = { 0, 4, m_header_parameters_["HeaderSize"].Position + m_header_parameters_["HeaderSize"].Size, 0 };
    m_header_parameters_["ImageHeight"] = { 0, 4, m_header_parameters_["ImageWidth"].Position + m_header_parameters_["ImageWidth"].Size, 0 };

    m_header_parameters_["Planes"] = { 1, 2, m_header_parameters_["ImageHeight"].Position + m_header_parameters_["ImageHeight"].Size, 0 };
    m_header_parameters_["BitsPerPixel"] = { 24, 2, m_header_parameters_["Planes"].Position + m_header_parameters_["Planes"].Size, 0 };

    m_header_parameters_["Compression"] = { 0, 4, m_header_parameters_["BitsPerPixel"].Position + m_header_parameters_["BitsPerPixel"].Size, 0 };

    m_header_parameters_["ImageSize"] = { 0, 4, m_header_parameters_["Compression"].Position + m_header_parameters_["Compression"].Size, 0 };

    m_header_parameters_["XPixelsPerMeter"] = { 0, 4, m_header_parameters_["ImageSize"].Position + m_header_parameters_["ImageSize"].Size, 0 };
    m_header_parameters_["YPixelsPerMeter"] = { 0, 4, m_header_parameters_["XPixelsPerMeter"].Position + m_header_parameters_["XPixelsPerMeter"].Size, 0 };

    m_header_parameters_["TotalColors"] = { 0, 4, m_header_parameters_["YPixelsPerMeter"].Position + m_header_parameters_["YPixelsPerMeter"].Size, 0 };
    //Важных цветов, значения по умолчанию нет, четыре байта, следует за TotalColors, не прочтена
    m_header_parameters_["ImportantColors"] = { 0, 4, m_header_parameters_["TotalColors"].Position + m_header_parameters_["TotalColors"].Size, 0 };

    m_checks_["Check_InImageSize"] = { false, "Checking if fields ImageSize, ImageWidth, ImageHeight and BitsPerPixel corresponds" };
    m_checks_["Check_RawImageSize"] = { false, "Checking if field ImageSize corresponds to data" };

    m_image_.clear();

    m_image_read_status_ = 0;
}

inline std::map<std::string, BitmapHeaderParameter> BitmapImage::GetHeaderParameters()
{
    return m_header_parameters_;
}

inline std::map<std::string, BitmapReadingCheck> BitmapImage::GetReadingChecks()
{
    return m_checks_;
}

inline int BitmapImage::GetImageReadStatus()
{
    return m_image_read_status_;
}

inline std::pair<unsigned int, unsigned int> BitmapImage::GetImageSize()
{
    if (!m_image_.empty()) {
        return { m_image_.size(), m_image_[0].size() };
    } else {
        return { 0, 0 };
    }
}

inline BasicColor BitmapImage::GetPixelColor(unsigned int t_y_coord, unsigned int t_x_coord)
{
    return m_image_[t_y_coord][t_x_coord].GetChannels();
}

inline BitmapColor BitmapImage::GetPixel(unsigned int t_y_coord, unsigned int t_x_coord)
{
    return m_image_[t_y_coord][t_x_coord];
}

inline void BitmapImage::SetPixelColor(BasicColor t_new_color, unsigned int t_y_coord, unsigned int t_x_coord)
{
    m_image_[t_y_coord][t_x_coord].SetChannels(t_new_color);
}

inline void BitmapImage::MultiplyPixel(BasicColor t_multipliers, unsigned int t_y_coord, unsigned int t_x_coord)
{
    m_image_[t_y_coord][t_x_coord].MultiplyChannels(t_multipliers);
}

inline void BitmapImage::ModifyPixel(BasicColor t_modifiers, unsigned int t_y_coord, unsigned int t_x_coord)
{
    m_image_[t_y_coord][t_x_coord].ModifyChannels(t_modifiers);
}
inline void BitmapImage::Crop(unsigned int t_new_width, unsigned int t_new_height)
{
    m_image_.erase(m_image_.begin(), m_image_.end() - t_new_height);
    for (unsigned int k = 0; k < m_image_.size(); k++) {
        m_image_[k].erase(m_image_[k].begin() + t_new_width, m_image_[k].end());
    }
}
inline void BitmapImage::ReadBitmapImage(std::ifstream &t_from)
{
    std::vector<unsigned char> raw_image_data(std::istreambuf_iterator<char>(t_from), {});
    if (raw_image_data.size() < m_header_parameters_["PixelDataOffset"].Value)
    {
        m_image_read_status_ = -1;
        m_header_parameters_["PixelDataOffset"].Value = raw_image_data.size();
        m_header_parameters_["PixelDataOffset"].Status = -1;
        return;
    }
    CheckParameterFromImage(raw_image_data, m_header_parameters_["FileType"]);
    CheckParameterFromImage(raw_image_data, m_header_parameters_["Reserved1"]);
    CheckParameterFromImage(raw_image_data, m_header_parameters_["Reserved2"]);
    CheckParameterFromImage(raw_image_data, m_header_parameters_["PixelDataOffset"]);
    CheckParameterFromImage(raw_image_data, m_header_parameters_["HeaderSize"]);
    CheckParameterFromImage(raw_image_data, m_header_parameters_["Planes"]);
    CheckParameterFromImage(raw_image_data, m_header_parameters_["BitsPerPixel"]);
    CheckParameterFromImage(raw_image_data, m_header_parameters_["Compression"]);
    ExtractParameterFromImage(raw_image_data, m_header_parameters_["FileSize"]);
    ExtractParameterFromImage(raw_image_data, m_header_parameters_["ImageWidth"]);
    ExtractParameterFromImage(raw_image_data, m_header_parameters_["ImageHeight"]);
    ExtractParameterFromImage(raw_image_data, m_header_parameters_["ImageSize"]);
    ExtractParameterFromImage(raw_image_data, m_header_parameters_["XPixelsPerMeter"]);
    ExtractParameterFromImage(raw_image_data, m_header_parameters_["YPixelsPerMeter"]);
    ExtractParameterFromImage(raw_image_data, m_header_parameters_["TotalColors"]);
    ExtractParameterFromImage(raw_image_data, m_header_parameters_["ImportantColors"]);
    unsigned int current_pos = m_header_parameters_["PixelDataOffset"].Value;
    m_padding_ = (4 - (m_header_parameters_["ImageWidth"].Value * m_header_parameters_["BitsPerPixel"].Value / 8) % 4) % 4;
    m_checks_["Check_InImageSize"].Flag = (m_header_parameters_["ImageSize"].Value == m_header_parameters_["ImageHeight"].Value * m_header_parameters_["ImageWidth"].Value * m_header_parameters_["BitsPerPixel"].Value / 8 + m_padding_ * m_header_parameters_["ImageHeight"].Value);
    m_checks_["Check_RawImageSize"].Flag = (m_header_parameters_["ImageSize"].Value + m_header_parameters_["PixelDataOffset"].Value == raw_image_data.size()  );
    if (!CompleteCheck()) {
        return;
    }
    for (unsigned int k = 0; k < m_header_parameters_["ImageHeight"].Value; k++)
    {
        m_image_.push_back(std::vector<BitmapColor>(m_header_parameters_["ImageWidth"].Value));

        for (unsigned int j = 0; j < m_header_parameters_["ImageWidth"].Value; j++)
        {
            ExtractChannel(raw_image_data, current_pos, k, j);

            current_pos += 3;
        }

        current_pos += m_padding_;
    }
}
inline void BitmapImage::OutputBitmapImage(std::ofstream& t_where)
{
    m_header_parameters_["ImageWidth"].Value = m_image_[0].size();
    m_header_parameters_["ImageHeight"].Value = m_image_.size();
    m_header_parameters_["ImageSize"].Value = m_header_parameters_["ImageHeight"].Value * m_header_parameters_["ImageWidth"].Value * m_header_parameters_["BitsPerPixel"].Value / 8 + m_padding_ * m_header_parameters_["ImageHeight"].Value;
    m_header_parameters_["FileSize"].Value = m_header_parameters_["ImageSize"].Value + m_header_parameters_["PixelDataOffset"].Value;
    m_padding_ = (4 - (m_header_parameters_["ImageWidth"].Value * m_header_parameters_["BitsPerPixel"].Value / 8) % 4) % 4;
    OutputHeaderParameter(t_where, m_header_parameters_["FileType"]);
    OutputHeaderParameter(t_where, m_header_parameters_["FileSize"]);
    OutputHeaderParameter(t_where, m_header_parameters_["Reserved1"]);
    OutputHeaderParameter(t_where, m_header_parameters_["Reserved2"]);
    OutputHeaderParameter(t_where, m_header_parameters_["PixelDataOffset"]);
    OutputHeaderParameter(t_where, m_header_parameters_["HeaderSize"]);
    OutputHeaderParameter(t_where, m_header_parameters_["ImageWidth"]);
    OutputHeaderParameter(t_where, m_header_parameters_["ImageHeight"]);
    OutputHeaderParameter(t_where, m_header_parameters_["Planes"]);
    OutputHeaderParameter(t_where, m_header_parameters_["BitsPerPixel"]);
    OutputHeaderParameter(t_where, m_header_parameters_["Compression"]);
    OutputHeaderParameter(t_where, m_header_parameters_["ImageSize"]);
    OutputHeaderParameter(t_where, m_header_parameters_["XPixelsPerMeter"]);
    OutputHeaderParameter(t_where, m_header_parameters_["YPixelsPerMeter"]);
    OutputHeaderParameter(t_where, m_header_parameters_["TotalColors"]);
    OutputHeaderParameter(t_where, m_header_parameters_["ImportantColors"]);
    for (unsigned int k = 0; k < m_image_.size(); k++)
    {
        for (unsigned int j = 0; j < m_image_[k].size(); j++) {
            OutputChannel(t_where, k, j);
        }
        for (unsigned int j = 0; j < m_padding_; j++) {
            t_where << static_cast<unsigned char>('A');
        }
    }
}