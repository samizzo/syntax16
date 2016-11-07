#ifndef _array_h
#define _array_h

typedef struct Array_tag
{
    int size;
    int elementSize;
    void* data;
} Array;

Array* array_create(int maxSize, int elementSize);
void array_destroy(Array* array);
void array_add(Array* array, void* element);
char* array_get(Array* array, int index);

#endif
