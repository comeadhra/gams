#!/usr/bin/perl
$gams_root=$ENV{'GAMS_ROOT'};
require "$gams_root/scripts/simulation/launch.pl";

$time=3600;
$madara_debug=0;
$period=0.5;
$num=3;
$sim="urec";
$area="small";

run($num, $time, $period, $sim, $area, $madara_debug);