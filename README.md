# image_processor
Implemented console application,
allowing you to apply various filters to images,
similar to filters in popular graphic editors.

~Supported image format: input and output graphic files must be in BMP format.

~Command line argument format description:
{program name} {path to input file} {path to output file} [-{filter name 1} [filter option 1] [filter option 2] ...] [-{filter name 2} [filter option 1] [ filter parameter 2] ...] ...

~List of basic filters: Crop (-crop width height), Grayscale (-gs), Negative (-neg), Sharpening (-sharp), Edge Detection (-edge threshold), Gaussian Blur (-blur sigma)
