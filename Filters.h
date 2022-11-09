//
// Created by ryula on 27.03.2022.
//

#ifndef IMAGE_PROCESSOR_CPP_BITMAPFILTERS_H
#define IMAGE_PROCESSOR_CPP_BITMAPFILTERS_H

#endif  // IMAGE_PROCESSOR_CPP_BITMAPFILTERS_H

#pragma once
#include <functional>
#include <string>
#include <valarray>

#include "bitmapImage.h"


namespace bitmap_filter {
    bool CheckPositiveArgs(const std::vector<float>& t_args, const unsigned int& t_arguments_num) {
        for (const auto& arg : t_args) {
            if (arg < 0) {
                return false;
            }
        }

        if (t_args.size() != t_arguments_num) {
            return false;
        }

        return true;
    }

    bool CheckGaussianBlur(const std::vector<float>& t_args, const unsigned int& t_arguments_num) {
        for (const auto& arg : t_args) {
            if (arg < 0.38) {
                return false;
            }
        }

        if (t_args.size() != t_arguments_num) {
            return false;
        }

        return true;
    }

    bool CheckEdgeDetection(const std::vector<float>& t_args, const unsigned int& t_arguments_num) {
        for (const auto& arg : t_args) {
            if (arg < 0 || arg > 1) {
                return false;
            }
        }

        if (t_args.size() != t_arguments_num) {
            return false;
        }

        return true;
    }

    bool CheckEmpty(const std::vector<float>& t_args, const unsigned int& t_arguments_num) {
        return t_args.size() == t_arguments_num;
    }


    std::array<BitmapColor, 9> GetSurroundingPixels(BitmapImage& t_image, unsigned int t_y_coord, unsigned int t_x_coord) {
        std::array<BitmapColor, 9> surrounding_pixels;

        std::array<std::pair<unsigned int, unsigned int>, 9> surrounding_indexes;

        static const std::pair<unsigned int, unsigned int> IMAGE_SIZE = t_image.GetImageSize();

        surrounding_indexes[0] = {t_y_coord - 1, t_x_coord - 1};
        surrounding_indexes[1] = {t_y_coord - 1, t_x_coord};
        surrounding_indexes[2] = {t_y_coord - 1, t_x_coord + 1};
        surrounding_indexes[3] = {t_y_coord, t_x_coord - 1};
        surrounding_indexes[4] = {t_y_coord, t_x_coord};
        surrounding_indexes[5] = {t_y_coord, t_x_coord + 1};
        surrounding_indexes[6] = {t_y_coord + 1, t_x_coord - 1};
        surrounding_indexes[7] = {t_y_coord + 1, t_x_coord};
        surrounding_indexes[8] = {t_y_coord + 1, t_x_coord + 1};

        if (t_y_coord == 0) {
            surrounding_indexes[0] = {0, surrounding_indexes[0].second};
            surrounding_indexes[1] = {0, surrounding_indexes[1].second};
            surrounding_indexes[2] = {0, surrounding_indexes[2].second};
        }
        else if (t_y_coord == IMAGE_SIZE.first - 1) {
            surrounding_indexes[6] = {IMAGE_SIZE.first - 1, surrounding_indexes[6].second};
            surrounding_indexes[7] = {IMAGE_SIZE.first - 1, surrounding_indexes[7].second};
            surrounding_indexes[8] = {IMAGE_SIZE.first - 1, surrounding_indexes[8].second};
        }
        if (t_x_coord == 0) {
            surrounding_indexes[0] = {surrounding_indexes[0].first, 0};
            surrounding_indexes[3] = {surrounding_indexes[3].first, 0};
            surrounding_indexes[6] = {surrounding_indexes[6].first, 0};
        }
        else if (t_x_coord == IMAGE_SIZE.second - 1) {
            surrounding_indexes[2] = {surrounding_indexes[2].first, IMAGE_SIZE.second - 1};
            surrounding_indexes[5] = {surrounding_indexes[5].first, IMAGE_SIZE.second - 1};
            surrounding_indexes[8] = {surrounding_indexes[8].first, IMAGE_SIZE.second - 1};
        }

        for (unsigned int k = 0; k < 9; k++) {
            surrounding_pixels[k] = t_image.GetPixel(surrounding_indexes[k].first, surrounding_indexes[k].second);
        }

        return surrounding_pixels;
    }

    BasicColor ApplyFilterToPixel(std::array<BitmapColor, 9> t_pixel_array, std::array<float, 9> t_filter_array) {
        BasicColor resulting_modifiers{0, 0, 0};

        for (unsigned int k = 0; k < 9; k++) {
            resulting_modifiers[0] += static_cast<float>(t_pixel_array[k].GetChannels()[0]) * t_filter_array[k];
            resulting_modifiers[1] += static_cast<float>(t_pixel_array[k].GetChannels()[1]) * t_filter_array[k];
            resulting_modifiers[2] += static_cast<float>(t_pixel_array[k].GetChannels()[2]) * t_filter_array[k];
        };

        return resulting_modifiers;
    }


    std::vector<std::vector<float>> CreateGaussianCore(std::vector<int> t_x1, std::vector<int> t_x2, unsigned int t_index1,
                                                       unsigned int t_index2, float t_s) {

        const float den = 1.f / (2.f * 3.1415 * t_s * t_s);

        const float pow_den = -1.f / (2.f * t_s * t_s);


        std::vector<std::vector<float>> res(t_x1.size(), std::vector<float>(t_x2.size(), 0));

        float sum = 0;


        for (unsigned int k = 0; k < t_x1.size(); k++) {
            for (unsigned int j = 0; j < t_x2.size(); j++) {
                res[k][j] = den * exp(pow_den * ((t_x1[t_index1] - t_x1[j]) * (t_x1[t_index1] - t_x1[j]) +
                                                 (t_x2[t_index2] - t_x2[k]) * (t_x2[t_index2] - t_x2[k])));
                sum += res[k][j];
            }
        }


        for (unsigned int k = 0; k < t_x1.size(); k++) {
            for (unsigned int j = 0; j < t_x2.size(); j++) {
                res[k][j] /= sum;
            }
        }

        return res;
    };


    void RemoveRed(BitmapImage& t_image, const std::vector<float>& t_args) {

        const std::pair<unsigned int, unsigned int> image_size = t_image.GetImageSize();


        for (unsigned int k = 0; k < image_size.first; k++) {
            for (unsigned int j = 0; j < image_size.second; j++) {

                t_image.MultiplyPixel({0, 1, 1}, k, j);
            }
        }
    }


    void RemoveGreen(BitmapImage& t_image, const std::vector<float>& t_args) {

        const std::pair<unsigned int, unsigned int> image_size = t_image.GetImageSize();


        for (unsigned int k = 0; k < image_size.first; k++) {
            for (unsigned int j = 0; j < image_size.second; j++) {
                t_image.MultiplyPixel({1, 0, 1}, k, j);
            }
        }
    }


    void RemoveBlue(BitmapImage& t_image, const std::vector<float>& t_args) {
        const std::pair<unsigned int, unsigned int> image_size = t_image.GetImageSize();


        for (unsigned int k = 0; k < image_size.first; k++) {
            for (unsigned int j = 0; j < image_size.second; j++) {

                t_image.MultiplyPixel({1, 1, 0}, k, j);
            }
        }
    }


    void Crop(BitmapImage& t_image, const std::vector<float>& t_args) {
        t_image.Crop(t_args[0], t_args[1]);
    }

    void GreyScale(BitmapImage& t_image, const std::vector<float>& t_args) {
        const std::pair<unsigned int, unsigned int> image_size = t_image.GetImageSize();


        for (unsigned int k = 0; k < image_size.first; k++) {
            for (unsigned int j = 0; j < image_size.second; j++) {

                float new_color = 0.299 * t_image.GetPixelColor(k, j)[0] + 0.587 * t_image.GetPixelColor(k, j)[1] +
                                  0.114 * t_image.GetPixelColor(k, j)[2];

                t_image.SetPixelColor({new_color, new_color, new_color}, k, j);
            }
        }
    }


    void Negate(BitmapImage& t_image, const std::vector<float>& t_args) {

        const std::pair<unsigned int, unsigned int> image_size = t_image.GetImageSize();


        for (unsigned int k = 0; k < image_size.first; k++) {
            for (unsigned int j = 0; j < image_size.second; j++) {

                BasicColor current_color = t_image.GetPixelColor(k, j);

                t_image.SetPixelColor({1.f - current_color[0], 1.f - current_color[1], 1.f - current_color[2]}, k, j);
            }
        }
    }

    void Sharpening(BitmapImage& t_image, const std::vector<float>& t_args) {

        const std::array<float, 9> filter_matrix = {0, -1, 0, -1, 5, -1, 0, -1, 0};

        const std::pair<unsigned int, unsigned int> image_size = t_image.GetImageSize();

        BitmapImage image_copy = t_image;

        for (unsigned int k = 0; k < image_size.first; k++) {
            for (unsigned int j = 0; j < image_size.second; j++) {
                t_image.SetPixelColor(ApplyFilterToPixel(GetSurroundingPixels(image_copy, k, j), filter_matrix), k, j);
            }
        }
    }

    void EdgeDetection(BitmapImage& t_image, const std::vector<float>& t_args) {

        const std::array<float, 9> filter_matrix = {0, -1, 0, -1, 4, -1, 0, -1, 0};

        const std::pair<unsigned int, unsigned int> image_size = t_image.GetImageSize();

        GreyScale(t_image, {});


        BitmapImage image_copy = t_image;


        for (unsigned int k = 0; k < image_size.first; k++) {

            for (unsigned int j = 0; j < image_size.second; j++) {
                t_image.SetPixelColor(ApplyFilterToPixel(GetSurroundingPixels(image_copy, k, j), filter_matrix), k, j);
            }
        }

        for (unsigned int k = 0; k < image_size.first; k++) {

            for (unsigned int j = 0; j < image_size.second; j++) {
                if (t_image.GetPixel(k, j).GetIntensity() > t_args[0]) {
                    t_image.SetPixelColor({1, 1, 1}, k, j);
                } else {
                    t_image.SetPixelColor({0, 0, 0}, k, j);
                }
            }
        }
    }


    void GaussianBlur(BitmapImage& t_image, const std::vector<float>& t_args) {
        std::pair<unsigned int, unsigned int> image_size = t_image.GetImageSize();


        const float effective_radius = 2.7 * t_args[0];


        std::vector<int> gaussian_nodes(effective_radius * 2 + 1);

        for (unsigned int k = 0; k < effective_radius * 2 + 1; k++) {
            gaussian_nodes[k] = -effective_radius + k;
        }


        std::vector<std::vector<float>> gaussian_core =
                CreateGaussianCore(gaussian_nodes, gaussian_nodes, effective_radius + 1, effective_radius + 1, t_args[0]);


        BitmapImage image_copy = t_image;

        for (unsigned int k = 0; k < image_size.first; k++) {
            for (unsigned int j = 0; j < image_size.second; j++) {
                BasicColor new_color = {0, 0, 0};

                int left_boundary = j - effective_radius;
                int gaussian_first_horizontal = 0;

                if (left_boundary < 0) {
                    left_boundary = 0;
                    gaussian_first_horizontal = effective_radius - j + 1;
                }

                unsigned int right_boundary = j + effective_radius;
                if (right_boundary > image_size.second - 1) {
                    right_boundary = image_size.second - 1;
                }

                int top_boundary = k - effective_radius;
                int gaussian_first_vertical = 0;

                if (top_boundary < 0) {
                    gaussian_first_vertical = effective_radius - k + 1;
                    top_boundary = 0;
                }

                unsigned int bot_boundary = k + effective_radius;
                if (bot_boundary > image_size.first - 1) {
                    bot_boundary = image_size.first - 1;
                }


                for (unsigned int k_2 = top_boundary; k_2 < bot_boundary; k_2++) {

                    for (unsigned int j_2 = left_boundary; j_2 < right_boundary; j_2++) {
                        for (unsigned int c = 0; c < 3; c++) {
                            new_color[c] += image_copy.GetPixelColor(k_2, j_2)[c] *
                                            gaussian_core[k_2 - top_boundary + gaussian_first_vertical]
                                            [j_2 - left_boundary + gaussian_first_horizontal];
                        }
                    }
                }


                t_image.SetPixelColor(new_color, k, j);
            }
        }
    }
}  /

class Filter {
private:

    unsigned int m_arguments_num_ = 0;
    std::function<void(BitmapImage& t_image, const std::vector<float>& t_args)> m_filter_;

    std::function<bool(const std::vector<float>& t_args, const unsigned int& t_arguments_num)> m_check_;

public:
    Filter(std::function<void(BitmapImage& t_image, const std::vector<float>& t_args)> t_filter,
    std::function<bool(const std::vector<float>& t_args, const unsigned int& t_arguments_num)> t_check,
    const unsigned int t_arguments_num);

    void ApplyFilter(BitmapImage& m_image, const std::vector<float>& t_args);


    bool DoCheck(const std::vector<float>& t_args);
};


Filter::Filter(std::function<void(BitmapImage& t_image, const std::vector<float>& t_args)> t_filter,
std::function<bool(const std::vector<float>& t_args, const unsigned int& t_arguments_num)> t_check,
const unsigned int t_arguments_num)
: m_arguments_num_(t_arguments_num), m_filter_(t_filter), m_check_(t_check) {
}

inline void Filter::ApplyFilter(BitmapImage& m_image, const std::vector<float>& t_args) {
    m_filter_(m_image, t_args);
}


inline bool Filter::DoCheck(const std::vector<float>& t_args) {
    return m_check_(t_args, m_arguments_num_);
}