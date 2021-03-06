#ifndef POS_FILEDELETE_H
#define POS_FILEDELETE_H

#include "log.h"
#include "current_time.h"
#include "file_ts.h"
#include "misc.h"
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include "profile.h"

struct profile_info_struct* profileos_filedelete_last_interface_empty(testenv_struct* env);
struct profile_info_struct* profileos_filedelete_last_interface_notempty(testenv_struct* env);
struct profile_info_struct* profileos_filedelete_last_interface_symlink(testenv_struct* env);
struct profile_info_struct* profileos_filedelete_last_utilities_empty(testenv_struct* env);
struct profile_info_struct* profileos_filedelete_last_utilities_notempty(testenv_struct* env);
struct profile_info_struct* profileos_filedelete_last_utilities_symlink(testenv_struct* env);

struct profile_info_struct* profileos_filedelete_notlast_interface_empty(testenv_struct* env);
struct profile_info_struct* profileos_filedelete_notlast_interface_notempty(testenv_struct* env);
struct profile_info_struct* profileos_filedelete_notlast_utilities_empty(testenv_struct* env);
struct profile_info_struct* profileos_filedelete_notlast_utilities_notempty(testenv_struct* env);

struct profile_info_struct* profileos_dirdelete_interface(testenv_struct* env);
struct profile_info_struct* profileos_dirdelete_interface_symlink(testenv_struct* env);
struct profile_info_struct* profileos_dirdelete_utilities_rm(testenv_struct* env);
struct profile_info_struct* profileos_dirdelete_utilities_rmdir(testenv_struct* env);
struct profile_info_struct* profileos_dirdelete_utilities_symlink(testenv_struct* env);


#endif
