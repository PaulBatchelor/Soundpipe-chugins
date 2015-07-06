LSamp ls => dac;
ls.open("909.smp");
while(1) 
{
    ls.select("snare_01");
    0.1::second => now;
}
