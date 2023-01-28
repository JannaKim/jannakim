#pragma once
#pragma once
#include <iostream>

template <class RandomAccessIterator>
void sort( RandomAccessIterator first, RandomAccessIterator last )
{
	std::cout << "sort\n";

	RandomAccessIterator it = first;
	RandomAccessIterator prev_last = it;
	for ( ; it != last; ++it ) {
		prev_last = it;
	}
	it = prev_last;
	for ( it; it != first; --it ) {
		for ( RandomAccessIterator it2 = first; it2 != last; ++it2 ) {
			RandomAccessIterator left = it2;
			++it2;
			RandomAccessIterator right = it2;
			if ( right != last && right < left ) {
				auto temp = *left;
				*left = *right; // left = *right; 시 operator=( const value_type& val ) 동작, 아니면 이 연산자 오버로딩은 한 번도 쓰이지 않음 
				*right = temp;
			}
			--it2;
		}
	}
}





template <class RandomAccessIterator, class Compare>  
void sort( RandomAccessIterator first, RandomAccessIterator last, Compare comp )
{

	std::cout << "sort with cmp \n";

	RandomAccessIterator it = first;
	RandomAccessIterator prev_last = --last;
	++last;
	it = prev_last;
	--first;
	for ( it; it != first; --it ) {
		for ( RandomAccessIterator it2 = first; it2 != last; ++it2 ) {
			RandomAccessIterator left = it2;
			++it2;
			RandomAccessIterator right = it2;
			if ( comp( *left, *right ) ) {
				auto temp = *left;
				*left = *right;
				*right = temp;
			}
			--it2;
		}
	}

}

