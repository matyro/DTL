/* compile_time_list.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include<type_traits>
#include <utility>

#include "meta/index_sequence.h"
#include "meta/identity.h"

namespace meta
{

    namespace list
    {

        template<class T, size_t TNum>
        class List;

        template<class T, size_t TNum>
        class List: public List<T, TNum - 1>
        {
        private:

        protected:
            T data;

            template<size_t ... Ints1, size_t ... Ints2>
            constexpr List<T, sizeof...(Ints1) + sizeof...(Ints2)> _concat(const List<T, sizeof...(Ints2)> rhs,  meta::integer_sequence<Ints1...>,  meta::integer_sequence<Ints2...>) const
            {
                return List<T, sizeof...(Ints1) + sizeof...(Ints2)>(this->get<Ints1>()..., rhs.template get<Ints2>()...);
            }

            template<size_t ... Ints>
            constexpr List<T, sizeof...(Ints)> _subSection(const meta::integer_sequence<Ints...>) const
            {
                return List<T, sizeof...(Ints)>(this->get<Ints>()...);
            }

        public:

            template<class ... TArgs>
            constexpr List(T d, TArgs&& ... arg)
            : List<T, TNum - 1>(std::forward<TArgs>(arg)...), data(d)
            {
                static_assert(TNum != sizeof...(TArgs), "Number of arguements and list size does not match!");
            }

            constexpr List(const List<T, TNum>& rhs)
            : List<T, TNum - 1>(static_cast<List<T, TNum - 1>>(rhs)), data(rhs.data)
            {
            }


            template<size_t TNum2, typename Indices1 =  meta::index_sequence_from<TNum>, typename Indices2 =  meta::index_sequence_from<TNum2>>
            constexpr List<T, TNum + TNum2> concat(const List<T, TNum2>& rhs) const
            {
                return this->_concat(rhs, Indices1(), Indices2());
            }

            template<size_t TBegin, size_t TEnd>
            constexpr List<T, TEnd - TBegin + 1> subSection() const
            {
                static_assert(TBegin <= TEnd, "TStart cant be smaller then TEnd!");
                static_assert(TEnd >= 0, "TEnd cant be smaller then 0!");
                static_assert(TBegin <= TNum, "TStart cant be bigger then TNum!");
                static_assert(TEnd <= TNum, "TEnd cant be bigger then TNum!");
                return _subSection( meta::index_sequence_from_to<TBegin, TEnd + 1>());
            }

            template<class TFunc, class TReturn>
            constexpr TReturn foldl(TFunc func, TReturn init) const
            {
                return List<T, TNum - 1>::foldl(func, func(init, data));
            }

            template<class TFunc, class TReturn>
            constexpr TReturn foldr(TFunc func, TReturn init) const
            {
                return func(data, List<T, TNum - 1>::foldr(func, init));
            }

            /// Map function over all elements of a tuple
            /**
            *  When Lambdas are used as function the return value is not constexpr anymore (earlier then C++17)
            */
            template<class TFunc>
            constexpr List<typename std::result_of<TFunc(T)>::type, TNum> map(TFunc func) const
            {
                // Call func on local data and add it to a list
                // append the rest
                //
                return List<typename std::result_of<TFunc(T)>::type, 1>(func(data)).concat( dynamic_cast<const List<T, TNum - 1>*>(this)->template map<TFunc>(func) );
            }

            /// Merge two lists elementwise into a new one
            /**
            *  When Lambdas are used as function the return value is not constexpr anymore (earlier then C++17)
            */
            template<class TFunc, class T2>
            constexpr List< typename std::result_of<TFunc(T, T2)>::type , TNum> zip(TFunc func, List<T2, TNum> rhs) const
            {
                // Call func on local data and add it to a list
                // append the rest
                return List< typename std::result_of<TFunc(T, T2)>::type, 1>(func(data, rhs.get())).concat( static_cast< const List<T, TNum - 1> >(*this).template zip<TFunc, typename std::result_of<TFunc(T, T2)>::type>(func, static_cast<const List<T, TNum - 1>>(rhs)) );
            }

            template<size_t TNum2>
            constexpr List<T, TNum + TNum2> operator+(const List<T, TNum2> rhs) const
            {
                return this->concat(rhs);
            }

            constexpr List<T, TNum> operator=(const List<T, TNum>& rhs) const
            {
                return List<T, TNum>(rhs);
            }

            /// Compare content of equal type and size
            constexpr bool operator==(const List<T, TNum>& rhs) const
            {
                std::cout << "Compare: " << data << " | " << rhs.data << std::endl;
                if(data == rhs.data)
                {
                    return (static_cast<List<T, TNum - 1>>(*this) == static_cast<List<T, TNum - 1>>(rhs));
                }
                return false;
            }
            /// Compare content of equal length
            template<class T2>
            constexpr bool operator==(const List<T2, TNum>& rhs) const
            {
                if(data == rhs.get())
                {
                    return (static_cast<List<T, TNum - 1>>(*this) == static_cast<List<T2, TNum - 1>>(rhs));
                }
                return false;
            }

            /// All types with not equal size or type compare to false
            template<class T2, size_t TNum2>
            constexpr bool operator==(const List<T2, TNum2>& rhs) const
            {
                return false;
            }

            constexpr T get(const size_t i) const
            {
                return (i == 0) ? data : List<T, TNum - 1>::get(i - 1);
            }

            template<size_t TI>
            constexpr T get() const
            {
                static_assert(TI < TNum, "Element out of valid range!");
                static_assert(TI >= 0, "Element out of valid range!");
                return static_cast<List<T, TNum - TI> >(*this).get();
            }

            constexpr T get() const
            {
                return data;
            }

            constexpr size_t size() const
            {
                return TNum;
            }

            template<class TReturn>
            constexpr operator const List<TReturn, TNum>() const
            {
                static_assert(std::is_convertible<TReturn, T>::value, "Cant cast into this type!");

                return List<TReturn, 1>( static_cast<TReturn>(data) ).concat( static_cast<List<TReturn, TNum - 1>>(  static_cast<List<T, TNum - 1>>(*this) ) );
            }

        };

        template<class T>
        class List<T, 1> : public List<T, 0>
        {
        private:

        protected:
            T data;

            template<size_t ... Ints>
            constexpr List<T, 1 + sizeof...(Ints)> _concat(const List<T, sizeof...(Ints)> rhs, const meta::integer_sequence<Ints...>) const
            {
                return List<T, 1 + sizeof...(Ints)>(this->get<0>(), rhs.template get<Ints>()...);
            }

            template<size_t ... Ints>
            constexpr List<T, sizeof...(Ints)> _subSection(const meta::integer_sequence<Ints...>) const
            {
                return List<T, sizeof...(Ints)>(this->get<Ints>()...);
            }

        public:

            constexpr List(T d)
            : data(d)
            {
            }

            constexpr List(const List<T, 1>& rhs)
            : data(rhs.data)
            {
            }

            template<size_t TNum2>
            constexpr List<T, 1 + TNum2> concat(const List<T, TNum2>& rhs) const
            {
                return this->_concat(rhs, meta::index_sequence_from<TNum2>());
            }

            template<size_t TBegin, size_t TEnd>
            constexpr List<T, TEnd - TBegin + 1> subSection() const
            {
                static_assert(TBegin <= TEnd, "TStart cant be smaller then TEnd!");
                static_assert(TEnd >= 0, "TEnd cant be smaller then 0!");
                static_assert(TBegin <= 1, "TStart cant be bigger then TNum!");
                static_assert(TEnd <= 1, "TEnd cant be bigger then TNum!");
                return _subSection(meta::index_sequence_from_to<TBegin, TEnd + 1>());
            }

            template<class TFunc, class TReturn>
            constexpr TReturn foldr(TFunc func, TReturn init) const
            {
                return func(data, init);
            }

            template<class TFunc, class TReturn>
            constexpr TReturn foldl(TFunc func, TReturn init) const
            {
                return func(List<T, 0>::foldl(func, init), data);
            }

            template<class TFunc>
            constexpr List<typename std::result_of<TFunc(T)>::type, 1> map(TFunc func) const
            {
                return List<typename std::result_of<TFunc(T)>::type, 1>(func(data));
            }

            template<class TFunc, class T2>
            constexpr List<typename std::result_of<TFunc(T, T2)>::type, 1> zip(TFunc func, List<T2, 1> rhs) const
            {
                // Call func on local data and add it to a list
                // append the rest
                return List<typename std::result_of<TFunc(T, T2)>::type, 1>(func(data, rhs.get()));
            }

            template<size_t TNum2>
            constexpr List<T, 1 + TNum2> operator+(const List<T, TNum2> rhs) const
            {
                return this->concat(rhs);
            }

            /// Compare content of equal type and size
            constexpr bool operator==(const List<T, 1>& rhs) const
            {
                return data == rhs.data;
            }
            /// Compare content of equal length
            template<class T2>
            constexpr bool operator==(const List<T2, 1>& rhs) const
            {
                return data == rhs.get();
            }

            /// All types with not equal size or type compare to false
            template<class T2, size_t TNum2>
            constexpr bool operator==(const List<T2, TNum2>& rhs) const
            {
                return false;
            }

            constexpr List<T, 1> operator=(const List<T, 1>& rhs) const
            {
                return List<T, 1>(rhs);
            }

            template<class TReturn>
            constexpr operator const List<TReturn, 1>() const
            {
                static_assert(std::is_convertible<TReturn, T>::value, "Cant cast into this type!");

                return List<TReturn, 1>( static_cast<TReturn>(data) );
            }


            constexpr T get(size_t i) const
            {
                (void)(i);
                return data;
            }

            template<size_t TI>
            constexpr T get() const
            {
                static_assert(TI == 0, "Element out of valid range!");
                return data;
            }

            constexpr T get() const
            {
                return data;
            }

            constexpr size_t size() const
            {
                return 1;
            }

        };

        template<class T>
        class List<T, 0>
        {
        private:

        protected:

        public:
            constexpr List()
            {
            }

            template<size_t TNum2>
            constexpr List<T, TNum2> concat(List<T, TNum2> rhs) const
            {
                return rhs;
            }

            template<size_t TStart, size_t TEnd>
            constexpr List<T, TEnd - TStart> subSection() const
            {
                //static_assert(false, "No subsection of empty List!");
            }

            template<class TFunc, class T2>
            constexpr List<typename std::result_of<TFunc(T, T2)>::type, 0> zip(TFunc func, List<T2, 0> rhs) const
            {
                // Call func on local data and add it to a list
                // append the rest
                return List<typename std::result_of<TFunc(T, T2)>::type, 0>();
            }

            template<size_t TNum2>
            constexpr List<T, TNum2> operator+(const List<T, TNum2> rhs) const
            {
                return rhs;
            }

            /// Compare content of equal length
            template<class T2>
            constexpr bool operator==(const List<T2, 0>& rhs) const
            {
                return true;
            }

            constexpr bool operator==(const List<T, 0>& rhs) const
            {
                return true;
            }

            template<class TFunc, class TReturn>
            constexpr TReturn foldl(TFunc func, TReturn init) const
            {
                return init;
            }

            template<class TFunc, class TReturn>
            constexpr List<TReturn, 0> map(TFunc func) const
            {
                return List<TReturn, 0>();
            }

            constexpr T get(size_t i) const
            {
                //static_assert(false, "Cant call get from an empty List!");
            }

            template<size_t TI>
            constexpr T get() const
            {
                //static_assert(false, "Cant call get from an empty List!");
            }

            constexpr T get() const
            {
                //static_assert(false, "Cant call get from an empty List!");
            }

            constexpr size_t size() const
            {
                return 0;
            }

        };

        /*template<size_t i, class T, size_t TNum>
        constexpr T get(const List<T, TNum> l)
        {
        static_assert((i < TNum), "Request element greater then size of List!");
        static_assert((i >= 0), "Request element smaller 0!");
        return dynamic_cast<const List<TNum - i, T>>(l).get();    }		*/

        template<class T, class ... TArgs>
        constexpr List<T, sizeof...(TArgs) + 1> make_list(T arg, TArgs ... args)
        {
            return List<T, sizeof...(TArgs) + 1>(arg, args...);
        }

    }

}
// */
