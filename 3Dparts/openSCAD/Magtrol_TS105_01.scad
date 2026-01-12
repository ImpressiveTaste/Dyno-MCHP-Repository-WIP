// template.scad


$fn=60;

// variables



thickness = 4.5;

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
    translate([0,0,thickness/2])rotate([0,0,0])cube([65+25,49+30,thickness],true);
 //   translate([0,0,12])rotate([0,0,0])cylinder(l1,12/2,11.8/2);
}

module minus()
{
    hull()
    {
        translate([25+12.5,15+15,0])rotate([0,0,0])cylinder(thickness,5.5/2,5.5/2);
        translate([25+12.5,-15-15,0])rotate([0,0,0])cylinder(thickness,5.5/2,5.5/2);
    }
    
    hull()
    {
        translate([-25-12.5,15+15,0])rotate([0,0,0])cylinder(thickness,5.5/2,5.5/2);
        translate([-25-12.5,-15-15,0])rotate([0,0,0])cylinder(thickness,5.5/2,5.5/2);
    }
    
    translate([25,15,0])rotate([0,0,0])cylinder(3.7,9.4/2,9.4/2,$fn=6);
    translate([25,-15,0])rotate([0,0,0])cylinder(3.7,9.4/2,9.4/2,$fn=6);
    translate([-25,15,0])rotate([0,0,0])cylinder(3.7,9.4/2,9.4/2,$fn=6);
    translate([-25,-15,0])rotate([0,0,0])cylinder(3.7,9.4/2,9.4/2,$fn=6);
    
    translate([25,15,0])rotate([0,0,0])cylinder(thickness,5.5/2,5.5/2);
    translate([25,-15,0])rotate([0,0,0])cylinder(thickness,5.5/2,5.5/2);
    translate([-25,15,0])rotate([0,0,0])cylinder(thickness,5.5/2,5.5/2);
    translate([-25,-15,0])rotate([0,0,0])cylinder(thickness,5.5/2,5.5/2);
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


