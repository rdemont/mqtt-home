use<lib/objLib.scad>

$fn=60;


//screwHole(20);


/*
//box
difference()
{
    cube([120,60,60]);
    translate([-1,-1,11])rotate([45,0,0])cube([140,100,100]);
    translate([2,2,2])cube([116,56,56]);
    translate([10,23,-1])cube([21,16,56]); //trou DHT
    translate([108,48,-1])cylinder(10,7); //trou courant
}

translate([6,23,2])cube([4,16,2]); //support gauche DHT
translate([31,23,2])cube([4,16,5]); //support droite DHT
*/



//couvercle ... manque les vis pour l'écran 
/*
difference()
{
    //plateau
    group() {
        cube([120,70,2]);
        translate([2,2,2])cube([116,66,2]);
    }
    //trou écran
    translate([60,34.5,-1])resize([80,55,0])rotate([0,0,45])cylinder(6,d1=60,d2=48,$fn=4);
    
    //coté 
    translate([-1,-1,-(sqrt(2)*20)+3])rotate([45,0,0])cube([140,20,20]);
    translate([-1,70,-(sqrt(2)*20)+3])rotate([45,0,0])cube([140,20,20]);
}
*/
