FOsc f => Envelope e => dac;


function void timbre() {
    while(10::samp => now) {
        e.last() * 1 => f.index;
    }
}

spork ~ timbre();

1 => e.time;
e.keyOn();
3::second => now;
e.keyOff();
3::second => now;