#include "Vector.h"

#include <string.h>	//	for memcpy()
#include <stdlib.h>	//	for malloc()
#include <assert.h>	//	for assert()
#include <limits.h>	//	for INT_MAX

//////////////////////////////////////////////////////////////////
//							   globals

static const int g_initCapacity = 10;
static const double g_factor = 1.5;


//////////////////////////////////////////////////////////////////
//								types

struct Vector
{
	void** buffer;
	int capacity;
	int size;
};

//////////////////////////////////////////////////////////////////
//								functions

static inline void checkPosRange(Vector vector, int pos)
{
	assert (vector != NULL);
	assert (pos >= 0 && pos < vector->size);
}

static inline void checkMaxCapacity(Vector vector)
{
	assert (vector->capacity <= (INT_MAX / g_factor) &&
			"meet max capacity!");
}

static inline bool vectorFullFilled(Vector vector)
{
	return vector->size == vector->capacity;
}

static inline void enlargeVectorBuffer(Vector vector)
{
	vector->capacity *= g_factor;
	void* newBuffer = malloc(sizeof(void*) * vector->capacity);
	assert (newBuffer && "run out of memory!");
	
	memcpy(newBuffer, vector->buffer, sizeof(void*) * (vector->size));
	free(vector->buffer);
	vector->buffer = newBuffer;
}

////////////////////////////////////////

Vector Vector_new(void)
{
	Vector vector = malloc(sizeof(*vector));
	assert (vector && "run out of memory!");

	vector->capacity = g_initCapacity;
	vector->buffer = malloc(sizeof(void*) * vector->capacity);
	assert (vector->buffer && "run out of memory!");

	vector->size = 0;
	return vector;
}

////////////////////////

Vector Vector_fromArray(void* array, int num)
{
    assert (array != NULL);
    assert (num >= 0);

    Vector vector = Vector_new();
    memcpy(vector->buffer, array, sizeof(void*) * num);
    vector->size = num;
    
    return vector;
}

////////////////////////

void Vector_free(Vector vector)
{
	assert (vector != NULL);

	free(vector->buffer);
	free(vector);
}

////////////////////////


void* Vector_get(Vector vector, int pos)
{
	assert (!Vector_empty(vector));
	checkPosRange(vector, pos);
	return vector->buffer[pos];
}

////////////////////////

void Vector_set(Vector vector, int pos, void* element)
{
	assert (!Vector_empty(vector));
	checkPosRange(vector, pos);
	vector->buffer[pos] = element;
}

////////////////////////

static inline void checkInsertPosRange(Vector vector, int beforePos)
{
	assert (vector != NULL);
	assert (beforePos >= 0 && beforePos <= vector->size);
}

static inline void moveElementsOneBackStep(Vector vector, int startPos)
{
	for (int i = vector->size; i > startPos; --i)
	{
		vector->buffer[i] = vector->buffer[i - 1];
	}
}

void Vector_insert(Vector vector, int beforePos, void* element)
{
	checkInsertPosRange(vector, beforePos);

	if (vectorFullFilled(vector))
	{
		checkMaxCapacity(vector);
		enlargeVectorBuffer(vector);
	}

	moveElementsOneBackStep(vector, beforePos);
	vector->buffer[beforePos] = element;
	vector->size++;
}

///////////////////////////

void* Vector_front(Vector vector)
{
	assert (!Vector_empty(vector));
	return Vector_get(vector, 0);
}

///////////////////////////

void* Vector_back(Vector vector)
{
	assert (!Vector_empty(vector));
	return Vector_get(vector, Vector_size(vector) - 1);
}

///////////////////////////

void Vector_pushback(Vector vector, void* element)
{
	assert (vector != NULL);
	
	if (vectorFullFilled(vector))
	{
		checkMaxCapacity(vector);
		enlargeVectorBuffer(vector);
	}

	vector->buffer[vector->size++] = element; 
}

////////////////////////////

void* Vector_popback(Vector vector)
{
	assert (!Vector_empty(vector));
	vector->size--;
	return vector->buffer[vector->size];
}

////////////////////////////

static inline void moveElementsOneForwardStep(Vector vector, int startPos)
{
	for (int i = startPos ; i < Vector_size(vector); ++i)
	{
		vector->buffer[i - 1] = vector->buffer[i];
	}
}

void Vector_erase(Vector vector, int pos)
{
	assert (!Vector_empty(vector));
	checkPosRange(vector, pos);

	moveElementsOneForwardStep(vector, pos + 1);
	vector->size--;
}

////////////////////////////

int Vector_find(Vector vector, void* element)
{
	assert (vector != NULL);
	for (int i = 0; i < vector->size; ++i)
	{
		if (vector->buffer[i] == element)
			return i;
	}
	return -1;
}

////////////////////////////

void Vector_clear(Vector vector)
{
	assert (vector != NULL);
	vector->size = 0;
}

////////////////////////////

bool Vector_empty(Vector vector)
{
	assert (vector != NULL);
	return vector->size == 0;
}

////////////////////////////

int Vector_size(Vector vector)
{
	assert (vector != NULL);
	return vector->size;
}

///////////////////////////

void* Vector_toArray(Vector vector)
{
	assert (vector != NULL);

	int sizeInBytes = sizeof(void*) * Vector_size(vector);
	void* newArray = malloc(sizeInBytes);
	assert (newArray != NULL && "no enough memory!");

	memcpy(newArray, vector->buffer, sizeInBytes);
	return newArray;
}

///////////////////////////////////////////////////////////////////////////////
//					 private functions used for unit test
void* getBufferField(Vector vector)
{
	return vector->buffer;
}

int getCapacityField(Vector vector)
{
	return vector->capacity;
}

int getSizeField(Vector vector)
{
	return vector->size;
}

int getInitCapacity(void)
{
	return g_initCapacity;
}

int getSizeFactor(void)
{
	return g_factor;
}
