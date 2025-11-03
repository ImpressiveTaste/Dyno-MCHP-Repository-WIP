// template.scad


$fn=60;

// variables

length = 180;
width = 115;

main();


module main()
{
    
    
    difference(){
        plus();
        minus();

    }
}


module plus()
{ 
    // base plate
    translate([0,0,0])rotate([0,0,0])cube([length,115,4],false);
    
    // base externders
    translate([15,0,4])rotate([0,0,0])cube([5,width,4],false);
    translate([length-20-5,0,4])rotate([0,0,0])cube([5,width,4],false);
    
    
    // PCB screws
    difference()
    {
        translate([13,width-4.5,0])rotate([0,0,0])cylinder(12,9/2,9/2);
        translate([13,width-4.5,0])rotate([0,0,0])cylinder(12,2/2,2/2);
    }
    difference()
    {
        translate([4.5,4.5,0])rotate([0,0,0])cylinder(12,9/2,9/2);
        translate([4.5,4.5,0])rotate([0,0,0])cylinder(12,2/2,2/2);
    }
    difference()
    {
        translate([length-4.5,4.5,0])rotate([0,0,0])cylinder(12,9/2,9/2);
        translate([length-4.5,4.5,0])rotate([0,0,0])cylinder(12,2/2,2/2);
    }
    difference()
    {
        translate([length-4.5,width-4.5,0])rotate([0,0,0])cylinder(12,9/2,9/2);
        translate([length-4.5,width-4.5,0])rotate([0,0,0])cylinder(12,2/2,2/2);
    }
    
}

module minus()
{
    // T-nut screw holes
    translate([30,30,0])rotate([0,0,0])cylinder(4,6/2,6/2);
    translate([145,30,0])rotate([0,0,0])cylinder(4,6/2,6/2);
    translate([30,105,0])rotate([0,0,0])cylinder(4,6/2,6/2);
    translate([145,105,0])rotate([0,0,0])cylinder(4,6/2,6/2);
    
    
    // base hole
    translate([40,10,0])rotate([0,0,0])cube([95,95,4],false);
}


module cube_r(x,y,z,r)
{
    hull()
    {
        translate([r,r,r])cube([x-2*r,y-2*r,z-2*r]);
        translate([r,r,r])rotate([0,0,0])cylinder(z-2*r,r,r);
        translate([r,y-r,r])rotate([0,0,0])cylinder(z-2*r,r,r);
        translate([x-r,r,r])rotate([0,0,0])cylinder(z-2*r,r,r);
        translate([x-r,y-r,r])rotate([0,0,0])cylinder(z-2*r,r,r);
        
        translate([r,r,r])rotate([0,90,0])cylinder(x-2*r,r,r);
        translate([r,y-r,r])rotate([0,90,0])cylinder(x-2*r,r,r);
        translate([r,y-r,z-r])rotate([0,90,0])cylinder(x-2*r,r,r);
        translate([r,r,z-r])rotate([0,90,0])cylinder(x-2*r,r,r);
        
        translate([x-r,y-r,r])rotate([90,0,0])cylinder(y-2*r,r,r);
        translate([r,y-r,r])rotate([90,0,0])cylinder(y-2*r,r,r);
        translate([r,y-r,z-r])rotate([90,0,0])cylinder(y-2*r,r,r);
        translate([x-r,y-r,z-r])rotate([90,0,0])cylinder(y-2*r,r,r);
        
        translate([r,r,r])sphere(r = r);
        translate([x-r,r,r])sphere(r = r);
        translate([r,y-r,r])sphere(r = r);
        translate([x-r,y-r,r])sphere(r = r);
        
        translate([r,r,z-r])sphere(r = r);
        translate([x-r,r,z-r])sphere(r = r);
        translate([r,y-r,z-r])sphere(r = r);
        translate([x-r,y-r,z-r])sphere(r = r);
    }
}

module cylinder_r(h,r1,r2,r3,r4)
{
    hull()
    {
        translate([0,0,r3])cylinder(h-r3-r4,r1-r3,r2-r4);
        
        translate([0,0,r3])rotate_extrude(convexity = 10, $fn = 100)
            translate([r1, 0, 0])circle(r = r3, $fn = 100);
        
        translate([0,0,h-r4])rotate_extrude(convexity = 10, $fn = 100)
            translate([r2, 0, 0])circle(r = r4, $fn = 100);
    }
    
}

module rectTriangle(a,b,h)
{
linear_extrude(height = h, center = false, convexity = 10, twist = 0)
    polygon(points=[[0,0],[a,0],[0,b]]);   
}


