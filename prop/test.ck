Prop p => blackhole;
p.parse("2(++)++3(+++)");
while(1) 
{
    p.next();
    if(p.type()) <<< "Bing!" >>>;
    p.dur()::second => now;
}
