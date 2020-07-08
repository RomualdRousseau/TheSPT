include<global.scad>

use <part1.scad>;
use <part2.scad>;
use <part3.scad>;
use <part4.scad>;
use <part5.scad>;
use <part6.scad>;
use <bottom_door.scad>;

//color("grey") translate([0, 0, -1]) cube(size = [42, 29.7, 0.5], center = true);

translate([-20.5, 0.5, 0]) part1_2d();
translate([-20.5, -14.5, 0]) bottom_door_2d();
translate([0.5, -5.5, 0]) part2_2d();
translate([0.5, -14.5, 0]) part3_2d();
translate([0.5, 0.5, 0]) part4_2d();
translate([0.5, 5, 0]) part5_2d();
translate([0.5, 7.5, 0]) part6_2d();
