#include "array.h"
#include <stdlib.h>
#include <string.h>

Array* array_create(int maxSize, int elementSize)
{
    Array* array = (Array*)malloc(sizeof(Array));
    array->size = 0;
    array->elementSize = elementSize;
    array->data = malloc(maxSize * elementSize);
    return array;
}

void array_add(Array* array, void* element)
{
    char* ptr = (char*)array->data;
    ptr += array->size * array->elementSize;
    memcpy(ptr, element, array->elementSize);
    array->size++;
}

void array_destroy(Array* array)
{
    free(array->data);
    free(array);
}

char* array_get(Array* array, int index)
{
    char* ptr = (char*)array->data;
    ptr += array->elementSize * index;
    return ptr;
}
