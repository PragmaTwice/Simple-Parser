// Liu M , 2017.3.25


#pragma once
#ifndef _TRIE_HPP_
#define _TRIE_HPP_

#include <set>
#include <deque>
#include <algorithm>

 
/* 
It is not an out-and-out standard container(*).
But in most cases you can use it as a standard container.
(*) The standard container is a concept in the cpp stantard documentation.
The iterator in trie is not a standard iterator(#) at all.
(#) The standard iterator is a concept in the cpp stantard documentation.
*/
template <
	typename T,
	template < typename >
	class Compare = std::less,
	template < typename >
	class Allocator = std::allocator >
class trie
{

private /*class*/ :

	class node
	{

	public /*type*/ :

		using value_type          =  typename Allocator<T>::value_type;
		using reference             =  typename Allocator<T>::reference;
		using const_reference  =  typename Allocator<T>::const_reference;

		using group_type         =  std::set<node, Compare<node>, Allocator<node>>;

	public /*member*/ :

		T value;
		group_type *child_group;

	public /*function*/ :

		node(const T& _value, group_type *_child_group = nullptr) : value(_value), child_group(_child_group) {}

		bool operator<(const node& another) const { return value < another.value; }
		bool operator==(const node& another) const { return value == another.value; }

	};

public /*type*/:

	using value_type              =  typename node::value_type;
	using reference                 =  typename node::reference;
	using const_reference      =  typename node::const_reference;

	template<typename U> 
	using value_compare       =  Compare<U>;
	template<typename U>
	using allocator_type         =  Allocator<U>;

private /*type*/:

	using node_type = typename node;

	using group_type = typename node::group_type;

	using iterator_base = std::deque<typename group_type::iterator>;

public /*class*/ :

	class iterator
	{

	public /*type*/ :

		friend class trie;

	private /*member*/ :

		const group_type* root_ptr;

		iterator_base nodes;

	public /*function*/ :

		iterator(const group_type* _root_ptr) : root_ptr(_root_ptr) {}

		iterator(const iterator& another) : nodes(another.nodes), root_ptr(another.root_ptr) {}

		iterator(iterator&& another) : root_ptr(another.root_ptr)  { nodes.swap(another.nodes); }

		iterator& operator=(const iterator& another)
		{
			root_ptr = another.root_ptr;
			nodes = another.nodes;
			return *this;
		}

		iterator& operator=(iterator&& another)
		{
			swap(another);
			return *this;
		}

		void swap(iterator& another)
		{
			std::swap(root_ptr,another.root_ptr);
			nodes.swap(another.nodes);
		}

		bool operator==(const iterator& another) const
		{
			return root_ptr == another.root_ptr && nodes == another.nodes;
		}

		bool operator!=(const iterator& another) const
		{
			return root_ptr != another.root_ptr || nodes != another.nodes;
		}

		template < class Container >                // the template parameter 'Container' must adapt to the cpp concept  'container'
		friend Container trie_cast(const iterator& it)
		{
			Container con;
			con.resize(it.nodes.size());

			auto i_con = con.begin();
			auto i_it = it.nodes.cbegin();
			for (;i_it != it.nodes.cend();++i_con,++i_it)
			{
				*i_con = (**i_it).value;
			}

			return con;
		}

		//only to override pre-increment operator.

		iterator& operator++()
		{
			if (nodes.size() > 1)
			{
				if (++nodes.back() != nodes[nodes.size() - 2]->child_group->end())
				{
					while (nodes.back()->child_group != nullptr) nodes.push_back(nodes.back()->child_group->begin());
				}
				else
				{
					nodes.pop_back();
					++(*this);
				}
			}
			else
			{
				if (++nodes.back() != root_ptr->end())
				{
					while (nodes.back()->child_group != nullptr) nodes.push_back(nodes.back()->child_group->begin());
				}
				else
				{
					nodes.pop_back();
				}
			}

			return *this;
		}

	};

private /*member*/ :

	group_type root;

	Allocator<group_type> group_allocator;

	iterator _begin;

	const iterator _end;

public /*construct*/ :

	trie() : _begin(&root), _end(&root) {}

	trie(const trie& another) : _begin(&root),_end(&root) { *this = another; }

	trie(trie&& another) : _begin(&root), _end(&root) { swap(another); }

	template< class Container >
	trie(std::initializer_list<Container> list) :  _begin(&root), _end(&root)
	{
		for(const auto& elem : list) insert(elem);
	}

public /*destruct*/ :

	~trie() 
	{
		std::deque<group_type*> group_ptrs;

		std::for_each(root.cbegin(), root.cend(), [&group_ptrs](const node_type& elem) {
			if( elem.child_group != nullptr ) group_ptrs.push_back(elem.child_group); 
		});

		while (!group_ptrs.empty())
		{
			std::for_each(group_ptrs.front()->cbegin(), group_ptrs.front()->cend(), [&group_ptrs](const node_type& elem) {
				if (elem.child_group != nullptr ) group_ptrs.push_back(elem.child_group); 
			});
			group_ptrs.front()->~group_type();
			group_allocator.deallocate(group_ptrs.front(),1);
			group_ptrs.pop_front();
		}
	}

public /*operator*/ :

	trie& operator=(const trie& another)
	{
		clear();

		root = another.root;
		_begin = another._begin;
		std::deque<group_type**> group_ptrs;

		std::for_each(root.cbegin(), root.cend(), [&group_ptrs](const node_type& elem) {
			if (elem.child_group != nullptr) group_ptrs.push_back(&(const_cast<node_type&>(elem).child_group));
		});

		while (!group_ptrs.empty())
		{
			*group_ptrs.front() = new(group_allocator.allocate(1)) group_type(**group_ptrs.front());
			std::for_each((*group_ptrs.front())->cbegin(), (*group_ptrs.front())->cend(), [&group_ptrs](const node_type& elem) {
				if (elem.child_group != nullptr) group_ptrs.push_back(&(const_cast<node_type&>(elem).child_group));
			});
			group_ptrs.pop_front();
		}

		return *this;
	}

	trie& operator=(trie&& another)
	{
		swap(another);
	}

private /*function*/:

	inline static group_type * &ref_child_group(typename group_type::iterator& _iterator)
	{
		return ((node_type *)&(*_iterator))->child_group;
	}

	inline static void make_iterator(iterator& it,const iterator_base& con)
	{
		it.nodes = con;
	}

public /*function*/ :

	inline const iterator &begin()
	{
		// refresh _begin
		_begin.nodes.clear();
		_begin.nodes.push_back(root.begin());
		while (_begin.nodes.back()->child_group != nullptr) _begin.nodes.push_back(_begin.nodes.back()->child_group->begin());

		return _begin;
	}

	inline const iterator &end() const
	{
		return _end;
	}

	template < class Container >                // the template parameter 'Container' must adapt to the cpp concept  'container'
	iterator insert(const Container& container)
	{
		if (container.empty()) return _end;

		iterator ret_iterator(&root);
		Container::const_iterator _iterator = container.cbegin();

		std::pair<group_type::iterator,bool> status = root.insert(node_type(*_iterator));
		ret_iterator.nodes.push_back(status.first);
		
		if (_iterator == container.cend()) return ret_iterator;
		for(++_iterator; _iterator != container.cend();++_iterator)
		{
			if (status.first->child_group == nullptr)
			{
				ref_child_group(status.first) = new(group_allocator.allocate(1)) group_type();
			}

			status = status.first->child_group->insert(node_type(*_iterator));
			ret_iterator.nodes.push_back(status.first);
		}
		return ret_iterator;
	}

	inline bool empty() const
	{
		return root.empty();
	}

	inline void swap(trie& another)
	{
		root.swap(another.root);
		std::swap(_begin, another._begin);
	}

	inline void clear()
	{
		trie().swap(*this);
	}

	template < class Container >                // the template parameter 'Container' must adapt to the cpp concept  'container'
	iterator find(const Container& container) const
	{
		if (container.empty()) return _end;
		if (empty()) return _end;

		iterator ret_iterator(&root);
		Container::const_iterator _iterator = container.cbegin();
		group_type::iterator found = root.find(node_type(*_iterator));
		ret_iterator.nodes.push_back(found);

		if (found == root.end()) return _end;
		if (_iterator == container.cend()) return ret_iterator;

		for (++_iterator; _iterator != container.cend(); ++_iterator)
		{
			if (found->child_group == nullptr) return _end;
			group_type::iterator tmp_iterator = found;
			found = tmp_iterator->child_group->find(node_type(*_iterator));
			ret_iterator.nodes.push_back(found);
			if (found == tmp_iterator->child_group->end()) return _end;
		}

		return ret_iterator;
	}

	template < class Container >                // the template parameter 'Container' must adapt to the cpp concept  'container'
	bool erase(const Container& container, typename Container::value_type* SFINAE = nullptr) //using "Substitution Failure Is Not an Error" to substitute non-template function.
	{ 
		return erase(find(container));
	}

	bool erase(iterator& it)
	{
		if (it.nodes.empty()) return false;

		while (it.nodes.size() > 1)
		{
			group_type::iterator& now_it = it.nodes.back();
			if (now_it->child_group == nullptr )
			{
				
				(it.nodes[it.nodes.size() - 2])->child_group->erase(now_it);
				it.nodes.pop_back();
			}
			else if (now_it->child_group->size() == 0)
			{
				now_it->child_group->~group_type();
				group_allocator.deallocate(now_it->child_group, 1);
				(it.nodes[it.nodes.size() - 2])->child_group->erase(now_it);
				it.nodes.pop_back();
			}
			else
			{
				it = _end;
				return true;
			}
		}
		group_type::iterator& last_it = it.nodes.back();
		if (last_it->child_group == nullptr)
		{
			root.erase(last_it);
			it.nodes.pop_back();
		}
		else if (last_it->child_group->size() == 0)
		{
			last_it->child_group->~group_type();
			group_allocator.deallocate(last_it->child_group, 1);
			root.erase(last_it);
			it.nodes.pop_back();
		}

		return true;
	}
};

#endif // ndef _TRIE_HPP_



//Copyright 2017 Liu Mingyang.
//This source file is part of SimpleParser.

//SimpleParser is free software : you can redistribute it and / or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//SimpleParser is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with SimpleParser.If not, see <http://www.gnu.org/licenses/>.