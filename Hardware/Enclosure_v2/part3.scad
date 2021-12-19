include <global.scad>;

module part3()
{
    color("SaddleBrown") difference()
    {
        cube(size = [width, 8, thickness]);
    }
}

module part3_2d()
{
    projection() part3();
}

part3();