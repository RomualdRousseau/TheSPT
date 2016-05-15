include<global.scad>

use <hardware.scad>;
use <electronic.scad>;

use <side_r.scad>;
use <side_l.scad>;
use <part1.scad>;
use <part2.scad>;
use <part3.scad>;
use <part4.scad>;
use <part5.scad>;
use <part6.scad>;
use <bottom_door.scad>;
use <gimbal joystick.scad>;
use <trim.scad>;
use <pcb.scad>;

module box()
{
    translate([explode_dist + width, 0, 0]) rotate([90,0,90]) side_r();
    translate([-explode_dist - thickness, 0, 0]) rotate([90,0,90]) side_l();
    translate([0, 1, height - thickness + explode_dist]) part1();
    translate([0, depth, height + explode_dist]) rotate([30, 0, 0]) translate([0, 0, -thickness]) part2();
    translate([0, depth, -explode_dist]) rotate([35, 0, 0]) part3();
    translate([0, thickness - explode_dist, 1]) rotate([90, 0, 0]) part4();
    translate([0, -explode_dist, height - 1]) rotate([45, 0, 0]) translate([0, 0, -thickness]) part5();
    translate([0, -explode_dist, 1]) rotate([-45, 0, 0]) part5();
    translate([0, depth + 5 * cos(30) + explode_dist, height + 5 * sin(30)]) rotate([-56.909, 0, 0]) translate([0, thickness, -thickness]) part6();
    translate([0, 1, -explode_dist]) bottom_door();
    translate([0, 1, -explode_dist * 2]) bottom_door_feet();
}

module box_skeleton()
{
    translate([width, 0, 0]) rotate([90,0,90]) side_r_skeleton();
    translate([-thickness, 0, 0]) rotate([90,0,90]) side_l_skeleton();
    translate([0, 1, height - thickness]) part1_skeleton();
    translate([0, thickness - explode_dist, 1]) rotate([90, 0, 0]) part4_skeleton();
    translate([0, depth + 5 * cos(30), height + 5 * sin(30)]) rotate([-56.909, 0, 0]) translate([0, thickness, -thickness]) part6_skeleton();
    translate([0, 1, 0]) bottom_door_skeleton();
}

module electronics()
{
    translate([1, 3.5, thickness + 0.55]) pcb();
    translate([width / 4, depth * 2 / 3 + 1.45, height - thickness]) joystick(angle_x = 30 * cos(360 * $t), angle_y = 30 * sin(360 * $t));
    translate([1.5 + thickness, depth - 1.5, thickness + 1]) rotate([90, 0, -90]) trim(angle = 10 * sin(360 * $t));
    translate([width / 4, depth * 2 / 3 - 2, height - thickness - 1.8]) rotate([0, 0, 180]) trim(angle = 10 * sin(360 * $t));
    translate([width * 3 / 4, depth * 2 / 3 + 1.45, height - thickness]) joystick(gaz = true, angle_x = -30 * cos(360 * $t), angle_y = -30 * sin(360 * $t));
    translate([width - 1.5 - thickness, depth - 1.5, thickness + 1]) rotate([90, 0, 90]) trim(angle = 10 * sin(360 * $t));
    translate([width * 3 / 4, depth * 2 / 3 - 2, height - thickness - 1.8]) rotate([0, 0, 180]) trim(angle = 10 * sin(360 * $t));
    translate([width / 2, depth * 2 / 3 - 1, height - thickness]) rotate([0, 0, 90]) switch(angle=-10 * sin(360 * $t), thickness = thickness);
    translate([width / 2, depth * 2 / 3 + 1, height - thickness - 0.1]) led_green();
    translate([0, depth + 2 * cos(35), 1.5 + 2 * sin(35)]) rotate([90, 35, -90]) switch(angle = -10 * sin(360 * $t), length = 2.1, thickness = thickness);
    translate([width, depth + 2 * cos(35), 1.5 + 2 * sin(35)]) rotate([90, -35, 90]) switch(angle = -10 * sin(360 * $t), length = 2.1, thickness = thickness);
    translate([width - 3, depth + 2.5 * cos(30), height - thickness - 0.05 + 2.5 * sin(30)]) rotate([0, 30, -90]) switch(angle = -10 * sin(360 * $t), length = 2.1, thickness = thickness);  
    translate([width + thickness + 0.1, 8.5, thickness + 1.3]) rotate([90, 0, 90]) rs232(thickness = thickness);
    translate([0, depth + 5 * cos(30), height + 5 * sin(30)]) rotate([-56.909, 0, 0]) translate([3, thickness + 1.73591, 0]) antenna();
    translate([(width - 15.5) / 2, 0.5 + thickness, thickness]) battery();
    translate([thickness + 7.25, 5, height - thickness]) lcd_16x2();
    translate([thickness + 11.75, 5, height - thickness]) switch_push(angle = 0, length = 0.5, thickness = thickness);  
}

module transmitter()
{
    box();
    box_skeleton();
    electronics();
}

transmitter();

