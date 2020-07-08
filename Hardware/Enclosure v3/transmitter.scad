include<global.scad>

use <hardware.scad>;
use <electronic.scad>;
use <gimbal joystick2.scad>;
use <trim.scad>;
use <pcb.scad>;

module uvc_receiver_5_8G()
{
    color("DarkSlateGray") cube([6, 4, 1]);
}

module switch_push(thickness = 0.3)
{
    color("Silver") 
    {
        translate([0, 0, -0.6 - thickness]) cylinder(d = 1, h = 1.01 + thickness);
        translate([-0.75, -0.75, 0.01]) cube([1.5, 1.5, 0.9]);
        translate([-0.5, -0.5, 0.91]) cube([1, 1, 0.4]);
     }
}

module electronics(solid_holes = false)
{
    p1 = depth * 5 / 10;
    p2 = depth * 4 / 10;
    p3 = 4.5;

    // Electronic
    translate([width/2 + 10.6, depth, thickness + 0.6]) rotate([0, 0, 180]) pcb(thickness = thickness);
    translate([width, depth - 3, thickness + 2.3]) rotate([90, 0, 90]) rs232(thickness = thickness + 0.2, extended_ports = solid_holes);
    translate([0, depth - 3, thickness + 2.3]) rotate([0, -90, 0]) rotate([0, 0, 90]) usb_b(thickness = thickness + 0.2, extended_ports = solid_holes);
    translate([width/2 + 1.5, depth, height - thickness - 0.6]) rotate([-90, 0, 0]) usb_minia(thickness = thickness + 0.2, extended_ports = solid_holes);
    
    translate([width - 18, 4.91, height - thickness - 2.41]) rotate([90, 0, 0]) battery2();
    
    translate([2.8, depth + 0.51, (height - 0.5) / 2 + 0.5]) rotate([-90, 0, 0]) antenna(extended_ports = solid_holes);   
    translate([width - 2.8, depth + 0.51, (height - 0.5) / 2 + 0.5]) rotate([-90, 0, 0]) antenna(extended_ports = solid_holes);
    
    // Command left
    translate([thickness + p3 + 0.2, p1 - 0.45, height - thickness - 0.2]) rotate([0, 0, 180]) joystick(gaz = true, angle_x = 30 * cos(360 * $t), angle_y = 30 * sin(360 * $t), thickness = thickness + 0.2, solid_holes = solid_holes);
    translate([thickness + 1.5, p1 + 2, thickness + 1]) rotate([90, 0, -90]) trim(angle = 10 * sin(360 * $t), thickness = thickness + 0.2, solid_holes = solid_holes);
    translate([thickness + p3, p1 - 3.8, height - thickness - 1.8]) rotate([0, 0, 180]) trim(angle = 10 * sin(360 * $t), thickness = thickness, solid_holes = solid_holes);
    translate([0, p1 + 4, 1.5 + 2 * sin(35)]) rotate([90, 35, -90]) switch(angle = -10 * sin(360 * $t), length = 2.1, thickness = thickness + 0.2);
    
    // Command right
    translate([width - p3 - thickness - 0.2, p1 - 0.45, height - thickness - 0.2]) joystick(gaz = true, angle_x = -30 * cos(360 * $t), angle_y = -30 * sin(360 * $t), thickness = thickness + 0.2, solid_holes = solid_holes);
    translate([width - thickness - 1.5, p1 + 2, thickness + 1]) rotate([90, 0, 90]) trim(angle = 10 * sin(360 * $t), thickness = thickness + 0.2, solid_holes = solid_holes);
    translate([width - p3 - thickness, p1 - 3.8, height - thickness - 1.8]) rotate([0, 0, 180]) trim(angle = 10 * sin(360 * $t), thickness = thickness, solid_holes = solid_holes);
    translate([width, p1 + 4, 1.5 + 2 * sin(35)]) rotate([90, -35, 90]) switch(angle = -10 * sin(360 * $t), length = 2.1, thickness = thickness + 0.2);    
    
    // Switch
    translate([width - 2.7, p1 + 5.5, height - thickness]) rotate([0, 0, -90]) switch(angle = -10 * sin(360 * $t), length = 2.1, thickness = thickness + 0.1);  
    
    // Control
    translate([width / 2, p2 + 1, height - thickness]) lcd_16x2(thickness = thickness, solid_holes = solid_holes);
    translate([width / 2 + 2.5, p2 - 1.5, height - thickness + 0.4]) switch_push(thickness = thickness + 0.1);
    translate([width / 2, p2 - 1.5, height - thickness - 0.1]) led_green();
    translate([width / 2 - 2.5, p2 - 1.5, height - thickness]) rotate([0, 0, 90]) switch(angle=-10 * sin(360 * $t), thickness = thickness + 0.1);  

    // UVC receiver
    translate([width - 9.5, depth - 5, height - 1.3]) uvc_receiver_5_8G();
    
    // handle
    translate([width / 2 - 4.55, depth + thickness + 0.3, height / 2]) rotate([90, 0, 0]) cylinder(d=0.3, h = thickness + 0.4);
    translate([width / 2 + 4.55, depth + thickness + 0.3, height / 2]) rotate([90, 0, 0]) cylinder(d=0.3, h = thickness + 0.4);
}

module mobile(solid_holes = false)
{
    if (!solid_holes)
    {
        color("DarkSlateGray") translate([width/2, depth - 3, height]) cylinder(d = 2.9, h = 2.8);
        color("DarkSlateGray") translate([(width - 15.5) / 2, depth - 7.4, height + 1.9]) rotate([45, 0, 0]) cube([15.5, 7.5, 0.9]);
    }
    
    if (solid_holes)
    {
        translate([width/2, depth - 3, height - thickness - 0.1]) cylinder(d = 0.6, h = thickness + 0.2);
        translate([width/2 - 0.55, depth - 3 - 0.45, height - thickness - 0.1]) cylinder(d = 0.4, h = thickness + 0.2);
        translate([width/2 + 0.55, depth - 3 + 0.45, height - thickness - 0.1]) cylinder(d = 0.4, h = thickness + 0.2);
    }
}

module renfort(s = [1, 1], solid_holes = false)
{
    if (!solid_holes) color("brown") scale([s[0], s[1], 1]) difference()
    {
        linear_extrude(thickness) polygon([[0, 0], [0, 2], [0.5, 2], [2, 0.5], [2, 0]]);
        translate([0.9, 0.9, -0.1]) cylinder(d = 0.5, h = thickness + 0.2);
    }
    
    if (solid_holes) color("brown") scale([s[0], s[1], 1])
    {
        linear_extrude(thickness) polygon([[0, 0], [0, 2], [0.5, 2], [2, 0.5], [2, 0]]);
        translate([0.9, 0.9, -1]) cylinder(d = 0.3, h = 1);
    }
}

module renforts(solid_holes = false)
{
    translate([-0.5, -0.5, thickness]) renfort([1, 1], solid_holes = solid_holes);
    translate([width + 0.5, depth + 0.5, thickness]) renfort([-1, -1], solid_holes = solid_holes);
    translate([width + 0.5, -0.5, thickness]) renfort([-1, 1], solid_holes = solid_holes);
    translate([-0.5, depth + 0.5, thickness]) renfort([1, -1], solid_holes = solid_holes);
    
    translate([-0.5, -0.5, height - thickness*2]) renfort([1, 1], solid_holes = solid_holes);
    translate([width + 0.5, depth + 0.5, height - thickness*2]) renfort([-1, -1], solid_holes = solid_holes);
    translate([width + 0.5, -0.5, height - thickness*2]) renfort([-1, 1], solid_holes = solid_holes);
    translate([-0.5, depth + 0.5, height - thickness*2]) renfort([1, -1], solid_holes = solid_holes);
    
    translate([1, 0, thickness]) color("brown") cube([width - 2, thickness, thickness]);
    translate([1, depth - thickness, thickness]) color("brown") cube([width - 2, thickness, thickness]);
    translate([0, 1, thickness]) color("brown") cube([thickness, depth - 2, thickness]);
    translate([width - thickness, 1, thickness]) color("brown") cube([thickness, depth - 2, thickness]);
}

module box_up()
{
    difference()
    {
        color("brown") translate([0, 0, height - thickness]) cube([width, depth, thickness]);
        electronics(solid_holes = true);
        renforts(solid_holes = true);
        mobile(solid_holes = true);
    }
}

module box_bottom()
{
    difference()
    {
        color("brown") translate([0, 0, 0]) cube([width, depth, thickness]);
        electronics(solid_holes = true);
        renforts(solid_holes = true);
        mobile(solid_holes = true);
    }
}

module box_left()
{
    difference()
    {
        color("brown") translate([-0.5, -0.5, 0]) cube([0.5, depth + 1, height]);
        electronics(solid_holes = true);
        renforts(solid_holes = true);
        mobile(solid_holes = true);
    }
}

module box_right()
{
    difference()
    {
        color("brown") translate([width, -0.5, 0]) cube([0.5, depth + 1, height]);
        electronics(solid_holes = true);
        renforts(solid_holes = true);
        mobile(solid_holes = true);
    }
}

module box_back()
{
    difference()
    {
        color("brown") translate([0, -0.5, 0]) cube([width, 0.5, height]);
        electronics(solid_holes = true);
        renforts(solid_holes = true);
        mobile(solid_holes = true);
    }
}

module box_front()
{
    difference()
    {
        color("brown") translate([0, depth, 0]) cube([width, 0.5, height]);
        electronics(solid_holes = true);
        renforts(solid_holes = true);
        mobile(solid_holes = true);
    }
}

module box_deco()
{
    p1 = depth * 5 / 10;
    p2 = depth * 4 / 10;
    p3 = 4.5;
    
    difference()
    {
        color("orange") union()
        {
            translate([thickness + p3 + 0.2, p1 - 0.45, height]) cylinder(h = 0.1, d = 9);
            translate([width - p3 - thickness - 0.2, p1 - 0.45, height]) cylinder(h = 0.1, d = 9);
            
            translate([(width - 10) / 2, 3, height]) rcube([10, 10, 0.1], 0.5);
        }
        electronics(solid_holes = true);
        renforts(solid_holes = true);
        mobile(solid_holes = true);
    }
}

module main()
{
    box_up();
    //box_bottom();
    box_left();
    box_right();
    box_back();
    box_front();
    box_deco();
    
    renforts();
    
    mobile();
    
    electronics();
}

module schema()
{
    projection()
    {
        box_up();
        
        translate([35, 0, 0]) rotate([0, 90, 0]) box_right();
        translate([-10, 0, 0]) rotate([0, 90, 0]) box_left();
        
        translate([0, 30, 0]) rotate([90, 0, 0]) box_front();
        translate([0, -5, 0]) rotate([90, 0, 0]) box_back();
        
        translate([0, 35, 0]) box_bottom();
        
        translate([-10, 30, 0]) renfort();
        translate([-5, 30, 0]) renfort();
        translate([-10, 35, 0]) renfort();
        translate([-5, 35, 0]) renfort();
        
        translate([-10, 40, 0]) renfort();
        translate([-5, 40, 0]) renfort();
        translate([-10, 45, 0]) renfort();
        translate([-5, 45, 0]) renfort();
        
        translate([0, 55, 0]) box_deco();
        
        translate([32, 35, 0]) rotate([0, 0, 90]) cube([width - 2, thickness, thickness]);
        translate([34, 35, 0]) rotate([0, 0, 90]) cube([width - 2, thickness, thickness]);
        translate([36, 35, 0]) cube([thickness, depth - 2, thickness]);
        translate([38, 35, 0]) cube([thickness, depth - 2, thickness]);
    }
}

main();

