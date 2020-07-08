include <global.scad>;

module side_shape()
{
    polygon(points = [
        [0, 1],
        [1, 0],
        [depth, 0],
        [depth + 8 * cos(35), 8 * sin(35)],
        [depth + 5 * cos(30), height + 5 * sin(30)],
        [depth, height],
        [1, height],
        [0, height - 1]
    ]);
}

side_shape();