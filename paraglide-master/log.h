#ifndef LOG_H
#define LOG_H
#include <robotics_cape.h>
#define LOG_TABLE \
    X(unsigned long, "%lu", time)\
    X(float,  "%f", roll	) \
    X(float,  "%f", pitch	) \
    X(float,  "%f", yaw		) \
	X(float,  "%f", xa      ) \
	X(float,  "%f", ya      ) \
	X(float,  "%f", za      ) \
	X(float,  "%f", xg      ) \
	X(float,  "%f", yg      ) \
	X(float,  "%f", zg      ) \
	X(long, "%li", temperature_raw)\
	X(long, "%li", pressure_raw)\
	X(long, "%li", temperature)\
	X(long, "%li", pressure)

// #define LOG_TABLE_RADIO \
    // X(unsigned long, "%lu", time)\
    // X(int,  "%d", Thr	) \
	// X(int,  "%d", Roll	) \
	// X(int,  "%d", Pitch	) \
	// X(int,  "%d", Yaw	) \
	// X(int,  "%d", Kill	) \
	// X(int,  "%d", Mode	) \
	// X(int,  "%d", Aux1	) \
	// X(int,  "%d", Aux2	) \
	// X(int,  "%d", Aux3	) 
	
	
#define LOG_TABLE_RADIO \
    X(unsigned long, "%lu", time)\
    X(float,  "%0.2f", Thr	) \
	X(float,  "%0.2f", Roll	) \
	X(float,  "%0.2f", Pitch	) \
	X(float,  "%0.2f", Yaw	) \
	X(float,  "%0.2f", Kill	) \
	X(float,  "%0.2f", Mode	) \
	X(float,  "%0.2f", Aux1	) \
	X(float,  "%0.2f", Aux2	) \
	X(float,  "%0.2f", Aux3	) 
	
#define LOG_TABLE_GPS \
    X(int, "%d", year)\
    X(int,  "%d", mon	) \
	X(int,  "%d", day	) \
	X(int,  "%d", hour	) \
	X(int,  "%d", min	) \
	X(int,  "%d", sec	) \
	X(int,  "%d", hsec	) \
	X(float,  "%f", lat	) \
	X(float,  "%f", lon	) \
	X(float,  "%f", elv	) \
	X(float,  "%f", speed)
	
#define X(type, fmt, name) type name ;
typedef struct log_entry_t { LOG_TABLE } log_entry_t;
typedef struct log_entry_r { LOG_TABLE_RADIO} log_entry_r;
typedef struct log_entry_g { LOG_TABLE_GPS} log_entry_g;
#undef X
	
FILE* log_file;
FILE* log_file_r;
FILE* log_file_g;
volatile log_entry_t log_buffer;
volatile log_entry_r log_buffer_r;
volatile log_entry_g log_buffer_g;
volatile int log_flag;

int log_write_entry(volatile log_entry_t* entry);
int log_write_entry_r(volatile log_entry_r* entry);
int log_write_entry_g(volatile log_entry_g* entry);
//void*  log_writer();
int log_start();
int log_stop();

#endif /*LOG_H*/