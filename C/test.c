#include "Vector.h"

#include <limits.h>	//	for INT_MAX
#include <stdlib.h>	//	for NULL
#include <assert.h>	//	for assert()

///////////////////////////////////////////////////////////////
//				test helper funtions in Vector.c
void* getBufferField(Vector vector);
int getCapacityField(Vector vector);
int getSizeField(Vector vector);
int getInitCapacity(void);
int getSizeFactor(void);

////////////////////

void test_VectorNew_ByDefault_FieldsInitializedCorrectly(void)
{
	Vector vector = Vector_new();

	assert (getCapacityField(vector) == getInitCapacity());
	assert (getSizeField(vector) == 0);

	Vector_free(vector);
}

///////////////////

char* g_chars[] = { "9", "8", "7", "6"  "5", "4", "3", "2", "1", "0" };

//////////////////

void test_VectorFromArray_TwoElements_CopyTwo(void)
{    
    Vector vector = Vector_fromArray(&g_chars, 2);

    assert (Vector_size(vector) == 2);
    assert (Vector_get(vector, 0) == g_chars[0]);
    assert (Vector_get(vector, 1) == g_chars[1]);
}

void test_VectorFromArray_ZeroElements_CopyZero(void)
{
    char* chars[2] = { "1", "0" };
    
    Vector vector = Vector_fromArray(&chars, 0);

    assert (Vector_size(vector) == 0);
}

////////////////////

void test_VectorPushback_CapacityNotFull_AddPushed(void)
{
	Vector vector = Vector_new();
	int sizeBeforeInsert = Vector_size(vector);

	void* valuePushed = (void*)1;
	Vector_pushback(vector, valuePushed);
	int sizeAfterInsert = Vector_size(vector);

	void* valueGot = Vector_get(vector, 0);
	assert (valuePushed == valueGot);
	assert (sizeAfterInsert - sizeBeforeInsert == 1);

	Vector_free(vector);
}

void test_VectorPushBack_CapacityNotFull_FormersNotChanged(void)
{
	Vector vector = Vector_new();
	void* firstPushed = (void*)1;
	void* secondPushed = (void*)2;
	Vector_pushback(vector, firstPushed);
	Vector_pushback(vector, secondPushed);

	Vector_pushback(vector, (void*)3);

	void* firstGot = Vector_get(vector, 0);
	void* secondGot = Vector_get(vector, 1);
	assert (firstPushed == firstGot);
	assert (secondPushed == secondGot);
}

void test_VectorPushback_CapacityFull_SizeEnlarged(void)
{
	Vector vector = Vector_new();
	for (int i = 0; i < getInitCapacity(); ++i)
	{
		Vector_pushback(vector, (void*)1);
	}

	Vector_pushback(vector, (void*)1);
	
	int currentCapacity = getCapacityField(vector);
	int expectedCapaticy = getInitCapacity() * getSizeFactor();
	assert (currentCapacity = expectedCapaticy);
}

void test_VectorPushback_CapacityFull_ThisOnePushed(void)
{
	Vector vector = Vector_new();
	for (int i = 0; i < getInitCapacity(); ++i)
	{
		Vector_pushback(vector, (void*)1);
	}
	int sizeBeforeInsert = Vector_size(vector);

	void* valuePushed = (void*)12;
	Vector_pushback(vector, valuePushed);
	int sizeAfterInsert = Vector_size(vector);

	assert (sizeAfterInsert - sizeBeforeInsert == 1);
	void* valueGot = Vector_get(vector, getInitCapacity());
	assert (valuePushed == valueGot);
}

void test_VectorPushback_CapacityFull_FormersNotChanged(void)
{
   	Vector vector = Vector_new();
	for (int i = 0; i < getInitCapacity(); ++i)
	{
		Vector_pushback(vector, g_chars[i]);
	}

	Vector_pushback(vector, (void*)12);

	for (int i = 0; i < getInitCapacity(); ++i)
	{
	    assert (g_chars[i] == Vector_get(vector, i));
	}
}

////////////////////

void test_VectorSet_ByDefault_SetWhatPassed(void)
{
	Vector vector = Vector_new();
	Vector_pushback(vector, (void*)1);

	void* valueSet = (void*)2;
	Vector_set(vector, 0, valueSet);

	void* valueGot = Vector_get(vector, 0);
	assert (valueSet == valueGot);
}

////////////////////

void test_VectorInsert_CapacityNotFull_OneInserted(void)
{
	Vector vector_one = Vector_new();
	Vector vector_two = Vector_new();
	for (int i = 0; i < 3; ++i)
		Vector_pushback(vector_two, g_chars[i]);

	void* valueInserted_one = g_chars[3];
	Vector_insert(vector_one, 0, valueInserted_one);
	void* valueInserted_two = g_chars[4];
	Vector_insert(vector_two, 1, valueInserted_two);

	void* valueGot_one = Vector_get(vector_one, 0);
	assert (valueInserted_one == valueGot_one);
	void* valueGot_two = Vector_get(vector_two, 1);
	assert (valueInserted_two == valueGot_two);
}

void test_VectorInsert_CapacityNotFull_ElementsBeforeNotMoved(void)
{
	Vector vector = Vector_new();
	for (int i = 0; i < 3; ++i)
		Vector_pushback(vector, g_chars[i]);

	Vector_insert(vector, 2, g_chars[3]);

	for (int i = 0; i < 2; ++i)
		assert (Vector_get(vector, i) == g_chars[i]);
}

void test_VectorInsert_CapacityNotFull_ElementsAfterMovedOneStep(void)
{
	Vector vector = Vector_new();
	for (int i = 0; i < 3; ++i)
		Vector_pushback(vector, g_chars[i]);
	int sizeBeforeInsert = Vector_size(vector);

	Vector_insert(vector, 1, (void*)4);
	int sizeAfterInsert = Vector_size(vector);

	assert (sizeAfterInsert - sizeBeforeInsert == 1);
	for (int i = 2; i < 4; ++i)
		assert (Vector_get(vector, i) == g_chars[i - 1]);
}

void test_VectorInsert_CapacityFull_OneInserted(void)
{
	Vector vector = Vector_new();
	for (int i = 0; i < getInitCapacity(); ++i)
		Vector_pushback(vector, g_chars[i]);

	void* valueInserted = g_chars[1];
	Vector_insert(vector, 2, valueInserted);

	assert (Vector_get(vector, 2) == valueInserted);
}

void test_VectorInsert_CapacityFull_ElementsBeforeNotMoved(void)
{
	Vector vector = Vector_new();
	for (int i = 0; i < getInitCapacity(); ++i)
	    Vector_pushback(vector, g_chars[i]);

	int insertPos = 5;
	Vector_insert(vector, insertPos, (void*)8989);

	for (int i = 0; i < insertPos; ++i)
		assert (Vector_get(vector, i) == g_chars[i]);
}

void test_VectorInsert_CapacityFull_ElementsAfterMovedOneStep(void)
{
	Vector vector = Vector_new();
	for (int i = 0; i < getInitCapacity(); ++i)
		Vector_pushback(vector, g_chars[i]);

	int insertPos = 5;
	Vector_insert(vector, insertPos, (void*)13233);

	for (int i = insertPos + 1; i < Vector_size(vector); ++i)
		assert (Vector_get(vector, i) == g_chars[i - 1]);
}

////////////////////

void test_VectorFront_ByDefault_GetFirstPushed(void)
{
	Vector vector = Vector_new();
	void* firstPushed = (void*)1;
	Vector_pushback(vector, firstPushed);

	void* frontGot = Vector_front(vector);
	assert (firstPushed == frontGot);


	void* secondPushed = (void*)2;
	Vector_pushback(vector, secondPushed);

	frontGot = Vector_front(vector);
	assert (firstPushed == frontGot);
}

////////////////////

void test_VectorBack_ByDefault_GetLastPushed(void)
{
	Vector vector = Vector_new();
	void* firstPushed = (void*)1;
	Vector_pushback(vector, firstPushed);

	void* backGot = Vector_back(vector);
	assert (firstPushed == backGot);


	void* secondPushed = (void*)2;
	Vector_pushback(vector, secondPushed);

	backGot = Vector_back(vector);
	assert (secondPushed == backGot);
}

////////////////////

void test_VectorPopback_ByDefault_PopLast(void)
{
	Vector vector = Vector_new();
	void* valuePushed = (void*)1;
	Vector_pushback(vector, valuePushed);

	void* valuePoped = Vector_popback(vector);

	assert (valuePoped == valuePushed);
}

void test_VectorPopback_AfterPoped_SizeDecrement(void)
{
	Vector vector = Vector_new();
	void* valuePushed = (void*)1;
	Vector_pushback(vector, valuePushed);

	int sizeBeforePop = Vector_size(vector);
	Vector_popback(vector);

	int sizeAfterPop = Vector_size(vector);
	assert (sizeAfterPop == sizeBeforePop - 1);
}

////////////////////

void test_VectorErase_EraseOne_SizeDrecrementOne(int pos)
{
	Vector vector = Vector_new();
	void* firstValue = (void*)1;
	void* secondValue = (void*)2;
	void* thridValue = (void*)3;
	Vector_pushback(vector, firstValue);
	Vector_pushback(vector, secondValue);
	Vector_pushback(vector, thridValue);
	int sizeBeforeErase = Vector_size(vector);

	Vector_erase(vector, pos);

	int sizeAfterErase = Vector_size(vector);
	assert (sizeBeforeErase - sizeAfterErase == 1);
}

////////

void test_VectorErase_EraseFirst_SizeDecrementOne(void)
{
	int erasePos = 0;
	test_VectorErase_EraseOne_SizeDrecrementOne(erasePos);
}

void test_VectorErase_EraseLast_SizeDecrementOne(void)
{
	int erasePos = 2;
	test_VectorErase_EraseOne_SizeDrecrementOne(erasePos);
}

void test_VectorErase_EraseFirst_MoveLaterElementsOneForward(void)
{
	Vector vector = Vector_new();
	void* firstValue = (void*)1;
	void* secondValue = (void*)2;
	void* thridValue = (void*)3;
	Vector_pushback(vector, firstValue);
	Vector_pushback(vector, secondValue);
	Vector_pushback(vector, thridValue);

	Vector_erase(vector, 0);

	assert (Vector_get(vector, 0) == secondValue);
	assert (Vector_get(vector, 1) == thridValue);
}

void test_VectorErase_EraseLast_MoveLaterElementsOneForward(void)
{
	Vector vector = Vector_new();
	void* firstValue = (void*)1;
	void* secondValue = (void*)2;
	void* thridValue = (void*)3;
	Vector_pushback(vector, firstValue);
	Vector_pushback(vector, secondValue);
	Vector_pushback(vector, thridValue);

	Vector_erase(vector, 2);

	assert (Vector_get(vector, 0) == firstValue);
	assert (Vector_get(vector, 1) == secondValue);
}

void test_VectorErase_EraseMiddle_MoveLaterElementsOneForward(void)
{
	Vector vector = Vector_new();
	void* firstValue = (void*)1;
	void* secondValue = (void*)2;
	void* thridValue = (void*)3;
	Vector_pushback(vector, firstValue);
	Vector_pushback(vector, secondValue);
	Vector_pushback(vector, thridValue);

	Vector_erase(vector, 1);

	assert (Vector_get(vector, 0) == firstValue);
	assert (Vector_get(vector, 1) == thridValue);
}

///////////////////

void test_VectorFind_FindOne_ReturnPos(void)
{
   	Vector vector = Vector_new();
	void* firstValue = (void*)1;
	void* secondValue = (void*)2;
	Vector_pushback(vector, firstValue);
	Vector_pushback(vector, secondValue);

 	int pos = Vector_find(vector, secondValue);

	assert (pos == 1);
}

void test_VectorFind_NotFindOne_ReturnMinusOne(void)
{
   	Vector vector = Vector_new();
	void* firstValue = (void*)1;
	void* secondValue = (void*)2;
	Vector_pushback(vector, firstValue);
	Vector_pushback(vector, secondValue);

 	int pos = Vector_find(vector, (void*)999);

	assert (pos == -1);
}

////////////////////

void test_VectorClear_AfterClear_NoElements(void)
{
	Vector vector = Vector_new();
	void* firstValue = (void*)1;
	Vector_pushback(vector, firstValue);

	Vector_clear(vector);

	assert (Vector_size(vector) == 0);
}

////////////////////

void test_VectorToArray_ByDefault_SameSizeAndElements(void)
{
	Vector vector = Vector_new();
	void* firstValue = (void*)1;
	void* secondValue = (void*)2;
	Vector_pushback(vector, firstValue);
	Vector_pushback(vector, secondValue);

	void** array = Vector_toArray(vector);

	int arraySize = Vector_size(vector);
	assert (arraySize == 2);
	assert (array[0] == firstValue);
	assert (array[1] == secondValue);
}

////////////////////

int main()
{
	test_VectorNew_ByDefault_FieldsInitializedCorrectly();

	test_VectorFromArray_TwoElements_CopyTwo();
	test_VectorFromArray_ZeroElements_CopyZero();

	test_VectorPushback_CapacityNotFull_AddPushed();
	test_VectorPushBack_CapacityNotFull_FormersNotChanged();
	test_VectorPushback_CapacityFull_SizeEnlarged();
	test_VectorPushback_CapacityFull_ThisOnePushed();
	test_VectorPushback_CapacityFull_FormersNotChanged();

	test_VectorSet_ByDefault_SetWhatPassed();

	test_VectorInsert_CapacityNotFull_OneInserted();
	test_VectorInsert_CapacityNotFull_ElementsBeforeNotMoved();
	test_VectorInsert_CapacityNotFull_ElementsAfterMovedOneStep();
	test_VectorInsert_CapacityFull_OneInserted();
	test_VectorInsert_CapacityFull_ElementsBeforeNotMoved();
	test_VectorInsert_CapacityFull_ElementsAfterMovedOneStep();

	test_VectorFront_ByDefault_GetFirstPushed();

	test_VectorBack_ByDefault_GetLastPushed();

	test_VectorPopback_ByDefault_PopLast();
	test_VectorPopback_AfterPoped_SizeDecrement();

	test_VectorErase_EraseFirst_SizeDecrementOne();
	test_VectorErase_EraseLast_SizeDecrementOne();
	test_VectorErase_EraseFirst_MoveLaterElementsOneForward();
	test_VectorErase_EraseLast_MoveLaterElementsOneForward();
	test_VectorErase_EraseMiddle_MoveLaterElementsOneForward();

	test_VectorFind_FindOne_ReturnPos();
	test_VectorFind_NotFindOne_ReturnMinusOne();

	test_VectorClear_AfterClear_NoElements();

	test_VectorToArray_ByDefault_SameSizeAndElements();

	return 0;
}
