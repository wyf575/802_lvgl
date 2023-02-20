#ifndef USER_AT_H
#define USER_AT_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

int send_recv_AT_cmd(const char *cmd, char *need_ret, char *recv_buf, int buf_len);


#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !USER_AT_H