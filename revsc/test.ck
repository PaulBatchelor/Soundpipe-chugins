SawOsc s => RevSC r => dac;
0.9 => r.feedback;
0.1 => s.gain;

1::second => now;

0 =>s.gain;

while(1) 
{
    1::second => now;
}
