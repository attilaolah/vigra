/************************************************************************/
/*                                                                      */
/*               Copyright 1998-2001 by Ullrich Koethe                  */
/*       Cognitive Systems Group, University of Hamburg, Germany        */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    You may use, modify, and distribute this software according       */
/*    to the terms stated in the LICENSE file included in               */
/*    the VIGRA distribution.                                           */
/*                                                                      */
/*    The VIGRA Website is                                              */
/*        http://kogs-www.informatik.uni-hamburg.de/~koethe/vigra/      */
/*    Please direct questions, bug reports, and contributions to        */
/*        koethe@informatik.uni-hamburg.de                              */
/*                                                                      */
/*  THIS SOFTWARE IS PROVIDED AS IS AND WITHOUT ANY EXPRESS OR          */
/*  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. */
/*                                                                      */
/************************************************************************/
 
 
#ifndef VIGRA_RGBVALUE_HXX
#define VIGRA_RGBVALUE_HXX

#include <cmath>    // abs(double)
#include <cstdlib>  // abs(int)
#include "vigra/config.hxx"
#include "vigra/numerictraits.hxx"
#include "vigra/accessor.hxx"
#include "vigra/tinyvector.hxx"

namespace vigra {

/********************************************************/
/*                                                      */
/*                      RGBValue                        */
/*                                                      */
/********************************************************/

/** \brief Class for a single RGB value.

    This class contains three values (of the specified type) that represent 
    red, green, and blue color channels. There are three possibilities
    to access these values: accessor functions (\ref red(), \ref green(),
    \ref blue()), index operator (operator[](dx), where 0 is red,
    1 is green and 2 is blue) and iterator (STL-compatible random access
    iterator that references the three colors in turn). The latter two
    methods, together with the necessary embedded typedefs, ensure
    compatibility of a RGBValue with a STL vector.
    
    <a href="BasicArithmeticFunctionsforRGBValue.html">Arithmetic operations</a> 
    on RGBValues are defined as component-wise applications of these 
    operations. Addition, subtraction, and multiplication of two RGBValues 
    (+=, -=, *=, +, -, *, unary -), multiplication and division of an
    RGBValue with a double, and NumericTraits/PromoteTraits are defined, 
    so that RGBValue fulfills the requirements of a Linear Algebra. 
    
    A number of <a href="AccessorsforRGBValue.html">Accessors</a> are provided
    that allow access to the RGBValues as a whole, to a selected
    component, or to the luminance value.
    
    <b>\#include</b> "<a href="rgbvalue_8hxx-source.html">vigra/rgbvalue.hxx</a>"<br>
    Namespace: vigra
*/
template <class VALUETYPE>
class RGBValue
: public TinyVector<VALUETYPE, 3>
{
  public:
        /** STL-compatible definition of valuetype
        */
    typedef VALUETYPE value_type;
        /** STL-compatible definition of iterator
        */
    typedef typename TinyVector<VALUETYPE, 3>::iterator iterator;
        /** STL-compatible definition of const iterator
        */
    typedef typename TinyVector<VALUETYPE, 3>::const_iterator const_iterator;
    
        /** Construct from explicit color values 
        */    
    RGBValue(value_type red, value_type green, value_type blue)
    {
        data_[0] = red;
        data_[1] = green;
        data_[2] = blue;
    }
    
        /** Construct gray value 
        */    
    RGBValue(value_type gray)
    {
        data_[0] = gray;
        data_[1] = gray;
        data_[2] = gray;
    }
    
        /** Construct from another sequence (must have length 3!)
        */
    template <class Iterator>   
    RGBValue(Iterator i, Iterator end)
    {
        for(iterator p = begin(); i != end; ++i, ++p)
            *p = detail::RequiresExplicitCast<value_type>::cast(*i);
    }

        /** Default constructor (sets all components to 0)  
        */    
    RGBValue()
    {
        data_[0] = 0;
        data_[1] = 0;
        data_[2] = 0;
    }

#if !defined(TEMPLATE_COPY_CONSTRUCTOR_BUG)
        
    RGBValue(RGBValue const & r)
    {
        data_[0] = r.red();
        data_[1] = r.green();
        data_[2] = r.blue();
    }

    RGBValue & operator=(RGBValue const & r)
    {
        data_[0] = r.red();
        data_[1] = r.green();
        data_[2] = r.blue();
        return *this;
    }

#endif // TEMPLATE_COPY_CONSTRUCTOR_BUG

    
        /** Copy constructor.
        */
    template <class U>   
    RGBValue(RGBValue<U> const & r)
    {
        data_[0] = detail::RequiresExplicitCast<value_type>::cast(r.red());
        data_[1] = detail::RequiresExplicitCast<value_type>::cast(r.green());
        data_[2] = detail::RequiresExplicitCast<value_type>::cast(r.blue());
    }

        /** Copy assignment.
        */    
    template <class U>   
    RGBValue & operator=(RGBValue<U> const & r)
    {
        data_[0] = detail::RequiresExplicitCast<value_type>::cast(r.red());
        data_[1] = detail::RequiresExplicitCast<value_type>::cast(r.green());
        data_[2] = detail::RequiresExplicitCast<value_type>::cast(r.blue());
        return *this;
    }

        /** construct from TinyVector
        */
    RGBValue(TinyVector<value_type, 3> const & r)
    {
        data_[0] = r[0];
        data_[1] = r[1];
        data_[2] = r[2];
    }

        /** assign TinyVector.
        */    
    RGBValue & operator=(TinyVector<value_type, 3> const & r)
    {
        data_[0] = r[0];
        data_[1] = r[1];
        data_[2] = r[2];
        return *this;
    }

        /** Unary negation (construct RGBValue with negative velues)
        */
    RGBValue operator-() const
    {
        return RGBValue(-red(), -green(), -blue());
    }
    
        /** Access red component.
        */
    value_type & red() { return data_[0]; }
    
        /** Access green component.
        */
    value_type & green() { return data_[1]; }
     
        /** Access blue component.
        */
    value_type & blue() { return data_[2]; }
    
        /** Get red component.
        */
    value_type red() const { return data_[0]; }
    
        /** Get green component.
        */
    value_type green() const { return data_[1]; }
    
        /** Get blue component.
        */
    value_type blue() const { return data_[2]; }
    
        /** Calculate luminance.
        */
    value_type luminance() const { 
         return detail::RequiresExplicitCast<value_type>::cast(0.3*red() + 0.59*green() + 0.11*blue()); }
    
        /** Calculate magnitude.
        */
    typename NumericTraits<VALUETYPE>::RealPromote
    magnitude() const { 
#ifndef CMATH_NOT_IN_STD
         return std::sqrt(squaredMagnitude());
#else
         return sqrt(squaredMagnitude());
#endif
    }
    
        /** Calculate squared magnitude.
        */
    typename NumericTraits<VALUETYPE>::Promote
    squaredMagnitude() const { 
         return red()*red() + green()*green() + blue()*blue();
    }
    
        /** Set red component. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>VALUETYPE</TT>.
        */
    template <class V>
    void setRed(V const & value) { data_[0] = detail::RequiresExplicitCast<value_type>::cast(value); }

        /** Set green component.The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>VALUETYPE</TT>.
        */
    template <class V>
    void setGreen(V const & value) { data_[1] = detail::RequiresExplicitCast<value_type>::cast(value); }

        /** Set blue component.The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>VALUETYPE</TT>.
        */
    template <class V>
    void setBlue(V const & value) { data_[2] = detail::RequiresExplicitCast<value_type>::cast(value); }
};

/********************************************************/
/*                                                      */
/*                     RGBValue Comparison              */
/*                                                      */
/********************************************************/

/** \addtogroup RGBValueOperators Functions for RGBValue

    \brief <b>\#include</b> "<a href="rgbvalue_8hxx-source.html">vigra/rgbvalue.hxx</a>
    
    These functions fulfill the requirements of a Linear Algebra.

    Namespace: vigra
    <p>
    
 */
//@{
    /// component-wise equal
template <class V1, class V2>
inline
bool 
operator==(RGBValue<V1> const & l, RGBValue<V2> const & r)
{
    return (l.red() == r.red()) &&
       (l.green() == r.green()) &&
       (l.blue() == r.blue());
}

    /// component-wise not equal
template <class V1, class V2>
inline
bool 
operator!=(RGBValue<V1> const & l, RGBValue<V2> const & r)
{
    return (l.red() != r.red()) ||
       (l.green() != r.green()) ||
       (l.blue() != r.blue());
}


//@}

/********************************************************/
/*                                                      */
/*                      RGBValue-Traits                 */
/*                                                      */
/********************************************************/

/** \page RGBValueTraits Numeric and Promote Traits of RGBValue
    The numeric and promote traits for RGBValues follow 
    the general specifications for \ref NumericPromotionTraits. 
    They are implemented in terms of the traits of the basic types by 
    partial template specialization:
    
    \code
    
    template <class T>
    struct NumericTraits<RGBValue<T> >
    {
        typedef RGBValue<typename NumericTraits<T>::Promote> Promote;
        typedef RGBValue<typename NumericTraits<T>::RealPromote> RealPromote;

        typedef typename NumericTraits<T>::isIntegral isIntegral;
        typedef VigraFalseType isScalar;

        // etc.
    };
    
    template <class T1, class T2>
    struct PromoteTraits<RGBValue<T1>, RGBValue<T2> > 
    { 
        typedef RGBValue<typename PromoteTraits<T1, T2>::Promote> Promote;
    };
    \endcode

    <b>\#include</b> "<a href="rgbvalue_8hxx-source.html">vigra/rgbvalue.hxx</a>"<br>
    Namespace: vigra
    
*/

#if !defined(NO_PARTIAL_TEMPLATE_SPECIALIZATION)

template <class T>
struct NumericTraits<RGBValue<T> >
{
    typedef RGBValue<T> Type;
    typedef RGBValue<typename NumericTraits<T>::Promote> Promote;
    typedef RGBValue<typename NumericTraits<T>::RealPromote> RealPromote;

    typedef typename NumericTraits<T>::isIntegral isIntegral;
    typedef VigraFalseType isScalar;
    typedef VigraFalseType isOrdered;
    
    static RGBValue<T> zero() { 
        return RGBValue<T>(NumericTraits<T>::zero()); 
    }
    static RGBValue<T> one() { 
        return RGBValue<T>(NumericTraits<T>::one()); 
    }
    static RGBValue<T> nonZero() { 
        return RGBValue<T>(NumericTraits<T>::nonZero()); 
    }
    
    static Promote toPromote(RGBValue<T> const & v) { 
        return Promote(v); 
    }
    static RealPromote toRealPromote(RGBValue<T> const & v) { 
        return RealPromote(v); 
    }
    static RGBValue<T> fromPromote(Promote const & v) { 
        return RGBValue<T>(NumericTraits<T>::fromPromote(v.red()),
                           NumericTraits<T>::fromPromote(v.green()),
                           NumericTraits<T>::fromPromote(v.blue()));
    }
    static RGBValue<T> fromRealPromote(RealPromote const & v) {
        return RGBValue<T>(NumericTraits<T>::fromRealPromote(v.red()),
                           NumericTraits<T>::fromRealPromote(v.green()),
                           NumericTraits<T>::fromRealPromote(v.blue()));
    }
};

template <class T1, class T2>
struct PromoteTraits<RGBValue<T1>, RGBValue<T2> >
{
    typedef RGBValue<typename PromoteTraits<T1, T2>::Promote> Promote;
};

template <class T>
struct PromoteTraits<RGBValue<T>, double >
{
    typedef RGBValue<typename NumericTraits<T>::RealPromote> Promote;
};

template <class T>
struct PromoteTraits<double, RGBValue<T> >
{
    typedef RGBValue<typename NumericTraits<T>::RealPromote> Promote;
};

#else // NO_PARTIAL_TEMPLATE_SPECIALIZATION

#define RGBVALUE_NUMTRAITS(T) \
template<>\
struct NumericTraits<RGBValue<T> >\
{\
    typedef RGBValue<T> Type;\
    typedef RGBValue<NumericTraits<T>::Promote> Promote;\
    typedef RGBValue<NumericTraits<T>::RealPromote> RealPromote;\
    typedef NumericTraits<T>::isIntegral isIntegral;\
    typedef VigraFalseType isScalar;\
    typedef VigraFalseType isOrdered;\
    \
    static RGBValue<T> zero() { \
        return RGBValue<T>(NumericTraits<T>::zero()); \
    }\
    static RGBValue<T> one() { \
        return RGBValue<T>(NumericTraits<T>::one()); \
    }\
    static RGBValue<T> nonZero() { \
        return RGBValue<T>(NumericTraits<T>::nonZero()); \
    }\
    \
    static Promote toPromote(RGBValue<T> const & v) { \
        return Promote(v); \
    }\
    static RealPromote toRealPromote(RGBValue<T> const & v) { \
        return RealPromote(v); \
    }\
    static RGBValue<T> fromPromote(Promote const & v) { \
        RGBValue<T> res;\
        RGBValue<T>::iterator d = res.begin();\
        Promote::const_iterator s = v.begin();\
        for(; d != res.end(); ++d, ++s)\
            *d = NumericTraits<T>::fromPromote(*s);\
        return res;\
    }\
    static RGBValue<T> fromRealPromote(RealPromote const & v) {\
        RGBValue<T> res;\
        RGBValue<T>::iterator d = res.begin();\
        RealPromote::const_iterator s = v.begin();\
        for(; d != res.end(); ++d, ++s)\
            *d = NumericTraits<T>::fromRealPromote(*s);\
        return res;\
    }\
};

#define RGBVALUE_PROMTRAITS1(type1) \
template<> \
struct PromoteTraits<RGBValue<type1>, RGBValue<type1> > \
{ \
    typedef RGBValue<PromoteTraits<type1, type1>::Promote> Promote; \
    static Promote toPromote(RGBValue<type1> const & v) { \
        return static_cast<Promote>(v); } \
};

#define RGBVALUE_PROMTRAITS2(type1, type2) \
template<> \
struct PromoteTraits<RGBValue<type1>, RGBValue<type2> > \
{ \
    typedef RGBValue<PromoteTraits<type1, type2>::Promote> Promote; \
    static Promote toPromote(RGBValue<type1> const & v) { \
        return static_cast<Promote>(v); } \
    static Promote toPromote(RGBValue<type2> const & v) { \
        return static_cast<Promote>(v); } \
};

RGBVALUE_NUMTRAITS(unsigned char)
RGBVALUE_NUMTRAITS(int)
RGBVALUE_NUMTRAITS(float)
RGBVALUE_NUMTRAITS(double)
RGBVALUE_PROMTRAITS1(unsigned char)
RGBVALUE_PROMTRAITS1(int)
RGBVALUE_PROMTRAITS1(float)
RGBVALUE_PROMTRAITS1(double)
RGBVALUE_PROMTRAITS2(float, unsigned char)
RGBVALUE_PROMTRAITS2(unsigned char, float)
RGBVALUE_PROMTRAITS2(int, unsigned char)
RGBVALUE_PROMTRAITS2(unsigned char, int)
RGBVALUE_PROMTRAITS2(int, float)
RGBVALUE_PROMTRAITS2(float, int)
RGBVALUE_PROMTRAITS2(double, unsigned char)
RGBVALUE_PROMTRAITS2(unsigned char, double)
RGBVALUE_PROMTRAITS2(int, double)
RGBVALUE_PROMTRAITS2(double, int)
RGBVALUE_PROMTRAITS2(double, float)
RGBVALUE_PROMTRAITS2(float, double)

#undef RGBVALUE_NUMTRAITS
#undef RGBVALUE_PROMTRAITS1
#undef RGBVALUE_PROMTRAITS2

#endif // NO_PARTIAL_TEMPLATE_SPECIALIZATION


/********************************************************/
/*                                                      */
/*                      RGBValue-Arithmetic             */
/*                                                      */
/********************************************************/

/** \addtogroup RGBValueOperators
 */
//@{
    /// componentwise add-assignment
template <class V1, class V2>
inline
RGBValue<V1> & 
operator+=(RGBValue<V1> & l, RGBValue<V2> const & r)
{
    l.red() += r.red();
    l.green() += r.green();
    l.blue() += r.blue();
    return l;
}

    /// componentwise subtract-assignment
template <class V1, class V2>
inline
RGBValue<V1> & 
operator-=(RGBValue<V1> & l, RGBValue<V2> const & r)
{
    l.red() -= r.red();
    l.green() -= r.green();
    l.blue() -= r.blue();
    return l;
}

    /// componentwise multiply-assignment
template <class V1, class V2>
inline
RGBValue<V1> & 
operator*=(RGBValue<V1> & l, RGBValue<V2> const & r)
{
    l.red() *= r.red();
    l.green() *= r.green();
    l.blue() *= r.blue();
    return l;
}

    /// componentwise scalar multiply-assignment
template <class V>
inline
RGBValue<V> & 
operator*=(RGBValue<V> & l, double r)
{
    l.red() *= r;
    l.green() *= r;
    l.blue() *= r;
    return l;
}

    /// componentwise scalar divide-assignment 
template <class V>
inline
RGBValue<V> & 
operator/=(RGBValue<V> & l, double r)
{
    l.red() /= r;
    l.green() /= r;
    l.blue() /= r;
    return l;
}

#ifndef CMATH_NOT_IN_STD
using std::abs;
#else
using ::abs;
#endif

    /// component-wise absolute value
template <class T>
inline
RGBValue<T> abs(RGBValue<T> const & v) { 
    return RGBValue<T>(abs(v.red()), abs(v.green()),  abs(v.blue()));
}



    /// component-wise addition
template <class V1, class V2>
inline 
typename PromoteTraits<RGBValue<V1>, RGBValue<V2> >::Promote
operator+(RGBValue<V1> const & r1, RGBValue<V2> const & r2)
{
    typename PromoteTraits<RGBValue<V1>, RGBValue<V2> >::Promote res(r1);
    
    res += r2;
    
    return res;
}

    /// component-wise subtraction
template <class V1, class V2>
inline 
typename PromoteTraits<RGBValue<V1>, RGBValue<V2> >::Promote
operator-(RGBValue<V1> const & r1, RGBValue<V2> const & r2)
{
    typename PromoteTraits<RGBValue<V1>, RGBValue<V2> >::Promote res(r1);
    
    res -= r2;
    
    return res;
}

    /// component-wise multiplication
template <class V1, class V2>
inline 
typename PromoteTraits<RGBValue<V1>, RGBValue<V2> >::Promote
operator*(RGBValue<V1> const & r1, RGBValue<V2> const & r2)
{
    typename PromoteTraits<RGBValue<V1>, RGBValue<V2> >::Promote res(r1);
    
    res *= r2;
    
    return res;
}

    /// component-wise left scalar multiplication
template <class V>
inline 
typename NumericTraits<RGBValue<V> >::RealPromote
operator*(double v, RGBValue<V> const & r)
{
    typename NumericTraits<RGBValue<V> >::RealPromote res(r);
    
    res *= v;
    
    return res;
}

    /// component-wise right scalar multiplication
template <class V>
inline 
typename NumericTraits<RGBValue<V> >::RealPromote
operator*(RGBValue<V> const & r, double v)
{
    typename NumericTraits<RGBValue<V> >::RealPromote res(r);
    
    res *= v;
    
    return res;
}

    /// component-wise scalar division
template <class V>
inline 
typename NumericTraits<RGBValue<V> >::RealPromote
operator/(RGBValue<V> const & r, double v)
{
    typename NumericTraits<RGBValue<V> >::RealPromote res(r);
    
    res /= v;
    
    return res;
}

    /// cross product
template <class V1, class V2>
inline 
typename PromoteTraits<RGBValue<V1>, RGBValue<V2> >::Promote
cross(RGBValue<V1> const & r1, RGBValue<V2> const & r2)
{
    typedef typename PromoteTraits<RGBValue<V1>, RGBValue<V2> >::Promote
            Res;
    return  Res(r1.green()*r2.blue() - r1.blue()*r2.green(),
                r1.blue()*r2.red() - r1.red()*r2.blue(),
                r1.red()*r2.green() - r1.green()*r2.red());
}

    /// dot product
template <class V1, class V2>
inline 
typename PromoteTraits<V1, V2>::Promote
dot(RGBValue<V1> const & r1, RGBValue<V2> const & r2)
{
    return r1.red()*r2.red() + r1.green()*r2.green() + r1.blue()*r2.blue();
}

#ifdef CMATH_NOT_IN_STD
    using ::ceil;
#else
    using std::ceil;
#endif

    /** Apply ceil() function to each RGB component.
    */
template <class V>
inline
RGBValue<V>
ceil(RGBValue<V> const & r)
{
    return RGBValue<V>(ceil(r.red()), 
                       ceil(r.green()),
                       ceil(r.blue()));
};

#ifdef CMATH_NOT_IN_STD
    using ::floor;
#else
    using std::floor;
#endif

    /** Apply floor() function to each RGB component.
    */
template <class V>
inline
RGBValue<V>
floor(RGBValue<V> const & r)
{
    return RGBValue<V>(floor(r.red()), 
                       floor(r.green()),
                       floor(r.blue()));
};

//@}

/********************************************************/
/*                                                      */
/*                      RGBValue-Accessors              */
/*                                                      */
/********************************************************/

/** \defgroup RGBValueAccessors Accessors for RGBValue */
//@{
    /** Encapsulate access to rgb values.

    <b>\#include</b> "<a href="rgbvalue_8hxx-source.html">vigra/rgbvalue.hxx</a>"<br>
    Namespace: vigra
    */
template <class RGBVALUE>
class RGBAccessor 
: public VectorAccessor<RGBVALUE>
{
  public:
  
    typedef typename RGBVALUE::value_type component_type;

        /** Get value of the red component
        */
    template <class RGBIterator>
    component_type red(RGBIterator & rgb) const
    {
        return (*rgb).red();
    }
    
        /** Set value of the red component. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>component_type</TT>.
        */
    template <class V, class RGBIterator>
    void setRed(V const & value, RGBIterator & rgb) const
    {
        (*rgb).setRed(value);
    }
    
        /** Get value of the red component at a distance
        */
    template <class RGBIterator, class DISTANCE>
    component_type red(RGBIterator & rgb, DISTANCE const & dist) const
    {
        return rgb[dist].red();
    }
    
        /** Set value of the red component at a distance. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>component_type</TT>.
        */
    template <class V, class RGBIterator, class DISTANCE>
    void setRed(V const & value, RGBIterator & rgb, DISTANCE const & dist) const
    {
        rgb[dist].setRed(value);
    }
       
        /** Get value of the green component
        */
    template <class RGBIterator>
    component_type green(RGBIterator & rgb) const
    {
        return (*rgb).green();
    }
    
        /** Set value of the green component. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>component_type</TT>.
        */
    template <class V, class RGBIterator>
    void setGreen(V const & value, RGBIterator & rgb) const
    {
        (*rgb).setGreen(value);
    }
    
        /** Get value of the green component at a distance
        */
    template <class RGBIterator, class DISTANCE>
    component_type green(RGBIterator & rgb, DISTANCE const & d) const
    {
        return rgb[d].green();
    }
    
        /** Set value of the green component at a distance. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>component_type</TT>.
        */
    template <class V, class RGBIterator, class DISTANCE>
    void setGreen(V const & value, RGBIterator & rgb, DISTANCE const & d) const
    {
        rgb[d].setGreen(value);
    }
    
        /** Get value of the blue component
        */
    template <class RGBIterator>
    component_type blue(RGBIterator & rgb) const
    {
        return (*rgb).blue();
    }
    
        /** Set value of the blue component. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>component_type</TT>.
        */
    template <class V, class RGBIterator>
    void setBlue(V const & value, RGBIterator & rgb) const
    {
        (*rgb).setBlue(value);
    }
    
        /** Get value of the blue component at a distance
        */
    template <class RGBIterator, class DISTANCE>
    component_type blue(RGBIterator & rgb, DISTANCE const & d) const
    {
        return rgb[d].blue();
    }
    
        /** Set value of the blue component at a distance. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>component_type</TT>.
        */
    template <class V, class RGBIterator, class DISTANCE>
    void setBlue(V const & value, RGBIterator & rgb, DISTANCE const & d) const
    {
        rgb[d].setBlue(value);
    }

};


/********************************************************/
/*                                                      */
/*                       RedAccessor                    */
/*                                                      */
/********************************************************/

    /** Encapsulate access to red band of an rgb value.

    <b>\#include</b> "<a href="rgbvalue_8hxx-source.html">vigra/rgbvalue.hxx</a>"<br>
    Namespace: vigra
    */
template <class RGBVALUE>
class RedAccessor
{
  public:
    typedef typename RGBVALUE::value_type value_type;

        /** Get value of the red component
        */
    template <class ITERATOR>
    value_type operator()(ITERATOR & i) const { 
        return (*i).red(); 
    }

        /** Get value of the red component at a distance
        */
    template <class ITERATOR, class DISTANCE>
    value_type operator()(ITERATOR & i, DISTANCE const & d) const 
    { 
        return i[d].red(); 
    }
    
        /** Set value of the red component. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>value_type</TT>.
        */
    template <class V, class ITERATOR>
    void set(V const & value, ITERATOR & i) const { 
        (*i).setRed(value); 
    }
    

        /** Set value of the red component at a distance. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>value_type</TT>.
        */
    template <class V, class ITERATOR, class DISTANCE>
    void set(V const & value, ITERATOR & i, DISTANCE const & d) const 
    { 
        i[d].setRed(value); 
    }
};

/********************************************************/
/*                                                      */
/*                     GreenAccessor                    */
/*                                                      */
/********************************************************/

    /** Encapsulate access to green band of an rgb value.

    <b>\#include</b> "<a href="rgbvalue_8hxx-source.html">vigra/rgbvalue.hxx</a>"<br>
    Namespace: vigra
    */
template <class RGBVALUE>
class GreenAccessor
{
  public:
    typedef typename RGBVALUE::value_type value_type;

        /** Get value of the green component
        */
    template <class ITERATOR>
    value_type operator()(ITERATOR & i) const { 
        return (*i).green(); 
    }

        /** Get value of the green component at a distance
        */
    template <class ITERATOR, class DISTANCE>
    value_type operator()(ITERATOR & i, DISTANCE const & d) const 
    { 
        return i[d].green(); 
    }
    
        /** Set value of the green component. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>value_type</TT>.
        */
    template <class V, class ITERATOR>
    void set(V const & value, ITERATOR & i) const { 
        (*i).setGreen(value); 
    }
    

        /** Set value of the green component at a distance. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>value_type</TT>.
        */
    template <class V, class ITERATOR, class DISTANCE>
    void set(V const & value, ITERATOR & i, DISTANCE const & d) const 
    { 
        i[d].setGreen(value); 
    }
};

/********************************************************/
/*                                                      */
/*                     BlueAccessor                     */
/*                                                      */
/********************************************************/

    /** Encapsulate access to blue band of an rgb value.

    <b>\#include</b> "<a href="rgbvalue_8hxx-source.html">vigra/rgbvalue.hxx</a>"<br>
    Namespace: vigra
    */
template <class RGBVALUE>
class BlueAccessor
{
  public:
    typedef typename RGBVALUE::value_type value_type;

        /** Get value of the blue component
        */
    template <class ITERATOR>
    value_type operator()(ITERATOR & i) const { 
        return (*i).blue(); 
    }

        /** Get value of the blue component at a distance
        */
    template <class ITERATOR, class DISTANCE>
    value_type operator()(ITERATOR & i, DISTANCE const & d) const 
    { 
        return i[d].blue(); 
    }
    
        /** Set value of the blue component. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>value_type</TT>.
        */
    template <class V, class ITERATOR>
    void set(V const & value, ITERATOR & i) const { 
        (*i).setBlue(value); 
    }
    

        /** Set value of the blue component at a distance. The type <TT>V</TT> of the passed
            in <TT>value</TT> is automatically converted to <TT>value_type</TT>.
        */
    template <class V, class ITERATOR, class DISTANCE>
    void set(V const & value, ITERATOR & i, DISTANCE const & d) const 
    { 
        i[d].setBlue(value); 
    }
};

/********************************************************/
/*                                                      */
/*                  RGBToGrayAccessor                   */
/*                                                      */
/********************************************************/

    /** Encapsulate access to luminance of an rgb value.

    <b>\#include</b> "<a href="rgbvalue_8hxx-source.html">vigra/rgbvalue.hxx</a>"<br>
    Namespace: vigra
    */
template <class RGBVALUE>
class RGBToGrayAccessor
{
  public:
    typedef typename RGBVALUE::value_type value_type;

        /** Get value of the luminance
        */
    template <class ITERATOR>
    value_type operator()(ITERATOR & i) const { 
                return (*i).luminance(); }

        /** Get value of the luminance at a distance
        */
    template <class ITERATOR, class DISTANCE>
    value_type operator()(ITERATOR & i, DISTANCE const & d) const 
    { 
        return i[d].luminance(); 
    }
};


//@}


} // namespace vigra

#endif // VIGRA_RGBVALUE_HXX
