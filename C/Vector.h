    /**************************************************************************
 *  @file
 *  @author lgxZJ@outlook.com
 *  @date   11/29/2016
 *
 *  @brief  A simple vector interface written in C.
 **************************************************************************/

#ifndef LGXZJ_VECTOR_H
#define LGXZJ_VECTOR_H

#include <stdbool.h>

typedef struct Vector* Vector;

/**
 *	@brief		Create a new vector.
 *	@return		A new vecotr.
 *	@remark		If no enough memory, it throws a runtime error.
 */
Vector Vector_new(void);

/**
 *	@brief		Create a new vector from an array.
 *	@param[in]	array   An array address.
 *	@param[in]	num	Number of elements in the given array.
 *	@return		A new vector.
 *
 *	@remark		It's a checked runtime error to pass in a NULL array address or a
 *			negative element number. If no enough memory, it throws a runtime
 *			error. Sometimes, there will be a need for users to do explicit type
 *			cast since the interface uses --void*-- as a general element type. 
 */
Vector Vector_fromArray(void* array, int num);

/**
 *	@brief		Free the formerly created vectors.
 *	@param[in]	vector	A vector created by Vector_new().
 *
 *	@remark		It is a checked runtime error to pass in a NULL vector. After
 *			freed, the passed in vector cannot be used anymore.
 */
void   Vector_free(Vector vector);

/**
 *	@brief		Get one element.
 *	@param[in]	vector	A vector where to get elements.
 *	@param[in]	pos	The element position.
 *	@return		The specified element.
 *
 *	@remark		It is a checked runtime error to pass in a NULL vector, an empty
 *			vector, a negative position or a position one pass the end. 
 *			The position here begins from 0 as the first element, and ends
 *			with Vector_size() - 1 as the last. So, it's actually an index.
 */
void*  Vector_get(Vector vector, int pos);

/**
 *	@brief		Set one element to the specified value.
 *	@param[in]	vector  A vector where to set elements.
 *	@param[in]	pos	The element position.
 *	@param[in]	element	The new element value.
 *
 *	@sa		Vector_get()
 */
void   Vector_set(Vector vector, int pos, void* element);

/**
 *	@brief		Insert one element before the given position.
 *	@param[in]	vector  A vector where to insert elements.
 *	@param[in]	pos	The element position.
 *	@param[in]	element	The new element value.
 *
 *	@remark		Passing a special position----one pass the end, is permitted here.
 *		        To insert the new element, elements start from pos is moved one
 *			step back while others remain non-changed.
 *	@sa		Vector_get()
 */
void   Vector_insert(Vector vector, int pos, void* element);

/**
 *	@brief		Get the first element pushed in.
 *	@param[in]	vector	A vector where to get the first element.
 *	@return		The first element.
 *
 *	@remark		It's a checked runtime error to pass in a NULL vector or an empty
 *			vector. Users need to cast --void*-- into their real type.
 */
void*  Vector_front(Vector vector);

/**
 *	@brief		Get the last element pushed in.
 *	@param[in]	vector	A vector where to get the last element.
 *	@return		void*	The last element.
 *
 *	@remark		It's a checked runtime error to pass in a NULL vector or an empty
 *			vector. Users need to cast --void*-- into their real type.
 */
void*  Vector_back(Vector vector);

/**
 *	@brief		Push one element into the given vector.
 *	@param[in]	vector	A vector where to push elements.
 *	@param[in]	element	The element to be pushed.
 *
 *	@remark		It's a checked runtime error to pass in a NULL vector.
 */
void   Vector_pushback(Vector vector, void* element);

/**
 *	@brief		Pop the last pushed element.
 *	@param[in]	vector	A vector where to pop elements.
 *	@return		void*	The last pushed element.
 *
 *	@remark		It's a checked runtime error to pass in a NULL vector or an empty
 *			vector.
 */
void*  Vector_popback(Vector vector);

/**
 *	@brief		Erase the specified element in vector.
 *	@param[in]	vector	A vector where to erase elements.
 *	@param[in]	pos	The position of the element.
 *
 *	@remark		It's a checked runtime error to pass in a NULL vector or an empty
 *			vector.
 *	@sa		Vector_get()
 */
void   Vector_erase(Vector vector, int pos);

/**
 *	@brief		Try to find the given element.
 *	@param[in]	vector	A vector where to find elements.
 *	@param[in]	element	The element to find.
 *	@return		If found, it returns the element position;
 *			If not, it returns -1.
 *
 *	@remark		It's a checked runtime error to pass in a NULL vector.
 *	@sa		Vector_get()
 */
int    Vector_find(Vector vector, void* element);

/**
 *	@brief	        Clear the vector.
 *	@param[in]	vector	A vector to be cleared.
 *	@remark		It's a checked runtime error to pass in a NULL vector.
 */
void   Vector_clear(Vector vector);

/**
 *	@brief		Check if the given vector is empty.
 *	@param[in]	vector	A vector to be checked.
 *	@return		A bool result.
 *
 *	@remark		It's a checked runtime error to pass in a NULL vector.
 */
bool   Vector_empty(Vector vector);

/**
 *	@brief		Get the element size of the given vector.
 *	@param[in]	vector	A vector.
 *	@return	        The element size.
 *
 *	@remark		It's a checked runtime error to pass in a NULL vector. Although
 *		        the return type is int, the return value is always non-negative.
 */
int    Vector_size(Vector vector);

/**
 *	@brief		Convert the elements in the given vector to an array.
 *	@param[in]	vector	A vector whose elements are to be converted.
 *	@return		An array containing the given vector's elements.
 *
 *	@remark		It's a checked runtime error to pass in a NULL vector. If no enough
 *			memory, a runtime error will be thrown. And users need to call free()
 *			after finishing using this array.
 */
void* Vector_toArray(Vector vector);

#endif
