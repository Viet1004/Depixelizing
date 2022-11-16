This project is done under the course MAP586

The goal of this project is to smooth out the the pixel arts.

The implementation of our algorithm consists of three steps:

1. Group the related pixels together. This is for detecting and separating different parts of the image. The rule of grouping is based on three categories:
+ If two pixels are part of a long curve feature, they should be connected.
+ in two-colored drawings, humans tend to perceive the sparser color as foreground and the other color as background. In this case we perceive the foreground pixels as connected (e.g., think of a dotted pencil line).
+ we attempt to avoid fragmentation of the figure into too many small components. Therefore, we avoid creating small disconnected pixel.

2. After fragmenting the image into different groups, we reshape the pixels so that their shapes is more natural. For example, if the pixel is at the end of the curve, its shape should be a bit more round instead of a square as usual

3. After changing the shape of the pixel, we use spline curve to smooth out the shape

The executable files are in the folder "build"

To run the code, simple type 
```
./DEPIXELIZER input_path output_path
```

For the detailed implementation of the algorithm, see [here](https://johanneskopf.de/publications/pixelart/paper/pixel.pdf)

For brief walkthrough of our implementation, see the report


