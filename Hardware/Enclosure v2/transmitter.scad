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

module electronics(solid_holes = false)
{
    translate([width - 2 * thickness - 18.0, thickness + 4.8, thickness + 0.55]) pcb(extended_ports = solid_holes);
    translate([width + thickness + 0.1, 10.5, thickness + 1.3]) rotate([90, 0, 90]) rs232(thickness = thickness);
    
    translate([thickness + 5, depth * 8 / 10, height - thickness - 0.025]) joystick(angle_x = 30 * cos(360 * $t), angle_y = 30 * sin(360 * $t), solid_holes = solid_holes);
    translate([thickness + 1.5, depth * 8 / 10, thickness + 1]) rotate([90, 0, -90]) trim(angle = 10 * sin(360 * $t), solid_holes = solid_holes);
    translate([thickness + 5, depth * 8 / 10 - 4, height - thickness - 1.8]) rotate([0, 0, 180]) trim(angle = 10 * sin(360 * $t), solid_holes = solid_holes);
    
    translate([width - 5 - thickness, depth * 8 / 10, height - thickness - 0.025]) joystick(gaz = true, angle_x = -30 * cos(360 * $t), angle_y = -30 * sin(360 * $t), solid_holes = solid_holes);
    translate([width - thickness - 1.5, depth * 8 / 10, thickness + 1]) rotate([90, 0, 90]) trim(angle = 10 * sin(360 * $t), solid_holes = solid_holes);
    translate([width - 5 - thickness, depth * 8 / 10 - 4, height - thickness - 1.8]) rotate([0, 0, 180]) trim(angle = 10 * sin(360 * $t), solid_holes = solid_holes);
    
    translate([width / 2, depth * 6 / 10 + 2, height - thickness - 0.1]) led_green();
    translate([width / 2, depth * 6 / 10, height - thickness]) rotate([0, 0, 90]) switch(angle=-10 * sin(360 * $t), thickness = thickness);
    
    translate([0, depth - 2, 1.5 + 2 * sin(35)]) rotate([90, 35, -90]) switch(angle = -10 * sin(360 * $t), length = 2.1, thickness = thickness);
    translate([width, depth - 2, 1.5 + 2 * sin(35)]) rotate([90, -35, 90]) switch(angle = -10 * sin(360 * $t), length = 2.1, thickness = thickness);
    translate([width - 1.7, depth - 1, height - thickness]) rotate([0, 0, -90]) switch(angle = -10 * sin(360 * $t), length = 2.1, thickness = thickness);  
    
    translate([3.5, depth, height / 2]) rotate([-90, 0, 0]) antenna();
    
    translate([width - 16.5, 4.9, height - thickness - 2.4]) rotate([90, 0, 0]) battery2();
    
    translate([thickness + 5, depth * 3 / 10, height - thickness]) lcd_16x2(solid_holes = solid_holes);
    translate([thickness + 9.75, depth * 3 / 10, height - thickness + 0.025]) small_switch_push(thickness = thickness);  
}

module mobile()
{
    translate([(width - 15.5) / 2, depth - 1.2, height]) rotate([45, 0, 0]) color("black") cube([15.5, 7.5, 0.9]);
}

module box2()
{
    translate([0, 0, 0]) cube([width, depth, thickness]);
    %translate([0, 0, height - thickness]) cube([width, depth, thickness]);
    
    translate([-0.3, -0.5, 0]) cube([0.3, depth + 1, height]);
    translate([width, -0.5, 0]) cube([0.3, depth + 1, height]);
    
    translate([0, -0.5, 0]) cube([width, 0.5, height]);
    translate([0, depth, 0]) cube([width, 0.5, height]);
    
    translate([(width - 16) / 2, depth - 1.5 + 0.5, height - 0.2]) rotate([45, 0, 0]) cube([16, 8, thickness]);
    translate([(width - 16) / 2, depth + 0.2, height]) cube([16, thickness, 1.3]);
}

module transmitter()
{
    
    difference()
    {
        box2();
        electronics(solid_holes = true);
    }
    
    mobile();
    
    electronics();
}

transmitter();

