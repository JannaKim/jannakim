#pragma once
#pragma once
#include <iostream>

template <class RandomAccessIterator>
void sort( RandomAccessIterator first, RandomAccessIterator last )
{
	std::cout << "sort\n";

	auto it = first;
	auto prev_last = it;
	for ( ; it != last; ++it ) {
		prev_last = it;
	}
	it = prev_last;
	for ( it; it != first; --it ) {
		for ( auto it2 = first; it2 != last; ++it2 ) {
			auto left = it2;
			++it2;
			auto right = it2;
			if ( right != last && right < left ) {
				auto temp = *left;
				*left = *right; // left = *right; 시 operator=( const value_type& val ) 동작, 아니면 이 연산자 오버로딩은 한 번도 쓰이지 않음 
				*right = temp;
			}
			--it2;
		}
	}
}





template <class RandomAccessIterator, class Compare>  void
sort( RandomAccessIterator first, RandomAccessIterator last, Compare comp );

