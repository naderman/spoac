/**
* This file is part of SPOAC.
*
* SPOAC is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* SPOAC is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* @package    SPOAC
* @author     Nils Adermann <naderman at naderman dot de>
* @copyright  2010 Nils Adermann
* @license    http://www.gnu.org/licenses/gpl.txt
*             GNU General Public License
*/

#ifndef SPOAC_STM_VARIANTMAP_H
#define SPOAC_STM_VARIANTMAP_H

#include <map>
#include <vector>
#include <algorithm>
#include <boost/variant.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/enum_params_with_a_default.hpp>

#define VARIANT_MAP_LIMIT_TYPES BOOST_VARIANT_LIMIT_TYPES

namespace spoac
{
    /**
    * A wrapper around STL map with boost::variant values.
    *
    * The template arguments are the key type and then a variable number of
    * value types. These types must meet the BoundedType requirements:
    * http://www.boost.org/doc/libs/1_37_0/doc/html/variant/
    * reference.html#variant.concepts.bounded-type
    */
    template <
        typename Key,
        BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
            VARIANT_MAP_LIMIT_TYPES, typename T,
            boost::detail::variant::void_
        )
    >
    class VariantMap
    {
    public:

        // type definitions for all the types defined within an STL map
        typedef Key key_type;

        /**
        * The variant type which is used as the map value type
        */
        typedef typename boost::variant<
            BOOST_PP_ENUM_PARAMS(VARIANT_MAP_LIMIT_TYPES, T)
        > variant_type;

        typedef typename std::map<Key, variant_type> map_type;

        typedef typename map_type::allocator_type allocator_type;
        typedef typename map_type::const_iterator const_iterator;
        typedef typename map_type::const_reverse_iterator
            const_reverse_iterator;
        typedef typename map_type::iterator iterator;
        typedef typename map_type::key_compare key_compare;
        typedef typename map_type::reverse_iterator reverse_iterator;
        typedef typename map_type::size_type size_type;
        typedef typename map_type::value_type value_type;
        typedef typename map_type::value_compare value_compare;

        typedef typename std::pair<iterator, iterator> bounds;
        typedef typename std::pair<const_iterator, const_iterator> const_bounds;

        /**
        * A visitor which returns the variant's value with the given type.
        *
        * Requires template parameter T to be default constructible.
        */
        template <typename T>
        struct visitor_settype : boost::static_visitor<T>
        {
            T default_value;

            /**
            * Visitor constructor which stores the default value to be returned
            * if the variant's type is not the same as the template type.
            */
            visitor_settype(T& default_value) : default_value(default_value)
            {
            }

            /**
            * Function call operator which simply returns the argument of the
            * same type as the template type.
            */
            T operator()(const T& x) const
            {
                return x;
            }

            /**
            * Template function call operator which returns the default value
            * passed to the constructor for all types.
            * This method is called if the type does not match that of the
            * template.
            */
            template <class X>
            T operator()(const X& x) const
            {
                return default_value;
            }
        };

        /**
        * A functor which calls boost::apply_visitor on the second member of a
        * pair with the visitor passed to the constructor.
        * The visitor should have a void return type.
        */
        template <typename Visitor>
        struct ValueApplyVisitor
        {
            /**
            * Constructor which simply stores a visitor internally.
            */
            ValueApplyVisitor(Visitor& visitor) : visitor(visitor)
            {
            }

            /**
            * Function call operator which calls boost::apply_visitor with the
            * visitor passed to the constructor on the second attribute of the
            * data pair.
            */
            void operator()(const value_type& data)
            {
                boost::apply_visitor(visitor, data.second);
            }

            Visitor& visitor;
        };

        /**
        * Default constructor - simple wrapper around STL map constructor.
        */
        VariantMap() : map()
        {
        }

        /**
        * Constructor from a sequence of another map - simple wrapper around STL
        * map constructor.
        */
        template <class InputIterator>
        VariantMap(InputIterator first, InputIterator last) : map(first, last)
        {
        }

        /**
        * Copy constructor which simply copy constructs the STL map.
        */
        VariantMap(const VariantMap& v) : map(v.map)
        {
        }

        /**
        * Virtual descructor.
        * Does nothing for now.
        */
        virtual ~VariantMap()
        {
        }

        /**
        * Assignment operator which only copies the internal map.
        */
        VariantMap& operator=(const VariantMap& v)
        {
            map = v.map;
            return *this;
        }

        /**
        * The get<T>() template method provides typed access to elements.
        *
        * Using this method allows for compile time type checking. If a value
        * does not have the correct type at runtime the default value will be
        * returned. The type T has to be default constructible.
        */
        template <typename T>
        T get(const Key& key, T default_value = T())
        {
            visitor_settype<T> visitor(default_value);
            return apply_visitor(visitor, map[key]);
        }

        /**
        * Applies a visitor to all elements in the specified range using the
        * ValueApplyVisitor template.
        */
        template<typename InputIterator, typename Visitor>
        void for_each_apply_visitor(
            InputIterator first, InputIterator last, Visitor& visitor)
        {
            std::for_each(first, last, ValueApplyVisitor<Visitor>(visitor));
        }

        /**
        * Overloaded wrapper which applies a visitor from begin() to end().
        */
        template<typename Visitor>
        void for_each_apply_visitor(Visitor& visitor)
        {
            for_each_apply_visitor(begin(), end(), visitor);
        }

        std::vector<Key> get_keys() const
        {
            std::vector<Key> result;

            const_iterator it;
            for (it = begin(); it != end(); ++it)
            {
                result.push_back(it->first);
            }
        }

        bool has_key(const Key& key) const
        {
            return find(key) != end();
        }

    private:
        /**
        * Internal instance of an STL map.
        */
        map_type map;


    // STL map interface - simple wrapper functions
    public:
        iterator begin()
        {
            return map.begin();
        }

        const_iterator begin() const
        {
            return map.begin();
        }

        void clear()
        {
            map.clear();
        }

        size_type count(const Key& key) const
        {
            return map.count(key);
        }

        bool empty() const
        {
            return map.empty();
        }

        iterator end()
        {
            return map.end();
        }

        const_iterator end() const
        {
            return map.end();
        }

        bounds equal_range(const Key& key)
        {
            return map.equal_range(key);
        }

        const_bounds equal_range(const Key& key) const
        {
            return map.equal_range(key);
        }

        void erase(iterator pos)
        {
            map.erase(pos);
        }

        size_type erase(const Key& key)
        {
            return map.erase(key);
        }

        void erase(iterator first, iterator last)
        {
            map.erase(first, last);
        }

        iterator find(const Key& key)
        {
            return find(key);
        }

        const_iterator find(const Key& key) const
        {
            return find(key);
        }

        allocator_type get_allocator() const
        {
            return map.get_allocator();
        }

        std::pair<iterator, bool> insert(const value_type& value)
        {
            return map.insert(value);
        }

        iterator insert(iterator pos, const value_type& value)
        {
            return map.insert(pos, value);
        }

        template <typename InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            map.insert<InputIterator>(first, last);
        }

        key_compare key_comp() const
        {
            return map.key_comp();
        }

        iterator lower_bound(const Key& key)
        {
            return map.lower_bound(key);
        }

        const_iterator lower_bound(const Key& key) const
        {
            return map.lower_bound(key);
        }

        size_type max_size() const
        {
            return map.max_size();
        }

        variant_type& operator[](const Key& key)
        {
            return map[key];
        }

        reverse_iterator rbegin()
        {
            return map.rbegin();
        }

        const_reverse_iterator rbegin() const
        {
            return map.rbegin();
        }

        reverse_iterator rend()
        {
            return map.rend();
        }

        const_reverse_iterator rend() const
        {
            return map.rend();
        }

        size_type size() const
        {
            return map.size();
        }

        virtual void swap(VariantMap& other_map)
        {
            map.swap(other_map.map);
        }

        iterator upper_bound(const Key& key)
        {
            return map.upper_bound(key);
        }

        const_iterator upper_bound(const Key& key) const
        {
            return map.upper_bound(key);
        }

        value_compare value_comp() const
        {
            return map.value_comp();
        }
    };
}

#endif
