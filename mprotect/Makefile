all: pkey_gen pkey_speedup pkey_signal pkey_max_anon

pkey_gen: pkey_gen.c
	cc -o pkey_gen pkey_gen.c

pkey_speedup: pkey_speedup.c
	cc -o pkey_speedup pkey_speedup.c

pkey_signal: pkey_signal.c
	cc -o pkey_signal pkey_signal.c

pkey_max_anon: pkey_max_anon.c
	cc -o pkey_max_anon pkey_max_anon.c

clean:
	rm pkey_gen pkey_speedup pkey_signal pkey_max_anon
