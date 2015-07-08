LSamp ls => dac;
ls.open("909.smp");
while(1) 
{
    ls.select("kick_01");
    0.1::second => now;
}
