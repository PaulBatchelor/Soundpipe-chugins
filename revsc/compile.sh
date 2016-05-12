g++ revsc.cpp -L../ -I../include -lsoundpipe -lm -fPIC -shared -DNO_LIBSNDFILE -D__PLATFORM_MACOSX__ -shared -o plugins/RevSC.chug
