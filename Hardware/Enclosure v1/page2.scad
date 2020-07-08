include<global.scad>

use <side_r.scad>;
use <side_l.scad>;

//color("grey") translate([0, 0, -1]) cube(size = [42, 29.7, 0.5], center = true);

translate([-3, -5, 0]) side_l_2d();
translate([-3, 5, 0]) side_r_2d();
