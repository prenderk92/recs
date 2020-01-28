#pragma once

/*------------------------------------------------------------------------------
*-----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
* SparseSet is a data structure which contains two internals arrays; the direct
* array and sparse array give the data structure two methods to access the
* underlying data. The arrays are maintained such that an element in the sparse  
* array at index N points to the Entity in the dense array with ID = N. By doing
* this, we can search and delete an Entity in the data structure in O(1) time 
* and keep the underlying data in contiguous memory. Then, by exposing the 
* underlying data we can iterate over it in a cache-friendly manner.
*
* This data structure comes at the cost of the additional memory required to
* maintain the sparse array. In order to encapsulate every possible Entity ID, 
* we would need to have a vector with a capacity = max(Entity ID). If we wanted
* 2^24 unique Entity identifiers, we would need to store a vector of 2^24
* integers for our sparse array. If we only store 10,000 items in our SparseSet,
* it seems like a waste of 16MB of memory in sparse array data. This being said,
* we can mitigate the amount of resources we waste by storing "Pages" of data
* for our sparse array. If a Page is unused, then it will waste no memory. As
* a freebie feature we also avoid having huge blocks of contiguous memory that
* may be resized and copied.
*
* Details on the Paging method can be seen seen over the getPage method.
*
* Source: https://www.geeksforgeeks.org/sparse-set/
*-----------------------------------------------------------------------------*/


namespace recs{

/*
*	TODO:
*		Build Iterator class for SparseSet
*		Define page system for
*/
// TODO:
//	Build Iterator for SparseSet
//  Define

template<typename Entity>
class SparseSet {

	// Define random-access 
};

}