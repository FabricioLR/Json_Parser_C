#ifndef JSON_H_
#define JSON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum TYPE {
	PRIMITIVE = 0,
	OBJECT = 1,
	ARRAY = 2
};
typedef enum TYPE TYPE;

struct JSON {
	char *key;
	int count;
	TYPE type;
	struct JSON *values;
	struct JSON *prev;
};
typedef struct JSON JSON;

struct Stack {
	TYPE type;
	struct Stack *next;
};
typedef struct Stack Stack;

void stack_push(Stack *stack_, TYPE type){
	assert(stack_ != NULL);
	Stack *stack = stack_;

	while (stack->next != NULL){
		stack = stack->next;	
	}

	stack->type = type;
	stack->next = (Stack *)malloc(sizeof(Stack));
	assert(stack->next);
	stack->next->next = NULL;
}

TYPE stack_pop(Stack *stack_){
	assert(stack_ != NULL);
	Stack *stack = stack_;

	TYPE type;

	if (stack->next->next == NULL) {
		type = stack->type;
		stack->type = -1;
		return type;
	}

	while (stack->next->next->next != NULL){
		stack = stack->next;	
	}

	Stack *ref = stack->next->next;
	type = stack->next->type;

	stack->next = ref;

	return type;
}

char *substring(char *string, int start, int end){
	char *buffer = (char *)malloc(end - start);
	assert(buffer != NULL);

	int j = 0;
	for (int i = start; i < end; i++){
		buffer[j] = string[i];
		j++;
	}

	buffer[j] = '\0';

	return buffer;
}

int last_index = -1;
void add_key(JSON *json_, Stack *stack, char *key, int index){
	assert(json_ != NULL);
	JSON *json = json_;

	TYPE type = stack_pop(stack);

	if (type == OBJECT){
		if (last_index == index){
			index++;
		}

		TYPE type2 = stack_pop(stack);
		if (type2 == ARRAY) index--;

		for (int i = 0; i < index; i++){
			if (json->count == 0){
				json = &(json->values[json->count]);
			} else {
				if (i == index - 1){
					json->values = (JSON *)realloc(json->values, sizeof(JSON) * (json->count + 1));
					JSON *json_ = &(json->values[json->count]);

					json_->key = NULL;
					json_->count = 0;
					json_->type = PRIMITIVE;
					json_->values = NULL;
					json_->prev = json;

					json = json_;	
				} else {
					json = &(json->values[json->count - 1]);
				}
			}
		}

		json->key = key;
		json->type = PRIMITIVE;
		json->prev->count++;
		json->prev->type = OBJECT;

		json->values = (JSON *)malloc(sizeof(JSON) * 1);
		assert(json->values != NULL);

		json->values->key = NULL;
		json->values->count = 0;
		json->values->type = PRIMITIVE;
		json->values->values = NULL;
		json->values->prev = json;

		if (type2 == ARRAY) index++;
		stack_push(stack, type2);

		last_index = index;
	} else if (type == ARRAY){
		for (int i = 0; i < index; i++){
			if (json->count == 0){
				json = &(json->values[json->count]);
			} else {
				if (i == index - 1){
					json->values = (JSON *)realloc(json->values, sizeof(JSON) * (json->count + 1));
					JSON *json_ = &(json->values[json->count]);

					json_->key = NULL;
					json_->count = 0;
					json_->values = NULL;
					json_->prev = json;

					json = json_;	
				} else {
					json = &(json->values[json->count - 1]);
				}
			}
		}

		json->key = key;
		json->type = PRIMITIVE;
		json->prev->count++;
		json->prev->type = ARRAY;

		json->values = (JSON *)malloc(sizeof(JSON) * 1);
		assert(json->values != NULL);

		json->values->key = NULL;
		json->values->count = 0;
		json->values->type = PRIMITIVE;
		json->values->values = NULL;
		json->values->prev = json;
	}

	stack_push(stack, type);
}

JSON *json_parse(char *string){
	int length = strlen(string);

	JSON *json = (JSON *)malloc(sizeof(JSON));
	assert(json != NULL);

	json->key = NULL;
	json->count = 0;
	json->values = NULL;
	json->prev = NULL;

	json->values = (JSON *)malloc(sizeof(JSON) * 1);
	assert(json->values != NULL);

	json->values->key = NULL;
	json->values->count = 0;
	json->values->values = NULL;
	json->values->prev = json;

	Stack *stack = (Stack *)malloc(sizeof(Stack));
	assert(stack != NULL);

	int key_start_offset = 0, inside_string = 0, index = 0;
	for (int i = 0; i < length; i++){
		if (string[i] == '"'){
			if (inside_string == 1){
				inside_string = 0;
				if (key_start_offset != 0) {
					char *key = substring(string, key_start_offset, i);
					add_key(json, stack, key, index);
					key_start_offset = 0;
				}
			} else {
				inside_string = 1;
				continue;
			}
			continue;
		}

		if (string[i] == '{' && inside_string == 0){
			index++;
			//stack_push(stack, OBJECT);
			if (key_start_offset != 0) {
				char *key = substring(string, key_start_offset, i);
				add_key(json, stack, key, index);
				key_start_offset = 0;
			}
			stack_push(stack, OBJECT);
			continue;
		}
		if (string[i] == '}' && inside_string == 0){
			if (key_start_offset != 0) {
				char *key = substring(string, key_start_offset, i);
				add_key(json, stack, key, index);
				key_start_offset = 0;
			}
			stack_pop(stack);
			index--;
			continue;
		}
		if (string[i] == '[' && inside_string == 0){
			index++;
			//stack_push(stack, ARRAY);
			if (key_start_offset != 0) {
				char *key = substring(string, key_start_offset, i);
				add_key(json, stack, key, index);
				key_start_offset = 0;
			}
			stack_push(stack, ARRAY);
			continue;
		}
		if (string[i] == ']' && inside_string == 0){
			if (key_start_offset != 0) {
				char *key = substring(string, key_start_offset, i);
				add_key(json, stack, key, index);
				key_start_offset = 0;
			}
			stack_pop(stack);
			index--;
			continue;
		}
		if (string[i] == ',' && inside_string == 0){
			if (key_start_offset != 0) {
				char *key = substring(string, key_start_offset, i);
				add_key(json, stack, key, index);
				key_start_offset = 0;
			}
			continue;
		}
		if (string[i] == ':' && inside_string == 0){
			if (key_start_offset != 0) {
				char *key = substring(string, key_start_offset, i);
				add_key(json, stack, key, index);
				key_start_offset = 0;
			}
			continue;
		}

		if (key_start_offset != 0) continue;

		key_start_offset = i;
	}

	return json;
}

void print_key(JSON *json, int index){
	if (json->count == 0) return;
	for (int i = 0; i < json->count; i++){
		if (json->values[i].type == OBJECT){
			printf("%*s%s (OBJECT)\n", 5 * index, "", json->values[i].key);
		} else if (json->values[i].type == ARRAY){
			printf("%*s%s (ARRAY)\n", 5 * index, "", json->values[i].key);
		} else {
			printf("%*s%s (PRIMITIVE)\n", 5 * index, "", json->values[i].key);
		}
		print_key(&(json->values[i]), index + 1);
	}
}

void print_json(JSON *json){
	if (json->count == 0) return;
	printf("JSON: {\n");
	print_key(json, 1);
	printf("}\n");
}

JSON *json_get_item(JSON *json, char *key){
	for (int i = 0; i < json->count; i++){
		if (strcmp(json->values[i].key, key) == 0){
			return &(json->values[i]);
		}
	}
	return NULL;
}

char *json_get_value(JSON *json){
	if (json->values[0].type == PRIMITIVE) return json->values[0].key;
	return "";
}

#endif // JSON_H_