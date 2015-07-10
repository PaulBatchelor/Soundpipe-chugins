SawOsc s => LPF filt => Delay3 del => dac;

120.0 => float tempo;
500 => filt.freq;
2 => del.maxTime;
0.75 * (60.0 / tempo)  => del.time;
1 => s.gain;

0.5 => del.feedback;

0.3 => del.wet;
0.5 => del.dry;

100::ms => now;

0 => s.gain;

while(1::second => now);