/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skim <skim@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 14:02:54 by skim              #+#    #+#             */
/*   Updated: 2021/12/25 16:45:05 by skim             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SET_HPP
# define SET_HPP

#include <iostream>
#include <memory>
#include "../utils/utils.hpp"
#include "../tree_utils/red_black.hpp"
#include "../tree_utils/pair.hpp"
#include "set_iterator.hpp"

namespace ft
{
	template <class Key, class compare = ft::less<Key>, class Alloc = std::allocator<Key> >
	class set
	{
		public:
			typedef Key					key_type;
			typedef Key					value_type;
			typedef compare				key_compare;
			typedef key_compare			value_compare;
			typedef Alloc				allocator_type;
			typedef size_t				size_type;
			typedef value_type&			reference;
			typedef const value_type&	const_reference;
			typedef value_type*			pointer;
			typedef const value_type*	const_pointer;

			// allocator
			typedef setConstIterator<Key, Key, compare>				iterator;
			typedef setConstIterator<Key, Key, compare>				const_iterator;
			typedef setReverseConstIterator<Key, Key, compare>		reverse_iterator;
			typedef setReverseConstIterator<Key, Key, compare>		const_reverse_iterator;

		private:
			node<key_type, value_type, key_compare>		*root;
			saver<key_type, value_type, key_compare>	*svr;
			unsigned int								num_of_ele;

			void	setSV() { svr->root = root; }

		public:
			/** Basic function **/
			// default constructor
			explicit set(const key_compare &cmp = key_compare(), const allocator_type &alloc = allocator_type()) : root(NULL), num_of_ele(0)
			{
				(void)cmp;
				(void)alloc;
				svr = new ft::saver<Key, Key, compare>();
			}

			// range constructor
			template <class InputIterator>
			set (InputIterator first, InputIterator last, const key_compare &cmp = key_compare(), const allocator_type &alloc = allocator_type()) : root(NULL), num_of_ele(0)
			{
				(void)cmp;
				(void)alloc;
				svr = new ft::saver<Key, Key, compare>();
				insert(first, last);
			}

			// copy constructor
			set (const set &x) : root(NULL), num_of_ele(0)
			{
				root = new node<Key, Key, compare>(*(x.root));
				num_of_ele = x.num_of_ele;
				svr = new ft::saver<Key, Key, compare>();
				setSV();
			}

			// =operator
			set<Key, compare, Alloc>	&operator=(const set<Key, compare, Alloc> &x)
			{
				root->deleteAll(root);
				root = new node<Key, Key, compare>(*(x.root));
				num_of_ele = x.num_of_ele;
				setSV();
				return (*this);
			}

			// destructor
			~set()
			{
				if (num_of_ele > 0)
					root->deleteAll(root);
				delete (svr);
			}

			/** Iterator function **/
			iterator				begin() { return iterator(root->getLeftest(root), svr); }
			const_iterator			begin() const { return (const_iterator(root->getLeftest(root), svr)); }
			iterator				end() { return iterator(NULL, svr); }
			const_iterator			end() const { return (const_iterator(NULL, svr)); }
			reverse_iterator		rbegin() { return (reverse_iterator(root->getRightest(root), svr)); }
			const_reverse_iterator	rbegin() const { return (const_reverse_iterator(root->getRightest(root), svr)); }
			reverse_iterator		rend() { return reverse_iterator(NULL, svr); }
			const_reverse_iterator	rend() const { return (const_reverse_iterator(NULL, svr)); }

			/** Caoacity function **/
			bool		empty() const { return (num_of_ele == 0); }
			size_type	size() const { return (num_of_ele); }
			size_type	max_size() const { return (Alloc().max_size()); }

			/** Modifiers **/
			pair<iterator, bool>	insert(const key_type &x)
			{
				node<Key, Key, compare>	*ret;
				if (num_of_ele == 0)
				{
					num_of_ele++;
					root = new node<Key, Key, compare>(x, x);
					setSV();
					return (pair<iterator, bool>(iterator(root, svr), true));
				}
				else
				{
					if ((ret = root->find(root, x)) != NULL)
						return (pair<iterator, bool>(iterator(ret, svr), false));
					else
					{
						num_of_ele++;
						ret = root->mergeInsert(root, x, x);
						root = root->getRoot(root);
						setSV();
						return (pair<iterator, bool>(iterator(ret, svr), true));
					}
				}
			}

			iterator				insert(iterator position, const key_type &x)
			{
				(void)position;
				return (insert(x).first);
			}

			template <class InputIterator>
			void					insert(InputIterator first, InputIterator last)
			{
				for (InputIterator it = first; it != last; it++)
					insert(*it);
			}

			size_type				erase(const key_type &key)
			{
				if (num_of_ele == 0)
					return (0);
				else
				{

					if (root->find(root, key) == NULL)
						return (0);
					else
					{
						if (num_of_ele == 1)
						{
							delete (root);
							root = NULL;
							setSV();
							num_of_ele--;
							return (1);
						}
						root->deleteNode(&root, root, key);
						setSV();
						num_of_ele--;
						return (1);
					}
				}
			}

			void					erase(iterator position)
			{
				erase(*position);
			}

			void					erase(iterator first, iterator last)
			{
				iterator	iter = first;
				iterator	iter_next;
				while (iter != last)
				{
					iter_next = iter;
					iter_next++;
					erase(*iter);
					iter = iter_next;
					if (iter == last)
						break;
				}

			}

			void					swap(set &x)
			{
				node<key_type, value_type, compare>	*tmp = root;
				root = x.root;
				x.root = tmp;

				unsigned int	temp_size = num_of_ele;
				num_of_ele = x.num_of_ele;
				x.num_of_ele = temp_size;
			}

			void					clear()
			{
				if (num_of_ele == 0)
					return ;
				num_of_ele = 0;
				root->deleteAll(root);
				root = NULL;
			}

			/** Lookup function **/
			iterator		find(const key_type &key)
			{
				if (num_of_ele == 0)
					return (iterator(NULL, svr));
				return (iterator(root->find(root, key), svr));
			}

			const_iterator	find(const key_type &key) const
			{
				if (num_of_ele == 0)
					return (const_iterator(NULL, svr));
				return (const_iterator(root->find(root, key), svr));
			}

			size_type		count(const key_type &key) const
			{
				if (num_of_ele == 0 || root->find(root, key) == NULL)
					return (0);
				return (1);
			}

			iterator		lower_bound(const key_type &key)
			{
				iterator	it = this->begin();

				for (; it != end(); it++)
				{
					if (key <= *it)
						return (it);
				}
				return (it);
			}

			const_iterator	lower_bound(const key_type &key) const
			{
				const_iterator	it = begin();

				for (; it != end(); it++)
				{
					if (key <= *it)
						return (it);
				}
				return (it);
			}

			iterator		upper_bound(const key_type &key)
			{
				iterator	it = this->begin();

				for (; it != end(); it++)
				{
					if (key < *it)
						return (it);
				}
				return (it);
			}

			const_iterator	upper_bound(const key_type &key) const
			{
				const_iterator	it = this->begin();

				for (; it != end(); it++)
				{
					if (key < *it)
						return (it);
				}
				return (it);
			}

			pair<iterator, iterator>				equal_range(const key_type &key)
			{
				return (pair<iterator, iterator>(lower_bound(key), upper_bound(key)));
			}

			pair<const_iterator, const_iterator>	equal_range(const key_type &key) const
			{
				return (pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key)));
			}

			/** Observers function **/
			key_compare		key_comp() const { return (key_compare()); }
			value_compare	value_comp() const { return (value_compare()); }

			void	nodePrint(void)
			{
				root->tree_print(root, "", true);
			}
	};

	template < class Key, class compare, class Alloc >
	bool	operator==(const set<Key, compare, Alloc> &lhs, const set<Key, compare, Alloc> &rhs)
	{
		return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template < class Key, class compare, class Alloc >
	bool	operator!=(const set<Key, compare, Alloc> &lhs, const set<Key, compare, Alloc> &rhs)
	{
		return (!(lhs == rhs));
	}

	template < class Key, class compare, class Alloc >
	bool	operator<(const set<Key, compare, Alloc> &lhs, const set<Key, compare, Alloc> &rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template < class Key, class compare, class Alloc >
	bool	operator>(const set<Key, compare, Alloc>& lhs, const set<Key, compare, Alloc>& rhs )
	{
		return (rhs < lhs);
	}

	template < class Key, class compare, class Alloc >
	bool	operator<=(const set<Key, compare, Alloc> &lhs, const set<Key, compare, Alloc> &rhs)
	{
		return (!(lhs > rhs));
	}

	template < class Key, class compare, class Alloc >
	bool	operator>=(const set<Key, compare, Alloc> &lhs, const set<Key, compare, Alloc> &rhs)
	{
		return (!(lhs < rhs));
	}

	template < class Key, class compare, class Alloc >
	void swap(set< Key, compare, Alloc >& x, set< Key, compare, Alloc >& y)
	{
	  x.swap(y);
	}
}

#endif
