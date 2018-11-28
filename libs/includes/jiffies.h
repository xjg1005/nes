#ifndef JIFFIES_H
#define JIFFIES_H


/* timer interface */
/* Parameters used to convert the timespec values: */
#define HZ				100L
#define MSEC_PER_SEC	1000L
#define USEC_PER_MSEC	1000L
#define NSEC_PER_USEC	1000L
#define NSEC_PER_MSEC	1000000L
#define USEC_PER_SEC	1000000L
#define NSEC_PER_SEC	1000000000L
#define FSEC_PER_SEC	1000000000000000LL


#define time_before(tck1,tck2)					((tck1) < (tck2))
#define time_after(a,b)							time_before(b,a)

extern unsigned long jiffies;

#define jiffies_to_msecs(j) j

#define msecs_to_jiffies(j) j

//unsigned long jiffies_to_msecs(unsigned long j);
//unsigned long msecs_to_jiffies(unsigned long m);

#endif

