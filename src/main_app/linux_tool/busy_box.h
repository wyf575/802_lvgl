#ifndef BUSY_BOX_H
#define BUSY_BOX_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus


typedef enum 
{
    FILE_TYPE = 0,
    DIR_TYPE,
}checkType;

int call_system_cmd(const char *cmd_str);
int check_creat_file_dir(const char *file_dir, checkType type);
int get_system_output(char *cmd, char *output, int size);
int set_file_all_authority(const char *filePath);
int user_remove_file(const char *filename);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif //_BUSY_BOX_H
