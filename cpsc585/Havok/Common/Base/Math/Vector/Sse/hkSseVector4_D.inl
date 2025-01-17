/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


#if !defined(HK_REAL_IS_DOUBLE)
#error This implementation is for 64-Bit double
#endif

/* quad, here for inlining */

#ifndef HK_DISABLE_MATH_CONSTRUCTORS
/* construct, assign, zero */
HK_FORCE_INLINE hkVector4::hkVector4(hkReal a, hkReal b, hkReal c, hkReal d)
{
	m_quad.xy = _mm_setr_pd(a,b);
	m_quad.zw = _mm_setr_pd(c,d);
}

HK_FORCE_INLINE hkVector4::hkVector4(const hkQuadReal& q)
{
	m_quad = q;
}

HK_FORCE_INLINE hkVector4::hkVector4(const hkVector4& v)
{
	m_quad = v.m_quad;
}
#endif

HK_FORCE_INLINE void hkVector4::set(hkReal a, hkReal b, hkReal c, hkReal d)
{
	m_quad.xy = _mm_setr_pd(a,b);
	m_quad.zw = _mm_setr_pd(c,d);
}

HK_FORCE_INLINE void hkVector4::set( hkSimdRealParameter a, hkSimdRealParameter b, hkSimdRealParameter c, hkSimdRealParameter d )
{
	m_quad.xy = _mm_unpacklo_pd(a.m_real, b.m_real);
	m_quad.zw = _mm_unpacklo_pd(c.m_real, d.m_real);
}

HK_FORCE_INLINE void hkVector4::setAll(const hkReal& a)
{
	m_quad.xy = _mm_set1_pd(a);
	m_quad.zw = _mm_set1_pd(a);
}

HK_FORCE_INLINE void hkVector4::setAll(hkSimdRealParameter a)
{
	m_quad.xy = a.m_real;
	m_quad.zw = a.m_real;
}

HK_FORCE_INLINE void hkVector4::setZero()
{
	m_quad.xy = _mm_setzero_pd();
	m_quad.zw = _mm_setzero_pd();
}

template <> 
HK_FORCE_INLINE void hkVector4::zeroComponent<0>()
{
	m_quad.xy = _mm_move_sd(m_quad.xy, _mm_setzero_pd());
}

template <> 
HK_FORCE_INLINE void hkVector4::zeroComponent<1>()
{
	m_quad.xy = _mm_move_sd(_mm_setzero_pd(), m_quad.xy);
}

template <> 
HK_FORCE_INLINE void hkVector4::zeroComponent<2>()
{
	m_quad.zw = _mm_move_sd(m_quad.zw, _mm_setzero_pd());
}

template <> 
HK_FORCE_INLINE void hkVector4::zeroComponent<3>()
{
	m_quad.zw = _mm_move_sd(_mm_setzero_pd(), m_quad.zw);
}

template <int I> 
HK_FORCE_INLINE void hkVector4::zeroComponent()
{
	HK_VECTOR4_NOT_IMPLEMENTED;
}

HK_FORCE_INLINE void hkVector4::zeroComponent(const int i)
{
	HK_ASSERT2(0x3bc36625, (i>=0) && (i<4), "Component index out of range");
	static HK_ALIGN16( const hkUint64 cx[2][2] ) ={ 
		{ 0x0000000000000000, 0xffffffffffffffff },
		{ 0xffffffffffffffff, 0x0000000000000000 } };

		if (i<2)
			m_quad.xy = _mm_and_pd( m_quad.xy, reinterpret_cast<const hkSingleReal*>(cx)[i] );
		else
			m_quad.zw = _mm_and_pd( m_quad.zw, reinterpret_cast<const hkSingleReal*>(cx)[i-2] );
}

HK_FORCE_INLINE const hkSimdReal hkVector4::getComponent(const int i) const
{
	HK_ASSERT2(0x6d0c31d7, i>=0 && i<4, "index out of bounds for component access");
	switch (i)
	{
		case 1:  return hkSimdReal::convert(_mm_unpackhi_pd(m_quad.xy, m_quad.xy));
		case 2:  return hkSimdReal::convert(_mm_unpacklo_pd(m_quad.zw, m_quad.zw));
		case 3:  return hkSimdReal::convert(_mm_unpackhi_pd(m_quad.zw, m_quad.zw));
		default: return hkSimdReal::convert(_mm_unpacklo_pd(m_quad.xy, m_quad.xy));
	}
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::getComponent<0>() const
{
	return hkSimdReal::convert(_mm_unpacklo_pd(m_quad.xy, m_quad.xy));
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::getComponent<1>() const
{
	return hkSimdReal::convert(_mm_unpackhi_pd(m_quad.xy, m_quad.xy));
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::getComponent<2>() const
{
	return hkSimdReal::convert(_mm_unpacklo_pd(m_quad.zw, m_quad.zw));
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::getComponent<3>() const
{
	return hkSimdReal::convert(_mm_unpackhi_pd(m_quad.zw, m_quad.zw));
}

template <int I>
HK_FORCE_INLINE const hkSimdReal hkVector4::getComponent() const
{
	HK_VECTOR4_NOT_IMPLEMENTED;
	return hkSimdReal::getConstant<HK_QUADREAL_0>();
}

HK_FORCE_INLINE void hkVector4::setAdd(hkVector4Parameter v0, hkVector4Parameter v1)
{
	m_quad.xy = _mm_add_pd(v0.m_quad.xy, v1.m_quad.xy);
	m_quad.zw = _mm_add_pd(v0.m_quad.zw, v1.m_quad.zw);
}

HK_FORCE_INLINE void hkVector4::setSub(hkVector4Parameter v0, hkVector4Parameter v1)
{
	m_quad.xy = _mm_sub_pd(v0.m_quad.xy, v1.m_quad.xy);
	m_quad.zw = _mm_sub_pd(v0.m_quad.zw, v1.m_quad.zw);
}

HK_FORCE_INLINE void hkVector4::setMul(hkVector4Parameter v0, hkVector4Parameter v1)
{
	m_quad.xy = _mm_mul_pd(v0.m_quad.xy, v1.m_quad.xy);
	m_quad.zw = _mm_mul_pd(v0.m_quad.zw, v1.m_quad.zw);
}

HK_FORCE_INLINE void hkVector4::setMul(hkVector4Parameter v1, hkSimdRealParameter r)
{
	m_quad.xy = _mm_mul_pd( r.m_real, v1.m_quad.xy);
	m_quad.zw = _mm_mul_pd( r.m_real, v1.m_quad.zw);
}

HK_FORCE_INLINE void hkVector4::setSubMul(hkVector4Parameter a, hkVector4Parameter b, hkSimdRealParameter r)
{
	m_quad.xy = _mm_sub_pd( a.m_quad.xy, _mm_mul_pd( r.m_real, b.m_quad.xy) );
	m_quad.zw = _mm_sub_pd( a.m_quad.zw, _mm_mul_pd( r.m_real, b.m_quad.zw) );
}

HK_FORCE_INLINE void hkVector4::setAddMul(hkVector4Parameter a, hkVector4Parameter b, hkSimdRealParameter r)
{
	m_quad.xy = _mm_add_pd( a.m_quad.xy, _mm_mul_pd( r.m_real, b.m_quad.xy) );
	m_quad.zw = _mm_add_pd( a.m_quad.zw, _mm_mul_pd( r.m_real, b.m_quad.zw) );
}

HK_FORCE_INLINE void hkVector4::setAddMul(hkVector4Parameter a, hkVector4Parameter x, hkVector4Parameter y)
{
	m_quad.xy = _mm_add_pd( a.m_quad.xy, _mm_mul_pd( x.m_quad.xy, y.m_quad.xy) );
	m_quad.zw = _mm_add_pd( a.m_quad.zw, _mm_mul_pd( x.m_quad.zw, y.m_quad.zw) );
}

HK_FORCE_INLINE void hkVector4::setSubMul(hkVector4Parameter a, hkVector4Parameter x, hkVector4Parameter y)
{
	m_quad.xy = _mm_sub_pd( a.m_quad.xy, _mm_mul_pd( x.m_quad.xy, y.m_quad.xy) );
	m_quad.zw = _mm_sub_pd( a.m_quad.zw, _mm_mul_pd( x.m_quad.zw, y.m_quad.zw) );
}

HK_FORCE_INLINE void hkVector4::setCross( hkVector4Parameter v0, hkVector4Parameter v1 )
{
	// x = y0z1 - z0y1
	const hkSingleReal x2 = _mm_mul_pd(_mm_shuffle_pd(v0.m_quad.xy, v0.m_quad.zw, _MM_SHUFFLE2(0,1)), _mm_shuffle_pd(v1.m_quad.zw, v1.m_quad.xy, _MM_SHUFFLE2(1,0)));
	const hkSingleReal x = _mm_sub_pd(x2, _mm_shuffle_pd(x2, x2, _MM_SHUFFLE2(0,1)));

	// y = z0x1 - x0z1
	const hkSingleReal y2 = _mm_mul_pd(_mm_shuffle_pd(v0.m_quad.zw, v0.m_quad.xy, _MM_SHUFFLE2(0,0)), _mm_shuffle_pd(v1.m_quad.xy, v1.m_quad.zw, _MM_SHUFFLE2(0,0)));
	const hkSingleReal y = _mm_sub_pd(y2, _mm_shuffle_pd(y2, y2, _MM_SHUFFLE2(0,1)));

	m_quad.xy = _mm_shuffle_pd(x, y, _MM_SHUFFLE2(0,0));

	// z = x0y1 - y0x1
	const hkSingleReal z2 = _mm_mul_pd(v0.m_quad.xy, _mm_shuffle_pd(v1.m_quad.xy, v1.m_quad.xy, _MM_SHUFFLE2(0,1)));
	m_quad.zw = _mm_sub_pd(z2, _mm_shuffle_pd(z2, z2, _MM_SHUFFLE2(0,1)));
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::equal(hkVector4Parameter a) const
{
	hkVector4Comparison comp;
	comp.m_mask.xy = _mm_cmpeq_pd(m_quad.xy, a.m_quad.xy);
	comp.m_mask.zw = _mm_cmpeq_pd(m_quad.zw, a.m_quad.zw);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::notEqual(hkVector4Parameter a) const
{
	hkVector4Comparison comp;
	comp.m_mask.xy = _mm_cmpneq_pd(m_quad.xy, a.m_quad.xy);
	comp.m_mask.zw = _mm_cmpneq_pd(m_quad.zw, a.m_quad.zw);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::less(hkVector4Parameter a) const
{
	hkVector4Comparison comp;
	comp.m_mask.xy = _mm_cmplt_pd(m_quad.xy, a.m_quad.xy);
	comp.m_mask.zw = _mm_cmplt_pd(m_quad.zw, a.m_quad.zw);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::lessEqual(hkVector4Parameter a) const
{
	hkVector4Comparison comp;
	comp.m_mask.xy = _mm_cmple_pd(m_quad.xy, a.m_quad.xy);
	comp.m_mask.zw = _mm_cmple_pd(m_quad.zw, a.m_quad.zw);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::greater(hkVector4Parameter a) const
{
	hkVector4Comparison comp;
	comp.m_mask.xy = _mm_cmpgt_pd(m_quad.xy, a.m_quad.xy);
	comp.m_mask.zw = _mm_cmpgt_pd(m_quad.zw, a.m_quad.zw);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::greaterEqual(hkVector4Parameter a) const
{
	hkVector4Comparison comp;
	comp.m_mask.xy = _mm_cmpge_pd(m_quad.xy, a.m_quad.xy);
	comp.m_mask.zw = _mm_cmpge_pd(m_quad.zw, a.m_quad.zw);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::lessZero() const
{
	hkVector4Comparison comp;
	const hkSingleReal zero = _mm_setzero_pd();
	comp.m_mask.xy = _mm_cmplt_pd(m_quad.xy, zero);
	comp.m_mask.zw = _mm_cmplt_pd(m_quad.zw, zero);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::lessEqualZero() const
{
	hkVector4Comparison comp;
	const hkSingleReal zero = _mm_setzero_pd();
	comp.m_mask.xy = _mm_cmple_pd(m_quad.xy, zero);
	comp.m_mask.zw = _mm_cmple_pd(m_quad.zw, zero);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::greaterZero() const
{
	hkVector4Comparison comp;
	const hkSingleReal zero = _mm_setzero_pd();
	comp.m_mask.xy = _mm_cmpgt_pd(m_quad.xy, zero);
	comp.m_mask.zw = _mm_cmpgt_pd(m_quad.zw, zero);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::greaterEqualZero() const
{
	hkVector4Comparison comp;
	const hkSingleReal zero = _mm_setzero_pd();
	comp.m_mask.xy = _mm_cmpge_pd(m_quad.xy, zero);
	comp.m_mask.zw = _mm_cmpge_pd(m_quad.zw, zero);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::equalZero() const
{
	hkVector4Comparison comp;
	const hkSingleReal zero = _mm_setzero_pd();
	comp.m_mask.xy = _mm_cmpeq_pd(m_quad.xy, zero);
	comp.m_mask.zw = _mm_cmpeq_pd(m_quad.zw, zero);
	return comp;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::notEqualZero() const
{
	hkVector4Comparison comp;
	const hkSingleReal zero = _mm_setzero_pd();
	comp.m_mask.xy = _mm_cmpneq_pd(m_quad.xy, zero);
	comp.m_mask.zw = _mm_cmpneq_pd(m_quad.zw, zero);
	return comp;
}

HK_FORCE_INLINE void hkVector4::setSelect( hkVector4ComparisonParameter comp, hkVector4Parameter trueValue, hkVector4Parameter falseValue )
{
#if HK_SSE_VERSION >= 0x41
	m_quad.xy = _mm_blendv_pd(falseValue.m_quad.xy, trueValue.m_quad.xy, comp.m_mask.xy);
	m_quad.zw = _mm_blendv_pd(falseValue.m_quad.zw, trueValue.m_quad.zw, comp.m_mask.zw);
#else
	m_quad.xy = _mm_or_pd( _mm_and_pd(comp.m_mask.xy, trueValue.m_quad.xy), _mm_andnot_pd(comp.m_mask.xy, falseValue.m_quad.xy) );
	m_quad.zw = _mm_or_pd( _mm_and_pd(comp.m_mask.zw, trueValue.m_quad.zw), _mm_andnot_pd(comp.m_mask.zw, falseValue.m_quad.zw) );
#endif
}

template <>
HK_FORCE_INLINE void hkVector4::setNeg<2>(hkVector4Parameter v)
{
	static HK_ALIGN16( const hkUint64 negateMask[2] ) = { 0x8000000000000000, 0x8000000000000000 }; // todo constant
	m_quad.xy = _mm_xor_pd(v.m_quad.xy, *(const hkSingleReal*)&negateMask);
	m_quad.zw = v.m_quad.zw;
}

template <>
HK_FORCE_INLINE void hkVector4::setNeg<3>(hkVector4Parameter v)
{
	static HK_ALIGN16( const hkUint64 negateMask[2] ) = { 0x8000000000000000, 0x8000000000000000 }; // todo constant
	static HK_ALIGN16( const hkUint64 negateMask2[2] ) = { 0x8000000000000000, 0x0000000000000000 }; // todo constant
	m_quad.xy = _mm_xor_pd(v.m_quad.xy, *(const hkSingleReal*)&negateMask);
	m_quad.zw = _mm_xor_pd(v.m_quad.zw, *(const hkSingleReal*)&negateMask2);
}

template <>
HK_FORCE_INLINE void hkVector4::setNeg<4>(hkVector4Parameter v)
{
	static HK_ALIGN16( const hkUint64 negateMask[2] ) = { 0x8000000000000000, 0x8000000000000000 }; // todo constant
	m_quad.xy = _mm_xor_pd(v.m_quad.xy, *(const hkSingleReal*)&negateMask);
	m_quad.zw = _mm_xor_pd(v.m_quad.zw, *(const hkSingleReal*)&negateMask);
}

template <int N>
HK_FORCE_INLINE void hkVector4::setNeg(hkVector4Parameter v)
{
	HK_VECTOR4_NOT_IMPLEMENTED;
}

HK_FORCE_INLINE void hkVector4::setAbs(hkVector4Parameter v)
{
	m_quad = hkMath::quadFabs(v.m_quad);
}

HK_FORCE_INLINE void hkVector4::setMin(hkVector4Parameter a, hkVector4Parameter b)
{
	m_quad.xy = _mm_min_pd(a.m_quad.xy, b.m_quad.xy);
	m_quad.zw = _mm_min_pd(a.m_quad.zw, b.m_quad.zw);
}

HK_FORCE_INLINE void hkVector4::setMax(hkVector4Parameter a, hkVector4Parameter b)
{
	m_quad.xy = _mm_max_pd(a.m_quad.xy, b.m_quad.xy);
	m_quad.zw = _mm_max_pd(a.m_quad.zw, b.m_quad.zw);
}

/* matrix3, rotation, quaternion, transform */

HK_FORCE_INLINE void hkVector4::_setRotatedDir(const hkMatrix3& r, hkVector4Parameter b )
{
	const hkQuadReal c0 = r.getColumn<0>().m_quad;
	const hkQuadReal c1 = r.getColumn<1>().m_quad;
	const hkQuadReal c2 = r.getColumn<2>().m_quad;

	const hkSingleReal b0 = _mm_shuffle_pd( b.m_quad.xy, b.m_quad.xy, _MM_SHUFFLE2(0,0));
	const hkSingleReal b1 = _mm_shuffle_pd( b.m_quad.xy, b.m_quad.xy, _MM_SHUFFLE2(1,1));
	const hkSingleReal b2 = _mm_shuffle_pd( b.m_quad.zw, b.m_quad.zw, _MM_SHUFFLE2(0,0));	

	{
		const hkSingleReal r0 = _mm_mul_pd( c0.xy, b0 );
		const hkSingleReal r1 = _mm_mul_pd( c1.xy, b1 );
		const hkSingleReal r2 = _mm_mul_pd( c2.xy, b2 );

		m_quad.xy = _mm_add_pd( _mm_add_pd(r0, r1), r2 );
	}
	{
		const hkSingleReal r0 = _mm_mul_pd( c0.zw, b0 );
		const hkSingleReal r1 = _mm_mul_pd( c1.zw, b1 );
		const hkSingleReal r2 = _mm_mul_pd( c2.zw, b2 );

		m_quad.zw = _mm_add_pd( _mm_add_pd(r0, r1), r2 );
	}
}

HK_FORCE_INLINE void hkVector4::_setRotatedInverseDir(const hkMatrix3& r, hkVector4Parameter b )
{
#if HK_SSE_VERSION >= 0x41
	const hkQuadReal c0 = r.getColumn<0>().m_quad;
	const hkQuadReal c1 = r.getColumn<1>().m_quad;
	const hkQuadReal c2 = r.getColumn<2>().m_quad;

	const hkSingleReal r0a = _mm_dp_pd( c0.xy, b.m_quad.xy, 0x31 );
	const hkSingleReal r0b = _mm_dp_pd( c0.zw, b.m_quad.zw, 0x11 );

	const hkSingleReal r1a = _mm_dp_pd( c1.xy, b.m_quad.xy, 0x32 );
	const hkSingleReal r1b = _mm_dp_pd( c1.zw, b.m_quad.zw, 0x12 );

	const hkSingleReal r2a = _mm_dp_pd( c2.xy, b.m_quad.xy, 0x31 );
	const hkSingleReal r2b = _mm_dp_pd( c2.zw, b.m_quad.zw, 0x11 );

	m_quad.xy = _mm_or_pd( _mm_add_pd(r0a, r0b), _mm_add_pd(r1a, r1b) );
	m_quad.zw = _mm_add_pd(r2a, r2b);
#else
	hkVector4 c0 = r.getColumn<0>();
	hkVector4 c1 = r.getColumn<1>();
	hkVector4 c2 = r.getColumn<2>();

	HK_TRANSPOSE3(c0,c1,c2);

	const hkSingleReal b0 = _mm_shuffle_pd( b.m_quad.xy, b.m_quad.xy, _MM_SHUFFLE2(0,0));
	const hkSingleReal b1 = _mm_shuffle_pd( b.m_quad.xy, b.m_quad.xy, _MM_SHUFFLE2(1,1));
	const hkSingleReal b2 = _mm_shuffle_pd( b.m_quad.zw, b.m_quad.zw, _MM_SHUFFLE2(0,0));	

	{
		const hkSingleReal r0 = _mm_mul_pd( c0.m_quad.xy, b0 );
		const hkSingleReal r1 = _mm_mul_pd( c1.m_quad.xy, b1 );
		const hkSingleReal r2 = _mm_mul_pd( c2.m_quad.xy, b2 );

		m_quad.xy = _mm_add_pd( _mm_add_pd(r0, r1), r2 );
	}
	{
		const hkSingleReal r0 = _mm_mul_pd( c0.m_quad.zw, b0 );
		const hkSingleReal r1 = _mm_mul_pd( c1.m_quad.zw, b1 );
		const hkSingleReal r2 = _mm_mul_pd( c2.m_quad.zw, b2 );

		m_quad.zw = _mm_add_pd( _mm_add_pd(r0, r1), r2 );
	}
#endif
}


template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::dot<2>(hkVector4Parameter a) const
{
#if HK_SSE_VERSION >= 0x41
	return hkSimdReal::convert(_mm_dp_pd(m_quad.xy, a.m_quad.xy, 0x33));
#elif HK_SSE_VERSION >= 0x30
	const hkSingleReal x2 = _mm_mul_pd(m_quad.xy,a.m_quad.xy);
	return hkSimdReal::convert(_mm_hadd_pd(x2,x2));
#else
	const hkSingleReal x2 = _mm_mul_pd(m_quad.xy,a.m_quad.xy);
	const hkSingleReal result = _mm_add_pd( _mm_shuffle_pd(x2,x2,_MM_SHUFFLE2(1,1)), _mm_shuffle_pd(x2,x2,_MM_SHUFFLE2(0,0))); // xy xy
	return hkSimdReal::convert(result);
#endif
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::dot<3>(hkVector4Parameter a) const
{
#if HK_SSE_VERSION >= 0x41
	const hkSingleReal xy = _mm_dp_pd(m_quad.xy, a.m_quad.xy, 0x33);
	const hkSingleReal z  = _mm_dp_pd(m_quad.zw, a.m_quad.zw, 0x13);
	return hkSimdReal::convert(_mm_add_pd(xy,z));
#elif HK_SSE_VERSION >= 0x30
	const hkSingleReal x2a = _mm_mul_pd(m_quad.xy,a.m_quad.xy);
	const hkSingleReal x2b = _mm_mul_pd(m_quad.zw,a.m_quad.zw);
	const hkSingleReal hsum = _mm_hadd_pd(x2a,x2a); // xy xy
	const hkSingleReal z = _mm_shuffle_pd(x2b,x2b,_MM_SHUFFLE2(0,0)); // zz
	return hkSimdReal::convert(_mm_add_pd(hsum, z)); // xyz xyz
#else
	const hkSingleReal x2a = _mm_mul_pd(m_quad.xy,a.m_quad.xy);
	const hkSingleReal x2b = _mm_mul_pd(m_quad.zw,a.m_quad.zw);
	const hkSingleReal xySum = _mm_add_pd( _mm_shuffle_pd(x2a,x2a,_MM_SHUFFLE2(1,1)), _mm_shuffle_pd(x2a,x2a,_MM_SHUFFLE2(0,0))); // xy xy
	const hkSingleReal z = _mm_shuffle_pd(x2b,x2b,_MM_SHUFFLE2(0,0)); // zz
	const hkSingleReal result = _mm_add_pd( z, xySum); // xyz xyz
	return hkSimdReal::convert(result);
#endif
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::dot<4>(hkVector4Parameter a) const
{
#if HK_SSE_VERSION >= 0x41
	const hkSingleReal xy = _mm_dp_pd(m_quad.xy, a.m_quad.xy, 0x33);
	const hkSingleReal zw = _mm_dp_pd(m_quad.zw, a.m_quad.zw, 0x33);
	return hkSimdReal::convert(_mm_add_pd(xy,zw));
#elif HK_SSE_VERSION >= 0x30
	const hkSingleReal x2a = _mm_mul_pd(m_quad.xy,a.m_quad.xy);
	const hkSingleReal x2b = _mm_mul_pd(m_quad.zw,a.m_quad.zw);
	const hkSingleReal hsum0 = _mm_hadd_pd(x2a,x2b); // xy zw
	return hkSimdReal::convert(_mm_hadd_pd(hsum0,hsum0)); // xyzw all 4
#else
	const hkSingleReal x2a = _mm_mul_pd(m_quad.xy,a.m_quad.xy);
	const hkSingleReal x2b = _mm_mul_pd(m_quad.zw,a.m_quad.zw);
	const hkSingleReal sum0a = _mm_add_pd( _mm_shuffle_pd(x2a,x2a,_MM_SHUFFLE2(0,1)), x2a); // yx+xy = xy xy
	const hkSingleReal sum0b = _mm_add_pd( _mm_shuffle_pd(x2b,x2b,_MM_SHUFFLE2(0,1)), x2b); // wz+zw = zw zw
	const hkSingleReal result = _mm_add_pd( sum0a, sum0b ); // = xyzw xyzw xyzw xyzw
	return hkSimdReal::convert(result);
#endif
}

template <int N>
HK_FORCE_INLINE const hkSimdReal hkVector4::dot(hkVector4Parameter a) const
{
	HK_VECTOR4_NOT_IMPLEMENTED;
	return hkSimdReal::getConstant<HK_QUADREAL_0>();
}


HK_FORCE_INLINE const hkSimdReal hkVector4::dot4xyz1(hkVector4Parameter a) const
{
#if HK_SSE_VERSION >= 0x41
	const hkSingleReal xy = _mm_dp_pd(m_quad.xy, a.m_quad.xy, 0x33);
	const hkSingleReal z  = _mm_dp_pd(m_quad.zw, a.m_quad.zw, 0x13);
	const hkSingleReal xyz= _mm_add_pd(xy,z);
	const hkSingleReal w  = _mm_shuffle_pd(m_quad.zw, m_quad.zw, _MM_SHUFFLE2(1,1));
	return hkSimdReal::convert(_mm_add_pd(xyz,w));
#elif HK_SSE_VERSION >= 0x30
	const hkSingleReal xx2a = _mm_mul_pd(m_quad.xy,a.m_quad.xy);
	const hkSingleReal xx2bf = _mm_mul_pd(m_quad.zw,a.m_quad.zw);
	const hkSingleReal xx2b = _mm_shuffle_pd(m_quad.zw, xx2bf, _MM_SHUFFLE2(0,1));	// replace w by this.w
	const hkSingleReal hsum0 = _mm_hadd_pd(xx2a,xx2b); // xy zw
	return hkSimdReal::convert(_mm_hadd_pd(hsum0,hsum0)); // xyzw all 4
#else
	const hkSingleReal xx2a = _mm_mul_pd(m_quad.xy,a.m_quad.xy);
	const hkSingleReal xx2bf = _mm_mul_pd(m_quad.zw,a.m_quad.zw);
	const hkSingleReal xx2b = _mm_shuffle_pd(m_quad.zw, xx2bf, _MM_SHUFFLE2(0,1));	// replace w by this.w
	const hkSingleReal sum0a = _mm_add_pd( _mm_shuffle_pd(xx2a,xx2a,_MM_SHUFFLE2(0,1)), xx2a); // yx+xy = xy xy
	const hkSingleReal sum0b = _mm_add_pd( _mm_shuffle_pd(xx2b,xx2b,_MM_SHUFFLE2(0,1)), xx2b); // wz+zw = zw zw
	const hkSingleReal result = _mm_add_pd( sum0a, sum0b ); // = xyzw xyzw xyzw xyzw
	return hkSimdReal::convert(result);
#endif
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalAdd<2>() const
{
#if HK_SSE_VERSION >= 0x30
	return hkSimdReal::convert(_mm_hadd_pd(m_quad.xy, m_quad.xy));
#else
	return hkSimdReal::convert(_mm_add_pd( _mm_shuffle_pd(m_quad.xy,m_quad.xy,_MM_SHUFFLE2(0,1)), m_quad.xy));
#endif
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalAdd<3>() const
{
#if HK_SSE_VERSION >= 0x30
	const hkSingleReal x2 = _mm_hadd_pd(m_quad.xy, m_quad.xy);
	return hkSimdReal::convert(_mm_add_pd( _mm_shuffle_pd(m_quad.zw,m_quad.zw,_MM_SHUFFLE2(0,0)), x2));
#else
	const hkSingleReal xySum = _mm_add_pd( _mm_shuffle_pd(m_quad.xy,m_quad.xy,_MM_SHUFFLE2(0,1)), m_quad.xy);
	return hkSimdReal::convert(_mm_add_pd( _mm_shuffle_pd(m_quad.zw,m_quad.zw,_MM_SHUFFLE2(0,0)), xySum));
#endif
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalAdd<4>() const
{
#if HK_SSE_VERSION >= 0x30
	const hkSingleReal x2a = _mm_hadd_pd(m_quad.xy, m_quad.zw);
	return hkSimdReal::convert(_mm_hadd_pd(x2a, x2a));
#else
	const hkSingleReal sum0 = _mm_add_pd( _mm_shuffle_pd(m_quad.xy,m_quad.xy,_MM_SHUFFLE2(0,1)), m_quad.xy); // yx+xy = xy xy
	const hkSingleReal sum1 = _mm_add_pd( _mm_shuffle_pd(m_quad.zw,m_quad.zw,_MM_SHUFFLE2(0,1)), m_quad.zw); // wz+zw = zw zw
	return hkSimdReal::convert(_mm_add_pd( sum0, sum1 )); // xywz all 4
#endif
}

template <int N>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalAdd() const
{
	HK_VECTOR4_NOT_IMPLEMENTED;
	return hkSimdReal::getConstant<HK_QUADREAL_0>();
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMin<1>() const
{
	return getComponent<0>();
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMin<2>() const
{
	return hkSimdReal::convert(_mm_min_pd( _mm_shuffle_pd(m_quad.xy,m_quad.xy,_MM_SHUFFLE2(0,1)), m_quad.xy));
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMin<3>() const
{
	const hkSingleReal xy = _mm_min_pd( _mm_shuffle_pd(m_quad.xy,m_quad.xy,_MM_SHUFFLE2(0,1)), m_quad.xy);
	return hkSimdReal::convert(_mm_min_pd( _mm_shuffle_pd(m_quad.zw,m_quad.zw,_MM_SHUFFLE2(0,0)), xy));
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMin<4>() const
{
	const hkSingleReal sum0 = _mm_min_pd( _mm_shuffle_pd( m_quad.xy, m_quad.xy,_MM_SHUFFLE2(0,1)), m_quad.xy); // yx+xy = xy xy
	const hkSingleReal sum1 = _mm_min_pd( _mm_shuffle_pd( m_quad.zw, m_quad.zw,_MM_SHUFFLE2(0,1)), m_quad.zw); // wz+zw = zw zw
	return hkSimdReal::convert(_mm_min_pd( sum0, sum1 )); // xywz all 4
}

template <int N>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMin() const
{
	HK_VECTOR4_NOT_IMPLEMENTED;
	return hkSimdReal::getConstant<HK_QUADREAL_0>();
}

/* operator () */

HK_FORCE_INLINE hkReal& hkVector4::operator() (int a)
{
	HK_ASSERT2(0x6d0c31d7, a>=0 && a<4, "index out of bounds for component access");
	if (a<2)
		return m_quad.xy.m128d_f64[a];
	else
		return m_quad.zw.m128d_f64[a-2];
}

HK_FORCE_INLINE const hkReal& hkVector4::operator() (int a) const
{
	HK_ASSERT2(0x6d0c31d7, a>=0 && a<4, "index out of bounds for component access");
	if (a<2)
		return m_quad.xy.m128d_f64[a];
	else
		return m_quad.zw.m128d_f64[a-2];
}

HK_FORCE_INLINE void hkVector4::setXYZ_W(hkVector4Parameter xyz, hkVector4Parameter w)
{
	m_quad.xy = xyz.m_quad.xy;
	m_quad.zw = _mm_move_sd(w.m_quad.zw, xyz.m_quad.zw);
}

HK_FORCE_INLINE void hkVector4::setXYZ_W(hkVector4Parameter xyz, hkSimdRealParameter w)
{
	m_quad.xy = xyz.m_quad.xy;
	m_quad.zw = _mm_move_sd(w.m_real, xyz.m_quad.zw);
}

HK_FORCE_INLINE void hkVector4::setW(hkVector4Parameter w)
{
	m_quad.zw = _mm_move_sd(w.m_quad.zw, m_quad.zw);
}

HK_FORCE_INLINE void hkVector4::setXYZ(hkVector4Parameter xyz)
{
	m_quad.xy = xyz.m_quad.xy;
	m_quad.zw = _mm_move_sd(m_quad.zw, xyz.m_quad.zw);
}

HK_FORCE_INLINE void hkVector4::addXYZ(hkVector4Parameter xyz)
{
	m_quad.xy = _mm_add_pd(m_quad.xy, xyz.m_quad.xy);
	m_quad.zw = _mm_add_pd(m_quad.zw, xyz.m_quad.zw);
	HK_ON_DEBUG( *(hkUint64*)&(m_quad.zw.m128d_f64[1]) = 0xffffffffffffffff; )
}

HK_FORCE_INLINE void hkVector4::subXYZ(hkVector4Parameter xyz)
{
	m_quad.xy = _mm_sub_pd(m_quad.xy, xyz.m_quad.xy);
	m_quad.zw = _mm_sub_pd(m_quad.zw, xyz.m_quad.zw);
	HK_ON_DEBUG( *(hkUint64*)&(m_quad.zw.m128d_f64[1]) = 0xffffffffffffffff; )
}

HK_FORCE_INLINE void hkVector4::setXYZ(hkReal v)
{
	const hkSingleReal q = _mm_set1_pd(v);
	m_quad.xy = q;
	m_quad.zw = _mm_move_sd(m_quad.zw, q);
}

HK_FORCE_INLINE void hkVector4::setXYZ(hkSimdRealParameter v)
{
	m_quad.xy = v.m_real;
	m_quad.zw = _mm_move_sd(m_quad.zw, v.m_real);
}

HK_FORCE_INLINE void hkVector4::setXYZ_0(hkVector4Parameter xyz)
{
	static HK_ALIGN16( const hkUint64 cw[2] ) = { 0xffffffffffffffff, 0x0000000000000000 };
	m_quad.xy = xyz.m_quad.xy;
	m_quad.zw = _mm_and_pd( xyz.m_quad.zw, *(const hkSingleReal*)&cw );
}

HK_FORCE_INLINE void hkVector4::setBroadcastXYZ(const int i, hkVector4Parameter v)
{
	setBroadcast(i,v);
	HK_ON_DEBUG( *(hkUint64*)&(m_quad.zw.m128d_f64[1]) = 0xffffffffffffffff; )
}

HK_FORCE_INLINE void hkVector4::setComponent(const int i, hkSimdRealParameter val)
{
	static HK_ALIGN16 (const hkUint64 indexToMask[4]) = 
	{
		0xffffffffffffffff, 0x0000000000000000,
		0x0000000000000000, 0xffffffffffffffff,
	};

	HK_ASSERT2(0x6d0c31d7, i>=0 && i<4, "index out of bounds for component access");

	if (i<2)
	{
		const hkSingleReal mask = *(const hkSingleReal*)&indexToMask[i*2];
#if HK_SSE_VERSION >= 0x41
		m_quad.xy = _mm_blendv_pd(m_quad.xy, val.m_real, mask);
#else
		m_quad.xy = _mm_or_pd( _mm_and_pd(mask, val.m_real), _mm_andnot_pd(mask, m_quad.xy) );
#endif
	}
	else
	{
		const hkSingleReal mask = *(const hkSingleReal*)&indexToMask[(i-2)*2];
#if HK_SSE_VERSION >= 0x41
		m_quad.zw = _mm_blendv_pd(m_quad.zw, val.m_real, mask);
#else
		m_quad.zw = _mm_or_pd( _mm_and_pd(mask, val.m_real), _mm_andnot_pd(mask, m_quad.zw) );
#endif
	}
}

template <>
HK_FORCE_INLINE void hkVector4::setComponent<0>(hkSimdRealParameter val)
{
	m_quad.xy = _mm_move_sd( m_quad.xy, val.m_real );
}
template <>
HK_FORCE_INLINE void hkVector4::setComponent<1>(hkSimdRealParameter val)
{
	m_quad.xy = _mm_move_sd( val.m_real, m_quad.xy );
}
template <>
HK_FORCE_INLINE void hkVector4::setComponent<2>(hkSimdRealParameter val)
{
	m_quad.zw = _mm_move_sd( m_quad.zw, val.m_real);
}
template <>
HK_FORCE_INLINE void hkVector4::setComponent<3>(hkSimdRealParameter val)
{
	m_quad.zw = _mm_move_sd( val.m_real, m_quad.zw);
}
template <int I>
HK_FORCE_INLINE void hkVector4::setComponent(hkSimdRealParameter val)
{
	HK_VECTOR4_NOT_IMPLEMENTED;
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMax<1>() const
{
	return getComponent<0>();
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMax<2>() const
{
	return hkSimdReal::convert(_mm_max_pd( _mm_shuffle_pd(m_quad.xy,m_quad.xy,_MM_SHUFFLE2(0,1)), m_quad.xy));
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMax<3>() const
{
	const hkSingleReal xy = _mm_max_pd( _mm_shuffle_pd(m_quad.xy,m_quad.xy,_MM_SHUFFLE2(0,1)), m_quad.xy);
	return hkSimdReal::convert(_mm_max_pd( _mm_shuffle_pd(m_quad.zw,m_quad.zw,_MM_SHUFFLE2(0,0)), xy));
}

template <>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMax<4>() const
{
	const hkSingleReal sum0 = _mm_max_pd( _mm_shuffle_pd( m_quad.xy, m_quad.xy,_MM_SHUFFLE2(0,1)), m_quad.xy); // yx+xy = xy xy
	const hkSingleReal sum1 = _mm_max_pd( _mm_shuffle_pd( m_quad.zw, m_quad.zw,_MM_SHUFFLE2(0,1)), m_quad.zw); // wz+zw = zw zw
	return hkSimdReal::convert(_mm_max_pd( sum0, sum1 )); // xywz all 4
}

template <int N>
HK_FORCE_INLINE const hkSimdReal hkVector4::horizontalMax() const
{
	HK_VECTOR4_NOT_IMPLEMENTED;
	return hkSimdReal::getConstant<HK_QUADREAL_0>();
}


template <>
HK_FORCE_INLINE void hkVector4::loadNotAligned<4>(const hkReal* p)
{
#if HK_SSE_VERSION >= 0x30
	m_quad.xy = _mm_castsi128_pd(_mm_lddqu_si128((const __m128i*)p));
	m_quad.zw = _mm_castsi128_pd(_mm_lddqu_si128((const __m128i*)(p+2)));
#else
	m_quad.xy = _mm_loadu_pd(p);
	m_quad.zw = _mm_loadu_pd(p+2);
#endif
}

template <>
HK_FORCE_INLINE void hkVector4::loadNotAligned<3>(const hkReal* p)
{
	m_quad.xy = _mm_set_pd(p[1], p[0]);
	m_quad.zw = _mm_set_pd(0.0 , p[2]);
	HK_ON_DEBUG( *(hkUint64*)&(m_quad.zw.m128d_f64[1]) = 0xffffffffffffffff; )
}

template <>
HK_FORCE_INLINE void hkVector4::loadNotAligned<2>(const hkReal* p)
{
	m_quad.xy = _mm_set_pd(p[1], p[0]);
	HK_ON_DEBUG( *(hkUint64*)&(m_quad.zw.m128d_f64[0]) = 0xffffffffffffffff; *(hkUint64*)&(m_quad.zw.m128d_f64[1]) = 0xffffffffffffffff; )
}

template <>
HK_FORCE_INLINE void hkVector4::loadNotAligned<1>(const hkReal* p)
{
	m_quad.xy = _mm_load_sd(p);
	HK_ON_DEBUG( *(hkUint64*)&(m_quad.xy.m128d_f64[1]) = 0xffffffffffffffff; )
	HK_ON_DEBUG( *(hkUint64*)&(m_quad.zw.m128d_f64[0]) = 0xffffffffffffffff; *(hkUint64*)&(m_quad.zw.m128d_f64[1]) = 0xffffffffffffffff; )
}

template <int N>
HK_FORCE_INLINE void hkVector4::loadNotAligned(const hkReal* p)
{
	HK_VECTOR4_NOT_IMPLEMENTED;
}

template <>
HK_FORCE_INLINE void hkVector4::loadNotAligned<4>(const hkHalf* p)
{
	__m128i fourhalfs = _mm_loadl_epi64((const __m128i*)p);
	__m128  fourfloats = _mm_castsi128_ps( _mm_unpacklo_epi16(_mm_setzero_si128(), fourhalfs) );
	m_quad.xy = _mm_cvtps_pd(fourfloats);
	m_quad.zw = _mm_cvtps_pd(_mm_shuffle_ps(fourfloats,fourfloats,_MM_SHUFFLE(0,1,2,3)));
}

template <>
HK_FORCE_INLINE void hkVector4::loadNotAligned<2>(const hkHalf* p)
{
	__m128i twohalfs = _mm_castps_si128( _mm_load_ss((const float*)p) );
	__m128  twofloats = _mm_castsi128_ps( _mm_unpacklo_epi16(_mm_setzero_si128(), twohalfs) );
	m_quad.xy = _mm_cvtps_pd(twofloats);
	HK_ON_DEBUG( *(hkUint64*)&(m_quad.zw.m128d_f64[0]) = 0xffffffffffffffff; *(hkUint64*)&(m_quad.zw.m128d_f64[1]) = 0xffffffffffffffff; )
}

template <int N>
HK_FORCE_INLINE void hkVector4::loadNotAligned(const hkHalf* p)
{
	HK_VECTOR4_NOT_IMPLEMENTED;
}

template <>
HK_FORCE_INLINE void hkVector4::storeNotAligned<4>(hkReal* p) const
{
	_mm_storeu_pd(p,   m_quad.xy);
	_mm_storeu_pd(p+2, m_quad.zw);
}

template <>
HK_FORCE_INLINE void hkVector4::storeNotAligned<3>(hkReal* p) const
{
	_mm_storeu_pd(p,  m_quad.xy);
	_mm_store_sd(p+2, m_quad.zw);
}

template <>
HK_FORCE_INLINE void hkVector4::storeNotAligned<2>(hkReal* p) const
{
	_mm_storeu_pd(p,  m_quad.xy);
}

template <>
HK_FORCE_INLINE void hkVector4::storeNotAligned<1>(hkReal* p) const
{
	_mm_store_sd(p, m_quad.xy);
}

template <int N>
HK_FORCE_INLINE void hkVector4::storeNotAligned(hkReal* p) const
{
	HK_VECTOR4_NOT_IMPLEMENTED;
}

template <>
HK_FORCE_INLINE void hkVector4::storeNotAligned<4>(hkHalf* p) const
{
	__m128 xy = _mm_cvtpd_ps(_mm_mul_pd(m_quad.xy,g_vectorConstants[HK_QUADREAL_PACK_HALF].xy));
	__m128 zw = _mm_cvtpd_ps(_mm_mul_pd(m_quad.zw,g_vectorConstants[HK_QUADREAL_PACK_HALF].xy));
	__m128 xyzw = _mm_shuffle_ps(xy,zw,_MM_SHUFFLE(1,0,1,0));
	__m128i tmp0 = _mm_srai_epi32( _mm_castps_si128(xyzw), 16 );
	__m128i tmp1 = _mm_packs_epi32(tmp0, tmp0);
	_mm_storel_epi64((__m128i*) p, tmp1);
}

template <>
HK_FORCE_INLINE void hkVector4::storeNotAligned<2>(hkHalf* p) const
{
	__m128 xy = _mm_cvtpd_ps(_mm_mul_pd(m_quad.xy,g_vectorConstants[HK_QUADREAL_PACK_HALF].xy));
	__m128i tmp0 = _mm_srai_epi32( _mm_castps_si128(xy), 16 );
	__m128i tmp1 = _mm_packs_epi32(tmp0, tmp0);
	_mm_store_ss((float*)p, _mm_castsi128_ps(tmp1));
}

template <int N>
HK_FORCE_INLINE void hkVector4::storeNotAligned(hkHalf* p) const
{
	HK_VECTOR4_NOT_IMPLEMENTED;
}

template <>
HK_FORCE_INLINE void hkVector4::storeNotAlignedNotRounded<4>(hkHalf* p) const
{
	__m128 xy = _mm_cvtpd_ps(m_quad.xy);
	__m128 zw = _mm_cvtpd_ps(m_quad.zw);
	__m128 xyzw = _mm_shuffle_ps(xy,zw,_MM_SHUFFLE(1,0,1,0));
	__m128i tmp0 = _mm_srai_epi32( _mm_castps_si128(xyzw), 16 );
	__m128i tmp1 = _mm_packs_epi32(tmp0, tmp0);
	_mm_storel_epi64((__m128i*) p, tmp1);
}

template <>
HK_FORCE_INLINE void hkVector4::storeNotAlignedNotRounded<2>(hkHalf* p) const
{
	__m128 xy = _mm_cvtpd_ps(m_quad.xy);
	__m128i tmp0 = _mm_srai_epi32( _mm_castps_si128(xy), 16 );
	__m128i tmp1 = _mm_packs_epi32(tmp0, tmp0);
	_mm_store_ss((float*)p, _mm_castsi128_ps(tmp1));
}

template <int N>
HK_FORCE_INLINE void hkVector4::storeNotAlignedNotRounded(hkHalf* p) const
{
	HK_VECTOR4_NOT_IMPLEMENTED;
}

HK_FORCE_INLINE void hkVector4::reduceToHalfPrecision()
{
	__m128i precisionMask = _mm_set1_epi32(0xffff0000);
	__m128 xy = _mm_cvtpd_ps(m_quad.xy);
	__m128 zw = _mm_cvtpd_ps(m_quad.zw);
	__m128 xyzw = _mm_shuffle_ps(xy,zw,_MM_SHUFFLE(1,0,1,0));
	xyzw = _mm_and_ps( xyzw, _mm_castsi128_ps(precisionMask) );
	m_quad.xy = _mm_cvtps_pd(xyzw);
	m_quad.zw = _mm_cvtps_pd(_mm_shuffle_ps(xyzw,xyzw,_MM_SHUFFLE(1,0,3,2)));
}

template <>
HK_FORCE_INLINE void hkVector4::load<4>(const hkReal* p)
{
	HK_ASSERT2(0x64211c2f, ( ((hkUlong)p) & 0x1f ) == 0, "p must be 32-byte aligned.");
	m_quad.xy = _mm_load_pd(p);
	m_quad.zw = _mm_load_pd(p+2);
}

template <int N>
HK_FORCE_INLINE void hkVector4::load(const hkReal* p)
{
	HK_ON_DEBUG(const hkUint32 dataSize = sizeof(hkReal)*(N!=3?N:4));
	HK_ASSERT2(0x64211c2f, ( ((hkUlong)p) & (dataSize-1) ) == 0, "p must be aligned.");
	loadNotAligned<N>(p);
}

template <>
HK_FORCE_INLINE void hkVector4::store<4>(hkReal* p) const
{
	HK_ASSERT2(0x64211c2f, ( ((hkUlong)p) & 0x1f ) == 0, "p must be 32-byte aligned.");
	_mm_store_pd(p,   m_quad.xy);
	_mm_store_pd(p+2, m_quad.zw);
}

template <int N>
HK_FORCE_INLINE void hkVector4::store(hkReal* p) const
{
	HK_ON_DEBUG(const hkUint32 dataSize = sizeof(hkReal)*(N!=3?N:4));
	HK_ASSERT2(0x64211c2f, ( ((hkUlong)p) & (dataSize-1) ) == 0, "p must be aligned.");
	storeNotAligned<N>(p);
}



template <> 
HK_FORCE_INLINE void hkVector4::storeNotCached<2>(hkReal* p) const
{
	HK_ASSERT2(0x64211c2f, ( ((hkUlong)p) & ((sizeof(hkReal)*2)-1) ) == 0, "p must be aligned.");
	_mm_stream_pd(p, m_quad.xy);
}

template <> 
HK_FORCE_INLINE void hkVector4::storeNotCached<4>(hkReal* p) const
{
	HK_ASSERT2(0x64211c2f, ( ((hkUlong)p) & 0x1f ) == 0, "p must be 32-byte aligned.");
	_mm_stream_pd(p,   m_quad.xy);
	_mm_stream_pd(p+2, m_quad.zw);
}

template <int N> 
HK_FORCE_INLINE void hkVector4::storeNotCached(hkReal* p) const
{
	HK_ON_DEBUG(const hkUint32 dataSize = sizeof(hkReal)*(N!=3?N:4));
	HK_ASSERT2(0x64211c2f, ( ((hkUlong)p) & (dataSize-1) ) == 0, "p must be aligned.");
	storeNotAligned<N>(p);
}



#if HK_SSE_VERSION >= 0x41
template <> 
HK_FORCE_INLINE void hkVector4::loadNotCached<4>(const hkReal* p)
{
	HK_ASSERT2(0x64211c2f, ( ((hkUlong)p) & 0x1f ) == 0, "p must be 32-byte aligned.");
	m_quad.xy = _mm_castsi128_pd(_mm_stream_load_si128((__m128i*) p));
	m_quad.zw = _mm_castsi128_pd(_mm_stream_load_si128((__m128i*) (p+2)));
}
#endif

template <int N> 
HK_FORCE_INLINE void hkVector4::loadNotCached(const hkReal* p)
{
	HK_ON_DEBUG(const hkUint32 dataSize = sizeof(hkReal)*(N!=3?N:4));
	HK_ASSERT2(0x64211c2f, ( ((hkUlong)p) & (dataSize-1) ) == 0, "p must be aligned.");
	loadNotAligned<N>(p);
}


template <> 
HK_FORCE_INLINE hkBool32 hkVector4::isOk<1>() const
{
	const hkSingleReal nanMask = _mm_cmpunord_pd(m_quad.xy, _mm_setzero_pd());
	return !(_mm_movemask_pd(nanMask) & 0x1);
}

template <> 
HK_FORCE_INLINE hkBool32 hkVector4::isOk<2>() const
{
	const hkSingleReal nanMask = _mm_cmpunord_pd(m_quad.xy, _mm_setzero_pd());
	return !_mm_movemask_pd(nanMask);
}

template <> 
HK_FORCE_INLINE hkBool32 hkVector4::isOk<3>() const
{
	const hkSingleReal zero = _mm_setzero_pd();
	const hkSingleReal nanMaskXY = _mm_cmpunord_pd(m_quad.xy, zero);
	const hkSingleReal nanMaskZW = _mm_cmpunord_pd(m_quad.zw, zero);
	return !(_mm_movemask_pd(nanMaskXY) || (_mm_movemask_pd(nanMaskZW)&0x1));
}

template <> 
HK_FORCE_INLINE hkBool32 hkVector4::isOk<4>() const
{
	const hkSingleReal zero = _mm_setzero_pd();
	const hkSingleReal nanMaskXY = _mm_cmpunord_pd(m_quad.xy, zero);
	const hkSingleReal nanMaskZW = _mm_cmpunord_pd(m_quad.zw, zero);
	return !(_mm_movemask_pd(nanMaskXY) || _mm_movemask_pd(nanMaskZW));
}

template <int N> 
HK_FORCE_INLINE hkBool32 hkVector4::isOk() const
{
	HK_VECTOR4_NOT_IMPLEMENTED;
	return false;
}

#if HK_SSE_VERSION >= 0x30
template <> 
HK_FORCE_INLINE void hkVector4::setPermutation<hkVectorPermutation::XXZZ>(hkVector4Parameter v)
{
	m_quad.xy = _mm_movedup_pd(v.m_quad.xy);
	m_quad.zw = _mm_movedup_pd(v.m_quad.zw);
}
#else
template <> 
HK_FORCE_INLINE void hkVector4::setPermutation<hkVectorPermutation::XXZZ>(hkVector4Parameter v)
{
	m_quad.xy = _mm_unpacklo_pd(v.m_quad.xy,v.m_quad.xy);
	m_quad.zw = _mm_unpacklo_pd(v.m_quad.zw,v.m_quad.zw);
}
#endif

template <> 
HK_FORCE_INLINE void hkVector4::setPermutation<hkVectorPermutation::YYWW>(hkVector4Parameter v)
{
	m_quad.xy = _mm_unpackhi_pd(v.m_quad.xy,v.m_quad.xy);
	m_quad.zw = _mm_unpackhi_pd(v.m_quad.zw,v.m_quad.zw);
}


template <> 
HK_FORCE_INLINE void hkVector4::setPermutation<hkVectorPermutation::XYZW>(hkVector4Parameter v)
{
	m_quad = v.m_quad;
}

template <hkVectorPermutation::Permutation P> 
HK_FORCE_INLINE void hkVector4::setPermutation(hkVector4Parameter v)
{
	const hkSimdReal X = v.getComponent<(P>>12)&0x3>();
	const hkSimdReal Y = v.getComponent<(P>>8)&0x3>();
	const hkSimdReal Z = v.getComponent<(P>>4)&0x3>();
	const hkSimdReal W = v.getComponent<(P)&0x3>();

	m_quad.xy = _mm_move_sd(Y.m_real, X.m_real);
	m_quad.zw = _mm_move_sd(W.m_real, Z.m_real);
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::signBitSet() const
{
	static HK_ALIGN16( const hkUint64 signmask[2] ) = { 0x8000000000000000, 0x8000000000000000 };
	const hkSingleReal signsSetXY = _mm_and_pd(m_quad.xy, *(const hkSingleReal*)&signmask);
	const hkSingleReal signsSetZW = _mm_and_pd(m_quad.zw, *(const hkSingleReal*)&signmask);
	hkVector4Comparison mask;
#if HK_SSE_VERSION >= 0x41
	mask.m_mask.xy = _mm_castsi128_pd(_mm_cmpeq_epi64(_mm_castpd_si128(signsSetXY), *(const __m128i*)&signmask));
	mask.m_mask.zw = _mm_castsi128_pd(_mm_cmpeq_epi64(_mm_castpd_si128(signsSetZW), *(const __m128i*)&signmask));
#else
	__m128i maskXY = _mm_cmpeq_epi32(_mm_castpd_si128(signsSetXY), *(const __m128i*)&signmask);
	__m128i maskZW = _mm_cmpeq_epi32(_mm_castpd_si128(signsSetZW), *(const __m128i*)&signmask);
	mask.m_mask.xy = _mm_castsi128_pd(_mm_shuffle_epi32(maskXY, _MM_SHUFFLE(3,3,1,1)));
	mask.m_mask.zw = _mm_castsi128_pd(_mm_shuffle_epi32(maskZW, _MM_SHUFFLE(3,3,1,1)));
#endif
	return mask;
}

HK_FORCE_INLINE const hkVector4Comparison hkVector4::signBitClear() const
{
	static HK_ALIGN16( const hkUint64 signmask[2] ) = { 0x7fffffffffffffff, 0x7fffffffffffffff };
	const hkSingleReal signsSetXY = _mm_and_pd(m_quad.xy, *(const hkSingleReal*)&signmask);
	const hkSingleReal signsSetZW = _mm_and_pd(m_quad.zw, *(const hkSingleReal*)&signmask);
	hkVector4Comparison mask;
#if HK_SSE_VERSION >= 0x41
	mask.m_mask.xy = _mm_castsi128_pd(_mm_cmpeq_epi64(_mm_castpd_si128(signsSetXY), _mm_castpd_si128(m_quad.xy)));
	mask.m_mask.zw = _mm_castsi128_pd(_mm_cmpeq_epi64(_mm_castpd_si128(signsSetZW), _mm_castpd_si128(m_quad.zw)));
#else
	__m128i maskXY = _mm_cmpeq_epi32(_mm_castpd_si128(signsSetXY), _mm_castpd_si128(m_quad.xy));
	__m128i maskZW = _mm_cmpeq_epi32(_mm_castpd_si128(signsSetZW), _mm_castpd_si128(m_quad.zw));
	mask.m_mask.xy = _mm_castsi128_pd(_mm_shuffle_epi32(maskXY, _MM_SHUFFLE(3,3,1,1)));
	mask.m_mask.zw = _mm_castsi128_pd(_mm_shuffle_epi32(maskZW, _MM_SHUFFLE(3,3,1,1)));
#endif
	return mask;
}

HK_FORCE_INLINE void hkVector4::setFlipSign(hkVector4Parameter v, hkVector4ComparisonParameter mask)
{
	static HK_ALIGN16( const hkUint64 signmask[2] ) = { 0x8000000000000000, 0x8000000000000000 };
	m_quad.xy = _mm_xor_pd(v.m_quad.xy, _mm_and_pd(mask.m_mask.xy, *(const hkSingleReal*)&signmask));
	m_quad.zw = _mm_xor_pd(v.m_quad.zw, _mm_and_pd(mask.m_mask.zw, *(const hkSingleReal*)&signmask));
}

HK_FORCE_INLINE void hkVector4::setFlipSign(hkVector4Parameter v, hkVector4Parameter vSign)
{
	static HK_ALIGN16( const hkUint64 signmask[2] ) = { 0x8000000000000000, 0x8000000000000000 };
	m_quad.xy = _mm_xor_pd(v.m_quad.xy, _mm_and_pd(vSign.m_quad.xy, *(const hkSingleReal*)&signmask));
	m_quad.zw = _mm_xor_pd(v.m_quad.zw, _mm_and_pd(vSign.m_quad.zw, *(const hkSingleReal*)&signmask));
}

HK_FORCE_INLINE void hkVector4::setFlipSign(hkVector4Parameter v, hkSimdRealParameter sSign)
{
	static HK_ALIGN16( const hkUint64 signmask[2] ) = { 0x8000000000000000, 0x8000000000000000 };
	m_quad.xy = _mm_xor_pd(v.m_quad.xy, _mm_and_pd(sSign.m_real, *(const hkSingleReal*)&signmask));
	m_quad.zw = _mm_xor_pd(v.m_quad.zw, _mm_and_pd(sSign.m_real, *(const hkSingleReal*)&signmask));
}



//
// advanced interface
//

namespace hkVector4_AdvancedInterface
{

template <hkMathAccuracyMode A, hkMathDivByZeroMode D>
struct unroll_setReciprocal { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	HK_VECTOR4_TEMPLATE_CONFIG_NOT_IMPLEMENTED;
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_IGNORE> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	switch (A)
	{
		case HK_ACC_23_BIT: 
			{
				self = hkMath::quadReciprocal(a.m_quad); 
			}
			break;
		case HK_ACC_12_BIT: 
			{
				const __m128 xy = _mm_cvtpd_ps(a.m_quad.xy);
				const __m128 zw = _mm_cvtpd_ps(a.m_quad.zw);
				const __m128 xyzw = _mm_shuffle_ps(xy,zw,_MM_SHUFFLE(1,0,1,0));
				const __m128 re = _mm_rcp_ps(xyzw);
				self.xy = _mm_cvtps_pd(re);
				self.zw = _mm_cvtps_pd(_mm_shuffle_ps(re,re,_MM_SHUFFLE(1,0,3,2)));
			}
			break;
		default:         
			{
				self.xy = _mm_div_pd(g_vectorConstants[HK_QUADREAL_1].xy, a.m_quad.xy);
				self.zw = _mm_div_pd(g_vectorConstants[HK_QUADREAL_1].xy, a.m_quad.zw);
			}
			break; // HK_ACC_FULL
	}
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_SET_ZERO> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	const __m128d equalsZeroXY = _mm_cmpeq_pd(a.m_quad.xy, _mm_setzero_pd());
	const __m128d equalsZeroZW = _mm_cmpeq_pd(a.m_quad.zw, _mm_setzero_pd());
	hkQuadReal e; unroll_setReciprocal<A, HK_DIV_IGNORE>::apply(e, a);
	self.xy = _mm_andnot_pd(equalsZeroXY, e.xy);
	self.zw = _mm_andnot_pd(equalsZeroZW, e.zw);
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_SET_HIGH> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	const __m128d equalsZeroXY = _mm_cmpeq_pd(a.m_quad.xy, _mm_setzero_pd());
	const __m128d equalsZeroZW = _mm_cmpeq_pd(a.m_quad.zw, _mm_setzero_pd());
	hkQuadReal e; unroll_setReciprocal<A, HK_DIV_IGNORE>::apply(e, a);
	const __m128d huge = _mm_set1_pd(HK_REAL_HIGH);
#if HK_SSE_VERSION >= 0x41
	self.xy = _mm_blendv_pd(e.xy, huge, equalsZeroXY);
	self.zw = _mm_blendv_pd(e.zw, huge, equalsZeroZW);
#else
	self.xy = _mm_or_pd( _mm_and_pd(equalsZeroXY, huge), _mm_andnot_pd(equalsZeroXY, e.xy) );
	self.zw = _mm_or_pd( _mm_and_pd(equalsZeroZW, huge), _mm_andnot_pd(equalsZeroZW, e.zw) );
#endif
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_SET_MAX> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	const __m128d equalsZeroXY = _mm_cmpeq_pd(a.m_quad.xy, _mm_setzero_pd());
	const __m128d equalsZeroZW = _mm_cmpeq_pd(a.m_quad.zw, _mm_setzero_pd());
	hkQuadReal e; unroll_setReciprocal<A, HK_DIV_IGNORE>::apply(e, a);
	const __m128d huge = _mm_set1_pd(HK_REAL_MAX);
#if HK_SSE_VERSION >= 0x41
	self.xy = _mm_blendv_pd(e.xy, huge, equalsZeroXY);
	self.zw = _mm_blendv_pd(e.zw, huge, equalsZeroZW);
#else
	self.xy = _mm_or_pd( _mm_and_pd(equalsZeroXY, huge), _mm_andnot_pd(equalsZeroXY, e.xy) );
	self.zw = _mm_or_pd( _mm_and_pd(equalsZeroZW, huge), _mm_andnot_pd(equalsZeroZW, e.zw) );
#endif
} };
template <hkMathAccuracyMode A>
struct unroll_setReciprocal<A, HK_DIV_SET_ZERO_AND_ONE> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	unroll_setReciprocal<A, HK_DIV_SET_ZERO>::apply(self, a);
	const __m128d one = g_vectorConstants[HK_QUADREAL_1].xy;
	const __m128d eps = g_vectorConstants[HK_QUADREAL_EPS].xy;
	const hkQuadReal absVal = hkMath::quadFabs(self);
	const __m128d absValLessOneXY = _mm_sub_pd(absVal.xy, one);
	const __m128d absValLessOneZW = _mm_sub_pd(absVal.zw, one);
	const __m128d lessEqualEpsXY = _mm_cmple_pd(absValLessOneXY, eps);
	const __m128d lessEqualEpsZW = _mm_cmple_pd(absValLessOneZW, eps);
#if HK_SSE_VERSION >= 0x41
	self.xy = _mm_blendv_pd(self.xy, one, lessEqualEpsXY);
	self.zw = _mm_blendv_pd(self.zw, one, lessEqualEpsZW);
#else
	self.xy = _mm_or_pd( _mm_and_pd(lessEqualEpsXY, one), _mm_andnot_pd(lessEqualEpsXY, self.xy) );
	self.zw = _mm_or_pd( _mm_and_pd(lessEqualEpsZW, one), _mm_andnot_pd(lessEqualEpsZW, self.zw) );
#endif
} };

} // namespace 

template <hkMathAccuracyMode A, hkMathDivByZeroMode D>
HK_FORCE_INLINE void hkVector4::setReciprocal(hkVector4Parameter a)
{
	hkVector4_AdvancedInterface::unroll_setReciprocal<A,D>::apply(m_quad,a);
}

HK_FORCE_INLINE void hkVector4::setReciprocal(hkVector4Parameter a)
{
	hkVector4_AdvancedInterface::unroll_setReciprocal<HK_ACC_23_BIT,HK_DIV_IGNORE>::apply(m_quad,a);
}



namespace hkVector4_AdvancedInterface
{

template <hkMathAccuracyMode A, hkMathDivByZeroMode D>
struct unroll_setDiv { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a, hkVector4Parameter b)
{
	HK_VECTOR4_TEMPLATE_CONFIG_NOT_IMPLEMENTED;
} };
template <hkMathAccuracyMode A>
struct unroll_setDiv<A, HK_DIV_IGNORE> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a, hkVector4Parameter b)
{
	switch (A)
	{
		case HK_ACC_23_BIT: 
			{
				const hkQuadReal re = hkMath::quadReciprocal(b.m_quad); 
				self.xy = _mm_mul_pd(a.m_quad.xy,re.xy); 
				self.zw = _mm_mul_pd(a.m_quad.zw,re.zw); 
			}
			break;
		case HK_ACC_12_BIT: 
			{
				const __m128 xy = _mm_cvtpd_ps(b.m_quad.xy);
				const __m128 zw = _mm_cvtpd_ps(b.m_quad.zw);
				const __m128 xyzw = _mm_shuffle_ps(xy,zw,_MM_SHUFFLE(1,0,1,0));
				const __m128 re = _mm_rcp_ps(xyzw);
				self.xy = _mm_mul_pd(a.m_quad.xy,_mm_cvtps_pd(re));
				self.zw = _mm_mul_pd(a.m_quad.zw,_mm_cvtps_pd(_mm_shuffle_ps(re,re,_MM_SHUFFLE(1,0,3,2))));
			}
			break;
		default:         
			{
				self.xy = _mm_div_pd(a.m_quad.xy, b.m_quad.xy);
				self.zw = _mm_div_pd(a.m_quad.zw, b.m_quad.zw);
			}
			break; // HK_ACC_FULL
	}
} };
template <hkMathAccuracyMode A>
struct unroll_setDiv<A, HK_DIV_SET_ZERO> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a, hkVector4Parameter b)
{
	const __m128d equalsZeroXY = _mm_cmpeq_pd(b.m_quad.xy, _mm_setzero_pd());
	const __m128d equalsZeroZW = _mm_cmpeq_pd(b.m_quad.zw, _mm_setzero_pd());
	hkQuadReal e; unroll_setReciprocal<A, HK_DIV_IGNORE>::apply(e, b);
	self.xy = _mm_mul_pd(a.m_quad.xy,_mm_andnot_pd(equalsZeroXY, e.xy));
	self.zw = _mm_mul_pd(a.m_quad.zw,_mm_andnot_pd(equalsZeroZW, e.zw));
} };
template <hkMathAccuracyMode A>
struct unroll_setDiv<A, HK_DIV_SET_HIGH> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a, hkVector4Parameter b)
{
	const __m128d equalsZeroXY = _mm_cmpeq_pd(b.m_quad.xy, _mm_setzero_pd());
	const __m128d equalsZeroZW = _mm_cmpeq_pd(b.m_quad.zw, _mm_setzero_pd());
	hkQuadReal e; unroll_setReciprocal<A, HK_DIV_IGNORE>::apply(e, b);
	const __m128d huge = _mm_set1_pd(HK_REAL_HIGH);
	const __m128d valXY = _mm_mul_pd(a.m_quad.xy, e.xy);
	const __m128d valZW = _mm_mul_pd(a.m_quad.zw, e.zw);
#if HK_SSE_VERSION >= 0x41
	self.xy = _mm_blendv_pd(valXY, huge, equalsZeroXY);
	self.zw = _mm_blendv_pd(valZW, huge, equalsZeroZW);
#else
	self.xy = _mm_or_pd( _mm_and_pd(equalsZeroXY, huge), _mm_andnot_pd(equalsZeroXY, valXY) );
	self.zw = _mm_or_pd( _mm_and_pd(equalsZeroZW, huge), _mm_andnot_pd(equalsZeroZW, valZW) );
#endif
} };
template <hkMathAccuracyMode A>
struct unroll_setDiv<A, HK_DIV_SET_MAX> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a, hkVector4Parameter b)
{
	const __m128d equalsZeroXY = _mm_cmpeq_pd(b.m_quad.xy, _mm_setzero_pd());
	const __m128d equalsZeroZW = _mm_cmpeq_pd(b.m_quad.zw, _mm_setzero_pd());
	hkQuadReal e; unroll_setReciprocal<A, HK_DIV_IGNORE>::apply(e, b);
	const __m128d huge = _mm_set1_pd(HK_REAL_MAX);
	const __m128d valXY = _mm_mul_pd(a.m_quad.xy, e.xy);
	const __m128d valZW = _mm_mul_pd(a.m_quad.zw, e.zw);
#if HK_SSE_VERSION >= 0x41
	self.xy = _mm_blendv_pd(valXY, huge, equalsZeroXY);
	self.zw = _mm_blendv_pd(valZW, huge, equalsZeroZW);
#else
	self.xy = _mm_or_pd( _mm_and_pd(equalsZeroXY, huge), _mm_andnot_pd(equalsZeroXY, valXY) );
	self.zw = _mm_or_pd( _mm_and_pd(equalsZeroZW, huge), _mm_andnot_pd(equalsZeroZW, valZW) );
#endif
} };

} // namespace 

template <hkMathAccuracyMode A, hkMathDivByZeroMode D>
HK_FORCE_INLINE void hkVector4::setDiv(hkVector4Parameter v0, hkVector4Parameter v1)
{
	hkVector4_AdvancedInterface::unroll_setDiv<A,D>::apply(m_quad,v0,v1);
}

HK_FORCE_INLINE void hkVector4::setDiv(hkVector4Parameter v0, hkVector4Parameter v1)
{
	hkVector4_AdvancedInterface::unroll_setDiv<HK_ACC_23_BIT,HK_DIV_IGNORE>::apply(m_quad,v0,v1);
}

template <hkMathAccuracyMode A, hkMathDivByZeroMode D>
HK_FORCE_INLINE void hkVector4::div(hkVector4Parameter a)
{
	setDiv<A,D>( *this, a );
}

HK_FORCE_INLINE void hkVector4::div(hkVector4Parameter a)
{
	setDiv( *this, a );
}



namespace hkVector4_AdvancedInterface
{

template <hkMathAccuracyMode A, hkMathNegSqrtMode S>
struct unroll_setSqrt { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	HK_VECTOR4_TEMPLATE_CONFIG_NOT_IMPLEMENTED;
} };
template <hkMathAccuracyMode A>
struct unroll_setSqrt<A, HK_SQRT_IGNORE> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	switch (A)
	{
		case HK_ACC_23_BIT: 
			{
				const hkQuadReal re = hkMath::quadReciprocalSquareRoot(a.m_quad); 
				self.xy = _mm_mul_pd(a.m_quad.xy,re.xy); 
				self.zw = _mm_mul_pd(a.m_quad.zw,re.zw); 
			}
			break;
		case HK_ACC_12_BIT: 
			{
				const __m128 xy = _mm_cvtpd_ps(a.m_quad.xy);
				const __m128 zw = _mm_cvtpd_ps(a.m_quad.zw);
				const __m128 xyzw = _mm_shuffle_ps(xy,zw,_MM_SHUFFLE(1,0,1,0));
				const __m128 re = _mm_rsqrt_ps(xyzw);
				self.xy = _mm_mul_pd(a.m_quad.xy,_mm_cvtps_pd(re));
				self.zw = _mm_mul_pd(a.m_quad.zw,_mm_cvtps_pd(_mm_shuffle_ps(re,re,_MM_SHUFFLE(1,0,3,2))));
			}
			break;
		default:         
			{
				self.xy = _mm_sqrt_pd(a.m_quad.xy); 
				self.zw = _mm_sqrt_pd(a.m_quad.zw); 
			}
			break; // HK_ACC_FULL
	}
} };
template <hkMathAccuracyMode A>
struct unroll_setSqrt<A, HK_SQRT_SET_ZERO> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	const __m128d equalsZeroXY = _mm_cmple_pd(a.m_quad.xy, _mm_setzero_pd());
	const __m128d equalsZeroZW = _mm_cmple_pd(a.m_quad.zw, _mm_setzero_pd());
	hkQuadReal e; unroll_setSqrt<A, HK_SQRT_IGNORE>::apply(e,a);
	self.xy = _mm_andnot_pd(equalsZeroXY, e.xy);
	self.zw = _mm_andnot_pd(equalsZeroZW, e.zw);
} };

} // namespace 

template <hkMathAccuracyMode A, hkMathNegSqrtMode S>
HK_FORCE_INLINE void hkVector4::setSqrt(hkVector4Parameter a)
{
	hkVector4_AdvancedInterface::unroll_setSqrt<A,S>::apply(m_quad, a);
}

HK_FORCE_INLINE void hkVector4::setSqrt(hkVector4Parameter a)
{
	hkVector4_AdvancedInterface::unroll_setSqrt<HK_ACC_23_BIT,HK_SQRT_SET_ZERO>::apply(m_quad, a);
}



namespace hkVector4_AdvancedInterface
{

template <hkMathAccuracyMode A, hkMathNegSqrtMode S>
struct unroll_setSqrtInverse { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	HK_VECTOR4_TEMPLATE_CONFIG_NOT_IMPLEMENTED;
} };
template <hkMathAccuracyMode A>
struct unroll_setSqrtInverse<A, HK_SQRT_IGNORE> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	switch (A)
	{
		case HK_ACC_23_BIT: 
			{
				self = hkMath::quadReciprocalSquareRoot(a.m_quad); 
			}
			break;
		case HK_ACC_12_BIT: 
			{
				const __m128 xy = _mm_cvtpd_ps(a.m_quad.xy);
				const __m128 zw = _mm_cvtpd_ps(a.m_quad.zw);
				const __m128 xyzw = _mm_shuffle_ps(xy,zw,_MM_SHUFFLE(1,0,1,0));
				const __m128 re = _mm_rsqrt_ps(xyzw);
				self.xy = _mm_cvtps_pd(re);
				self.zw = _mm_cvtps_pd(_mm_shuffle_ps(re,re,_MM_SHUFFLE(1,0,3,2)));
			}
			break;
		default:         
			{
				self.xy = _mm_div_pd(g_vectorConstants[HK_QUADREAL_1].xy, _mm_sqrt_pd(a.m_quad.xy));
				self.zw = _mm_div_pd(g_vectorConstants[HK_QUADREAL_1].xy, _mm_sqrt_pd(a.m_quad.zw));
			}
			break; // HK_ACC_FULL
	}
} };
template <hkMathAccuracyMode A>
struct unroll_setSqrtInverse<A, HK_SQRT_SET_ZERO> { HK_FORCE_INLINE static void apply(hkQuadReal& self, hkVector4Parameter a)
{
	const __m128d equalsZeroXY = _mm_cmple_pd(a.m_quad.xy, _mm_setzero_pd());
	const __m128d equalsZeroZW = _mm_cmple_pd(a.m_quad.zw, _mm_setzero_pd());
	hkQuadReal e; unroll_setSqrtInverse<A, HK_SQRT_IGNORE>::apply(e,a);
	self.xy = _mm_andnot_pd(equalsZeroXY, e.xy);
	self.zw = _mm_andnot_pd(equalsZeroZW, e.zw);
} };
} // namespace 

template <hkMathAccuracyMode A, hkMathNegSqrtMode S>
HK_FORCE_INLINE void hkVector4::setSqrtInverse(hkVector4Parameter a)
{
	hkVector4_AdvancedInterface::unroll_setSqrtInverse<A,S>::apply(m_quad,a);
}

HK_FORCE_INLINE void hkVector4::setSqrtInverse(hkVector4Parameter a)
{
	hkVector4_AdvancedInterface::unroll_setSqrtInverse<HK_ACC_23_BIT,HK_SQRT_SET_ZERO>::apply(m_quad,a);
}

/* 
 * Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20110913) 
 *  
 * Confidential Information of Havok.  (C) Copyright 1999-2011 
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok 
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership 
 * rights, and intellectual property rights in the Havok software remain in 
 * Havok and/or its suppliers. 
 *  
 * Use of this software for evaluation purposes is subject to and indicates 
 * acceptance of the End User licence Agreement for this product. A copy of 
 * the license is included with this software and is also available at www.havok.com/tryhavok. 
 *  
 */
