scheduler-benchmarks-
=====================

Sample output:
```
phani@phani-thinkpad:~/research/experiments/scheduler-benchmarks$ sudo ./test.sh ./threads > sample_data/threads_ubench.txt
phani@phani-thinkpad:~/research/experiments/scheduler-benchmarks$ sudo ./test.sh ./shm > sample_data/processes_ubench.txt
phani@phani-thinkpad:~/research/experiments/scheduler-benchmarks$ sudo ./test.sh ./timer > sample_data/timer_ubench.txt
phani@phani-thinkpad:~/research/experiments/scheduler-benchmarks$ perl stats.pl < sample_data/threads_ubench.txt 
Samples: 10000
Minimum: 13256
Average: 10754.0275
StdDev:  4509.0408995865
Maximum: 8940
50 %ile: 11676
90 %ile: 11808
95 %ile: 8909
phani@phani-thinkpad:~/research/experiments/scheduler-benchmarks$ perl stats.pl < sample_data/processes_ubench.txt 
Samples: 10000
Minimum: 14732
Average: 11617.8098
StdDev:  3753.28913381636
Maximum: 10475
50 %ile: 14277
90 %ile: 15233
95 %ile: 10260
phani@phani-thinkpad:~/research/experiments/scheduler-benchmarks$ perl stats.pl < sample_data/timer_ubench.txt 
Samples: 10000
Minimum: 9454
Average: 6341.2784
StdDev:  2707.10245042433
Maximum: 6452
50 %ile: 5952
90 %ile: 6524
95 %ile: 5420
phani@phani-thinkpad:~/research/experiments/scheduler-benchmarks$ 
```

Test machine:
1. cpuinfo: (CORE-0 only)
```
processor	: 0
vendor_id	: GenuineIntel
cpu family	: 6
model		: 61
model name	: Intel(R) Core(TM) i5-5200U CPU @ 2.20GHz
stepping	: 4
microcode	: 0x21
cpu MHz		: 2187.195
cache size	: 3072 KB
physical id	: 0
siblings	: 4
core id		: 0
cpu cores	: 2
apicid		: 0
initial apicid	: 0
fdiv_bug	: no
f00f_bug	: no
coma_bug	: no
fpu		: yes
fpu_exception	: yes
cpuid level	: 20
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe nx pdpe1gb rdtscp lm constant_tsc arch_perfmon pebs bts xtopology nonstop_tsc aperfmperf eagerfpu pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch epb intel_pt tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid rdseed adx smap xsaveopt dtherm ida arat pln pts
bugs		:
bogomips	: 4390.06
clflush size	: 64
cache_alignment	: 64
address sizes	: 39 bits physical, 48 bits virtual
power management:
```
2. Os: 
```
Distributor ID:	Ubuntu
Description:	Ubuntu 14.04.5 LTS
Release:	14.04
Codename:	trusty
```
