module screwHole(length)
{

cylinder(h=2,r1=3.5,r2=1.1) ;
cylinder(h=length,r=1.1);
    
}


module screwingHole(length)
{

cylinder(h=length,r=1);
    
}

//$fn=30;
screwHole(10);