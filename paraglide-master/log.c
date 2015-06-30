#include "log.h"
#include <robotics_cape.h>

int log_write_entry(volatile log_entry_t* entry)
{
	#define X(type, fmt, name) fprintf(log_file, fmt "," , entry->name);
    LOG_TABLE
	#undef X	
	fprintf(log_file, "\n");
	return 0;
}

int log_write_entry_r(volatile log_entry_r* entry)
{
	#define X(type, fmt, name) fprintf(log_file_r, fmt "," , entry->name);
    LOG_TABLE_RADIO
	#undef X	
	fprintf(log_file_r, "\n");
	return 0;
}

int log_write_entry_g(volatile log_entry_g* entry)
{
	#define X(type, fmt, name) fprintf(log_file_g, fmt "," , entry->name);
    LOG_TABLE_GPS
	#undef X	
	fprintf(log_file_g, "\n");
	return 0;
}


int log_start()
{
	char time_str[50];
	char logfile_path[100];
	char logfile_path_r[100];
	char logfile_path_g[100];
    time_t t;
    struct tm *tmp;
	t = time(NULL);
    tmp = localtime(&t);
    if (tmp == NULL) {
        printf("can't access localtime\n");
		return -1;
    }
    if (strftime(time_str, sizeof(time_str),
		"%a %d %b %Y %R:%S", tmp) == 0) {
        printf("strftime returned 0");
        return -1;
    }
	

	// construct new logfile name
	strcpy (logfile_path, LOG_DIRECTORY);
	strcat (logfile_path, time_str);
	strcat (logfile_path, ".csv");
	printf("starting new logfile\n");
	printf("%s\n", logfile_path);
	
	strcpy (logfile_path_r, LOG_DIRECTORY);
	strcat (logfile_path_r, time_str);
	strcat (logfile_path_r, "_radio.csv");
	printf("starting new radio logfile\n");
	printf("%s\n", logfile_path_r);
	
	strcpy (logfile_path_g, LOG_DIRECTORY);
	strcat (logfile_path_g, time_str);
	strcat (logfile_path_g, "_gps.csv");
	printf("starting new gps logfile\n");
	printf("%s\n", logfile_path_g);
	
	log_file = fopen(logfile_path, "w");
	if (log_file==NULL){
		printf("could not open logging directory\n");
		return -1;
	}
	
	#define X(type, fmt, name) fprintf(log_file, "%s," , #name);
    LOG_TABLE
	#undef X	
	fprintf(log_file, "\n");
	fflush(log_file);
	
	log_file_r = fopen(logfile_path_r,"w");
	if (log_file_r==NULL){
		printf("could not open radio logging directroy\n");
		return -1;
	}
	#define X(type, fmt, name) fprintf(log_file_r, "%s," , #name);
    LOG_TABLE_RADIO
	#undef X	
	fprintf(log_file_r, "\n");
	fflush(log_file_r);
	
	log_file_g = fopen(logfile_path_g,"w");
	if (log_file_g==NULL){
		printf("could not open gps logging directroy\n");
		return -1;
	}
	#define X(type, fmt, name) fprintf(log_file_g, "%s," , #name);
    LOG_TABLE_GPS
	#undef X	
	fprintf(log_file_g, "\n");
	fflush(log_file_g);
	
	log_flag = 1;
	return 0;
}

int log_stop()
{
	fclose(log_file);
	return 0;
}