/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's 
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok. 
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2011 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement. 
 */


#ifndef HK_DISABLE_MATH_CONSTRUCTORS
inline hkAabb::hkAabb(const hkVector4& min, const hkVector4& max)
	: m_min(min), m_max(max)
{
}
#endif

hkBool32 hkAabb::overlaps( const hkAabb& other ) const
{
	HK_ASSERT2(0x3f5578fc,  isValid() || isEmpty(), "Invalid AABB in hkAabb::overlaps." );
	HK_ASSERT2(0x76dd800a,  other.isValid() || other.isEmpty(), "Invalid AABB in hkAabb::overlaps.");
	hkVector4Comparison mincomp = m_min.lessEqual(other.m_max);
	hkVector4Comparison maxcomp = other.m_min.lessEqual(m_max);
	hkVector4Comparison both; both.setAnd( mincomp, maxcomp );
	return both.allAreSet(hkVector4Comparison::MASK_XYZ);
}


hkBool32 hkAabb::overlapsUpdateSmallestNonExtraHitAabb( const hkAabb& other, hkAabb& smallestNonHitAabbInOut ) const
{
	HK_ASSERT2(0x3f5578fc,  isValid(), "Invalid AABB in hkAabb::overlaps." );
	//HK_ASSERT2(0x76dd800a,  other.isValid(), "Invalid AABB in hkAabb::overlaps.");
	hkVector4Comparison mincomp = m_min.less(other.m_max);
	hkVector4Comparison maxcomp = other.m_min.less(m_max);

	hkVector4 newMax; newMax.setMin( smallestNonHitAabbInOut.m_max, other.m_min );
	hkVector4 newMin; newMin.setMax( smallestNonHitAabbInOut.m_min, other.m_max );

	hkVector4Comparison both; both.setAnd( mincomp, maxcomp );

	smallestNonHitAabbInOut.m_min.setSelect( mincomp, smallestNonHitAabbInOut.m_min, newMin );
	smallestNonHitAabbInOut.m_max.setSelect( maxcomp, smallestNonHitAabbInOut.m_max, newMax );

	return both.allAreSet(hkVector4Comparison::MASK_XYZ);
}


bool hkAabb::contains(const hkAabb& other) const
{
	hkVector4Comparison mincomp = m_min.lessEqual(other.m_min);
	hkVector4Comparison maxcomp = other.m_max.lessEqual(m_max);
	hkVector4Comparison both; both.setAnd( mincomp, maxcomp );
	return both.allAreSet(hkVector4Comparison::MASK_XYZ);
}

hkBool32 hkAabb::containsPoint(const hkVector4& other) const
{
	hkVector4Comparison mincomp = m_min.lessEqual(other);
	hkVector4Comparison maxcomp = other.lessEqual(m_max);
	hkVector4Comparison both; both.setAnd( mincomp, maxcomp );
	return both.allAreSet(hkVector4Comparison::MASK_XYZ);
}

bool hkAabb::isEmpty() const
{
	hkVector4Comparison comp = m_min.greaterEqual(m_max);
	return comp.anyIsSet(hkVector4Comparison::MASK_XYZ);
}

void hkAabb::includePoint (const hkVector4& point)
{
	m_min.setMin(m_min, point);
	m_max.setMax(m_max, point);
}

void hkAabb::includeAabb (const hkAabb& aabb)
{
	m_min.setMin(m_min, aabb.m_min);
	m_max.setMax(m_max, aabb.m_max);
}

void hkAabb::setFull()
{
	m_max = hkVector4::getConstant<HK_QUADREAL_MAX>();
	m_min.setNeg<4>(hkVector4::getConstant<HK_QUADREAL_MAX>());
}

void hkAabb::setEmpty()
{
	m_min = hkVector4::getConstant<HK_QUADREAL_MAX>();
	m_max.setNeg<4>( m_min );
}

void hkAabb::getCenter( hkVector4& c ) const
{
	hkVector4 g; g.setAdd( m_min, m_max );
	c.setMul( hkVector4::getConstant<HK_QUADREAL_INV_2>(), g);
}

void hkAabb::getHalfExtents( hkVector4& he ) const
{
	hkVector4 sub; sub.setSub( m_max, m_min );
	he.setMul( hkVector4::getConstant<HK_QUADREAL_INV_2>(), sub );
}

void hkAabb::getVertex(int index, hkVector4& vertex) const
{
	HK_ASSERT2(0xF7BD00CA, index >= 0 && index <= 7, "Index out-of-range");
	hkVector4Comparison	comp; comp.set((hkVector4Comparison::Mask)index);
	vertex.setSelect(comp, m_max, m_min);
}

void hkAabb::expandBy( hkSimdRealParameter exp )
{
	hkVector4 expansion; expansion.setAll(exp);
	m_max.add(expansion);
	m_min.sub(expansion);
}

void hkAabb::setExpandBy( const hkAabb& aabb, hkSimdRealParameter exp )
{
	hkVector4 expansion; expansion.setAll(exp);
	m_max.setAdd( aabb.m_max, expansion );
	m_min.setSub( aabb.m_min, expansion );
}


hkBool32 hkAabb::equals(const hkAabb& aabb) const
{
	hkVector4Comparison mi = aabb.m_min.equal(m_min);
	hkVector4Comparison ma = aabb.m_max.equal(m_max);
	hkVector4Comparison co; co.setAnd( mi, ma );
	return co.allAreSet( hkVector4Comparison::MASK_XYZ );
}

void hkAabb::setIntersection(const hkAabb& aabb0, const hkAabb& aabb1)
{
	m_min.setMax(aabb0.m_min, aabb1.m_min);
	m_max.setMin(aabb0.m_max, aabb1.m_max);
}

inline void hkAabbUint32::setInvalid()
{
	const int ma = 0x7fffffff;

	// SNC warning workaround
	hkUint32* minP = m_min;
	hkUint32* maxP = m_max;

	minP[0] = ma;
	minP[1] = ma;
	minP[2] = ma;
	minP[3] = 0;
	maxP[0] = 0;
	maxP[1] = 0;
	maxP[2] = 0;
	maxP[3] = 0;
}

inline void hkAabbUint32::setInvalidY()
{
	const int ma = 0x7fff0000;
	hkUint32* minP = m_min;
	hkUint32* maxP = m_max;

	minP[1] = ma;
	minP[2] = ma;
	minP[3] = 0;
	maxP[1] = 0;
	maxP[2] = 0;
}

void hkAabbUint32::operator=( const hkAabbUint32& other )
{
	hkMemUtil::memCpyOneAligned<sizeof(hkAabbUint32),16>( this, &other );
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
