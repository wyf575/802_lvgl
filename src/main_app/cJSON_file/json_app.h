#ifndef JSON_APP_H
#define JSON_APP_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus


#include "cJSON.h"
typedef enum
{
	J_String = 0,
	J_Int,
	J_Long,
	J_Double,
	J_Bool,
	J_Item,
	J_Array,
} add_type;

int create_json_table(cJSON *json_table, add_type type, ...);
int parse_json_table(cJSON *json_table, add_type type, char *key,void *value);


#ifdef __cplusplus
}
#endif // __cplusplus
#endif/*JSON_APP_H*/





