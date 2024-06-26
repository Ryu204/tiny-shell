#pragma once

#include <stdbool.h>

#include "../core/args.h"
#include "type.h"

/*! @brief Fetch current working directory and copy them into `buffer` */
void get_cwd(unsigned int buffer_size, os_char *buffer);

/**
 * @brief Attempt to change current working directory
 * @return Result of the operation
 */
bool change_cwd(const os_char *new_dir);

/*! @brief Clear the console */
void clear_screen();

/*! @brief Kill a process */
bool kill_process(int proc_id);

/*! @brief Resume running a process */
bool resume(int proc_id);

/*! @brief Show child processes */
bool show_child_processes(int proc_id);

/*! @brief Delete a specific file */
bool delete_file(const os_char *filename);

/*! @brief List of files or folders in specific directory */
bool lsdir(const os_char *dir);

/*! @brief Launch the executable */
bool launch_executable(const struct args args);

/**
 * @brief Set environment variable of the shell process
 * @param `val` may be `NULL`
 */
bool set_shell_env(const os_char *name, const os_char *val);

/**
 * @brief Unset environment variable of the shell process
 */
bool unset_shell_env(const os_char *name);

/*! @brief Get value of an an environment variable */
bool get_shell_env(const os_char *var, unsigned int buffer_size, os_char *buffer);

/*! @brief Check if a variable environment exists */
bool has_shell_env(const os_char *var);

/*! @brief Get all current environment variable */
os_char *get_all_shell_env_display();

/*! @brief List all running processes */
bool enum_proc();

/*! @brief Get current time*/
bool get_time();

/*! @brief Get current date*/
bool get_date();

/*! @brief Stop a process use it ID*/
bool stop_proccess(int proc_id);

/*! @brief Launch shell executable and execute the file specified by `args.argv[0]` */
bool minibat(const struct args args);
