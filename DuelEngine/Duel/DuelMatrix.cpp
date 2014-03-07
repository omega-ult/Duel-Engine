//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelMatrix.h"
#include "DuelMath.h"
#include "DuelVector.h"
#include "DuelPlane.h"
#include "DuelQuaternion.h"


namespace Duel {

	const DReal DMatrix3::EPSILON = (DReal)1e-06;
    const DMatrix3 DMatrix3::ZERO(0,0,0, 0,0,0, 0,0,0);
    const DMatrix3 DMatrix3::IDENTITY(1,0,0, 0,1,0, 0,0,1);


	

	DMatrix3::DMatrix3(DReal m00, DReal m01, DReal m02, DReal m10, DReal m11, DReal m12,
        DReal m20, DReal m21, DReal m22)
	{
		M[0][0] = m00;
		M[0][1] = m01;
		M[0][2] = m02;
		
		M[1][0] = m10;
		M[1][1] = m11;
		M[1][2] = m12;

		M[2][0] = m20;
		M[2][1] = m21;
		M[2][2] = m22;
	}


	DMatrix3	DMatrix3::operator +	(const DMatrix3& m)
	{
		DMatrix3 result;
		for(size_t iRow = 0; iRow < 3; ++iRow)
		{
			for(size_t iCol = 0; iCol < 3; ++iCol)
			{
				result.M[iRow][iCol] =
					M[iRow][iCol] +
					m.M[iRow][iCol];
			}
		}
		return result;
	}

	DMatrix3	DMatrix3::operator -	(const DMatrix3& m)
	{
		DMatrix3 result;
		for(size_t iRow = 0; iRow < 3; ++iRow)
		{
			for(size_t iCol = 0; iCol < 3; ++iCol)
			{
				result.M[iRow][iCol] =
					M[iRow][iCol] -
					m.M[iRow][iCol];
			}
		}
		return result;
	}

	DMatrix3	DMatrix3::operator *	(const DMatrix3&	rM)
	{
		DMatrix3 result;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				result.M[iRow][iCol] =
					M[iRow][0]*rM.M[0][iCol] +
					M[iRow][1]*rM.M[1][iCol] +
					M[iRow][2]*rM.M[2][iCol];
			}
		}
		return result;
// 		DMatrix3 retM;
// 		retM.M[0][0] = rM.M[0][0] * M[0][0] + rM.M[0][1] * M[1][0] + rM.M[0][2] * M[2][0];
// 		retM.M[0][1] = rM.M[0][0] * M[0][1] + rM.M[0][1] * M[1][1] + rM.M[0][2] * M[2][1];
// 		retM.M[0][2] = rM.M[0][0] * M[0][2] + rM.M[0][1] * M[1][2] + rM.M[0][2] * M[2][2];
// 
// 		retM.M[1][0] = rM.M[1][0] * M[0][0] + rM.M[1][1] * M[1][0] + rM.M[1][2] * M[2][0];
// 		retM.M[1][1] = rM.M[1][0] * M[0][1] + rM.M[1][1] * M[1][1] + rM.M[1][2] * M[2][1];
// 		retM.M[1][2] = rM.M[1][0] * M[0][2] + rM.M[1][1] * M[1][2] + rM.M[1][2] * M[2][2];
// 
// 		retM.M[2][0] = rM.M[2][0] * M[0][0] + rM.M[2][1] * M[1][0] + rM.M[2][2] * M[2][0];
// 		retM.M[2][1] = rM.M[2][0] * M[0][1] + rM.M[2][1] * M[1][1] + rM.M[2][2] * M[2][1];
// 		retM.M[2][2] = rM.M[2][0] * M[0][2] + rM.M[2][1] * M[1][2] + rM.M[2][2] * M[2][2];

	}


	DMatrix3	DMatrix3::operator *	(DReal scalar)
	{
		DMatrix3 result;
		for(size_t iRow = 0; iRow < 3; ++iRow)
		{
			for(size_t iCol = 0; iCol < 3; ++iCol)
			{
				result.M[iRow][iCol] =
					M[iRow][iCol] * scalar;
			}
		}
		return result;
	}

	DVector3 DMatrix3::operator * ( const DVector3& rV)
	{
		DVector3 result;
		for(size_t iCol = 0; iCol < 3; ++iCol)
		{
			result[iCol] = 
				M[0][iCol] * rV[0] +
				M[1][iCol] * rV[1] +
				M[2][iCol] * rV[2];
		}
		return result;
	}

	DVector3 operator*(const DVector3& rV, const DMatrix3& rM )
	{
		DVector3 retV;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			retV[iRow] =
				rV[0]*rM.M[0][iRow] +
				rV[1]*rM.M[1][iRow] +
				rV[2]*rM.M[2][iRow];
		}
		return retV;
	}


	bool DMatrix3::operator==( const DMatrix3& rM )
	{
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				if ( M[iRow][iCol] != rM.M[iRow][iCol] )
					return false;
			}
		}

		return true;
	}

	DMatrix3 DMatrix3::getTranspose() const
	{
		return DMatrix3(M[0][0], M[1][0], M[2][0],
					 M[0][1], M[1][1], M[2][1],
					 M[0][2], M[1][2], M[2][2]);
	}

	DMatrix3 DMatrix3::getInverse() const
	{
		DMatrix3 retM;
		// make the outMat be an adjoint matrix of the matrix
		retM[0][0] = M[1][1]*M[2][2] - M[1][2]*M[2][1];
		retM[0][1] = M[0][2]*M[2][1] - M[0][1]*M[2][2];
		retM[0][2] = M[0][1]*M[1][2] - M[0][2]*M[1][1];
		retM[1][0] = M[1][2]*M[2][0] - M[1][0]*M[2][2];
		retM[1][1] = M[0][0]*M[2][2] - M[0][2]*M[2][0];
		retM[1][2] = M[0][2]*M[1][0] - M[0][0]*M[1][2];
		retM[2][0] = M[1][0]*M[2][1] - M[1][1]*M[2][0];
		retM[2][1] = M[0][1]*M[2][0] - M[0][0]*M[2][1];
		retM[2][2] = M[0][0]*M[1][1] - M[0][1]*M[1][0];

		DReal det = M[0][0]*retM[0][0] + 
				   M[0][1]*retM[0][1] + 
				   M[0][2]*retM[0][2];
		if(DMath::realEqual(det,0.0f))
		{
			return DMatrix3::IDENTITY;
		}
		else
		{
			for(int iRow = 0; iRow < 3; ++iRow)
			{
				for(int iCol = 0; iCol < 3; ++iCol)
				{
					retM[iRow][iCol] /= det;
				}
			}
			return retM;
		}

	}

	DReal DMatrix3::getDeterminant() const
	{
		DReal d00 = M[1][1]*M[2][2] - M[1][2]*M[2][1];
		DReal d01 = M[1][2]*M[2][0] - M[1][0]*M[2][2];
		DReal d02 = M[1][0]*M[2][1] - M[1][1]*M[2][0];

		return M[0][0]*d00 + M[0][1]*d01 + M[0][2]*d02;
	}

	DMatrix3 DMatrix3::getAdjoint() const
	{
		DMatrix3 retM;
		retM[0][0] = M[1][1]*M[2][2] - M[1][2]*M[2][1];
		retM[0][1] = M[0][2]*M[2][1] - M[0][1]*M[2][2];
		retM[0][2] = M[0][1]*M[1][2] - M[0][2]*M[1][1];
		retM[1][0] = M[1][2]*M[2][0] - M[1][0]*M[2][2];
		retM[1][1] = M[0][0]*M[2][2] - M[0][2]*M[2][0];
		retM[1][2] = M[0][2]*M[1][0] - M[0][0]*M[1][2];
		retM[2][0] = M[1][0]*M[2][1] - M[1][1]*M[2][0];
		retM[2][1] = M[0][1]*M[2][0] - M[0][0]*M[2][1];
		retM[2][2] = M[0][0]*M[1][1] - M[0][1]*M[1][0];
		return retM;
	}

	void DMatrix3::QDUDecomposition( DMatrix3& kQ, DVector3& kD, DVector3& kU) const
	{
		// Factor M = QR = QDU where Q is orthogonal, D is diagonal,
		// and U is upper triangular with ones on its diagonal.  Algorithm uses
		// Gram-Schmidt orthogonalization (the QR algorithm).
		//
		// If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
		//
		//   q0 = m0/|m0|
		//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
		//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
		//
		// where |V| indicates length of vector V and A*B indicates dot
		// product of vectors A and B.  The matrix R has entries
		//
		//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
		//   r10 = 0      r11 = q1*m1  r12 = q1*m2
		//   r20 = 0      r21 = 0      r22 = q2*m2
		//
		// so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
		// u02 = r02/r00, and u12 = r12/r11.

		// Q = rotation
		// D = scaling
		// U = shear

		// D stores the three diagonal entries r00, r11, r22
		// U stores the entries U[0] = u01, U[1] = u02, U[2] = u12

		// build orthogonal matrix Q
		DReal fInvLength = DMath::invSqrt(M[0][0]*M[0][0]
		+ M[1][0]*M[1][0] +
			M[2][0]*M[2][0]);
		kQ[0][0] = M[0][0]*fInvLength;
		kQ[1][0] = M[1][0]*fInvLength;
		kQ[2][0] = M[2][0]*fInvLength;

		DReal fDot = kQ[0][0]*M[0][1] + kQ[1][0]*M[1][1] +
			kQ[2][0]*M[2][1];
		kQ[0][1] = M[0][1]-fDot*kQ[0][0];
		kQ[1][1] = M[1][1]-fDot*kQ[1][0];
		kQ[2][1] = M[2][1]-fDot*kQ[2][0];
		fInvLength = DMath::invSqrt(kQ[0][1]*kQ[0][1] + kQ[1][1]*kQ[1][1] +
			kQ[2][1]*kQ[2][1]);
		kQ[0][1] *= fInvLength;
		kQ[1][1] *= fInvLength;
		kQ[2][1] *= fInvLength;

		fDot = kQ[0][0]*M[0][2] + kQ[1][0]*M[1][2] +
			kQ[2][0]*M[2][2];
		kQ[0][2] = M[0][2]-fDot*kQ[0][0];
		kQ[1][2] = M[1][2]-fDot*kQ[1][0];
		kQ[2][2] = M[2][2]-fDot*kQ[2][0];
		fDot = kQ[0][1]*M[0][2] + kQ[1][1]*M[1][2] +
			kQ[2][1]*M[2][2];
		kQ[0][2] -= fDot*kQ[0][1];
		kQ[1][2] -= fDot*kQ[1][1];
		kQ[2][2] -= fDot*kQ[2][1];
		fInvLength = DMath::invSqrt(kQ[0][2]*kQ[0][2] + kQ[1][2]*kQ[1][2] +
			kQ[2][2]*kQ[2][2]);
		kQ[0][2] *= fInvLength;
		kQ[1][2] *= fInvLength;
		kQ[2][2] *= fInvLength;

		// guarantee that orthogonal matrix has determinant 1 (no reflections)
		DReal fDet = kQ[0][0]*kQ[1][1]*kQ[2][2] + kQ[0][1]*kQ[1][2]*kQ[2][0] +
			kQ[0][2]*kQ[1][0]*kQ[2][1] - kQ[0][2]*kQ[1][1]*kQ[2][0] -
			kQ[0][1]*kQ[1][0]*kQ[2][2] - kQ[0][0]*kQ[1][2]*kQ[2][1];

		if ( fDet < 0.0 )
		{
			for (size_t iRow = 0; iRow < 3; iRow++)
				for (size_t iCol = 0; iCol < 3; iCol++)
					kQ[iRow][iCol] = -kQ[iRow][iCol];
		}

		// build "right" matrix R
		DMatrix3 kR;
		kR[0][0] = kQ[0][0]*M[0][0] + kQ[1][0]*M[1][0] +
			kQ[2][0]*M[2][0];
		kR[0][1] = kQ[0][0]*M[0][1] + kQ[1][0]*M[1][1] +
			kQ[2][0]*M[2][1];
		kR[1][1] = kQ[0][1]*M[0][1] + kQ[1][1]*M[1][1] +
			kQ[2][1]*M[2][1];
		kR[0][2] = kQ[0][0]*M[0][2] + kQ[1][0]*M[1][2] +
			kQ[2][0]*M[2][2];
		kR[1][2] = kQ[0][1]*M[0][2] + kQ[1][1]*M[1][2] +
			kQ[2][1]*M[2][2];
		kR[2][2] = kQ[0][2]*M[0][2] + kQ[1][2]*M[1][2] +
			kQ[2][2]*M[2][2];

		// the scaling component
		kD[0] = kR[0][0];
		kD[1] = kR[1][1];
		kD[2] = kR[2][2];

		// the shear component
		DReal fInvD0 = 1.0f/kD[0];
		kU[0] = kR[0][1]*fInvD0;
		kU[1] = kR[0][2]*fInvD0;
		kU[2] = kR[1][2]/kD[1];
	}



	// DMatrix4----------------------------------------------------
	const DMatrix4 DMatrix4::ZERO(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
	const DMatrix4 DMatrix4::IDENTITY(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

	

	DMatrix4::DMatrix4( DReal m00, DReal m01, DReal m02, DReal m03, DReal m10, DReal m11, DReal m12, DReal m13, DReal m20, DReal m21, DReal m22, DReal m23, DReal m30, DReal m31, DReal m32, DReal m33 )
	{
		M[0][0] = m00; M[0][1] = m01; M[0][2] = m02; M[0][3] = m03; 
		M[1][0] = m10; M[1][1] = m11; M[1][2] = m12; M[1][3] = m13; 
		M[2][0] = m20; M[2][1] = m21; M[2][2] = m22; M[2][3] = m23; 
		M[3][0] = m30; M[3][1] = m31; M[3][2] = m32; M[3][3] = m33; 
	}

	DMatrix4::DMatrix4( const DMatrix3& rM )
	{
		operator=(IDENTITY); 
		operator=(rM);
	}

	DMatrix4::DMatrix4( const DQuaternion& qRot )
	{
		DMatrix3	refM;
		qRot.toRotationMatrix(refM);
		operator=(IDENTITY);
		operator=(refM);
	}




	DMatrix4 DMatrix4::operator*( const DMatrix4& rM ) const
	{

		DMatrix4 result;
		for (size_t iRow = 0; iRow < 4; iRow++)
		{
			for (size_t iCol = 0; iCol < 4; iCol++)
			{
				result.M[iRow][iCol] =
					M[iRow][0]*rM.M[0][iCol] +
					M[iRow][1]*rM.M[1][iCol] +
					M[iRow][2]*rM.M[2][iCol] +
					M[iRow][3]*rM.M[3][iCol];
			}
		}
		return result;
	}

	DMatrix4 DMatrix4::operator*( DReal scalar ) const
	{
		DMatrix4 retM(M[0][0]*scalar, M[0][1]*scalar, M[0][2]*scalar, M[0][3]*scalar,
					 M[1][0]*scalar, M[1][1]*scalar, M[1][2]*scalar, M[1][3]*scalar,
					 M[2][0]*scalar, M[2][1]*scalar, M[2][2]*scalar, M[2][3]*scalar,
					 M[3][0]*scalar, M[3][1]*scalar, M[3][2]*scalar, M[3][3]*scalar);
		return	retM;
	}

	DVector3 DMatrix4::operator*( const DVector3& rV ) const
	{
		DVector3 r;

		DReal fInvW = 1.0f / ( M[0][3] * rV.x + M[1][3] * rV.y + M[2][3] * rV.z + M[3][3] );

		r.x = ( M[0][0] * rV.x + M[1][0] * rV.y + M[2][0] * rV.z + M[3][0] ) * fInvW;
		r.y = ( M[0][1] * rV.x + M[1][1] * rV.y + M[2][1] * rV.z + M[3][1] ) * fInvW;
		r.z = ( M[0][2] * rV.x + M[1][2] * rV.y + M[2][2] * rV.z + M[3][2] ) * fInvW;

		return r;
	}

	DVector4 DMatrix4::operator*( const DVector4& rV ) const
	{
		DVector4 result;
		for(size_t iCol = 0; iCol < 4; ++iCol)
		{
			result[iCol] = 
				M[0][iCol] * rV[0] +
				M[1][iCol] * rV[1] +
				M[2][iCol] * rV[2] +
				M[3][iCol] * rV[3];
		}
		return result;
	}

	DPlane DMatrix4::operator*( const DPlane& rP ) const
	{
		DPlane ret;
		DMatrix4	invTrans = getInverse().getTranpose();
		DVector4 v4(rP.normal.x, rP.normal.y, rP.normal.z, rP.d);
		v4 = invTrans * v4;
		ret.normal.x = v4.x;
		ret.normal.y = v4.y;
		ret.normal.z = v4.z;
		ret.d = v4.w / ret.normal.normalize();
		return ret;
	}






	DMatrix4 DMatrix4::operator+( const DMatrix4& rM ) const
	{
		DMatrix4 result;
		for(size_t iRow = 0; iRow < 4; ++iRow)
		{
			for(size_t iCol = 0; iCol < 4; ++iCol)
			{
				result.M[iRow][iCol] =
					M[iRow][iCol] +
					rM.M[iRow][iCol];
			}
		}
		return result;
// 		DMatrix4 retM;
// 
// 		retM.M[0][0] = M[0][0] + rM.M[0][0];
// 		retM.M[0][1] = M[0][1] + rM.M[0][1];
// 		retM.M[0][2] = M[0][2] + rM.M[0][2];
// 		retM.M[0][3] = M[0][3] + rM.M[0][3];
// 
// 		retM.M[1][0] = M[1][0] + rM.M[1][0];
// 		retM.M[1][1] = M[1][1] + rM.M[1][1];
// 		retM.M[1][2] = M[1][2] + rM.M[1][2];
// 		retM.M[1][3] = M[1][3] + rM.M[1][3];
// 
// 		retM.M[2][0] = M[2][0] + rM.M[2][0];
// 		retM.M[2][1] = M[2][1] + rM.M[2][1];
// 		retM.M[2][2] = M[2][2] + rM.M[2][2];
// 		retM.M[2][3] = M[2][3] + rM.M[2][3];
// 
// 		retM.M[3][0] = M[3][0] + rM.M[3][0];
// 		retM.M[3][1] = M[3][1] + rM.M[3][1];
// 		retM.M[3][2] = M[3][2] + rM.M[3][2];
// 		retM.M[3][3] = M[3][3] + rM.M[3][3];
// 
// 		return retM;
	}

	DMatrix4 DMatrix4::operator-( const DMatrix4& rM ) const
	{
		DMatrix4 result;
		for(size_t iRow = 0; iRow < 4; ++iRow)
		{
			for(size_t iCol = 0; iCol < 4; ++iCol)
			{
				result.M[iRow][iCol] =
					M[iRow][iCol] -
					rM.M[iRow][iCol];
			}
		}
		return result;
	}

	bool DMatrix4::operator==( const DMatrix4& rM ) const
	{
		if( M[0][0] != rM.M[0][0] || M[0][1] != rM.M[0][1] || M[0][2] != rM.M[0][2] || M[0][3] != rM.M[0][3] ||
			M[1][0] != rM.M[1][0] || M[1][1] != rM.M[1][1] || M[1][2] != rM.M[1][2] || M[1][3] != rM.M[1][3] ||
			M[2][0] != rM.M[2][0] || M[2][1] != rM.M[2][1] || M[2][2] != rM.M[2][2] || M[2][3] != rM.M[2][3] ||
			M[3][0] != rM.M[3][0] || M[3][1] != rM.M[3][1] || M[3][2] != rM.M[3][2] || M[3][3] != rM.M[3][3] )

		{
			return false;
		}
		return true;
	}




	DMatrix4 DMatrix4::getTranpose() const
	{
		return DMatrix4( M[0][0], M[1][0], M[2][0], M[3][0],
			M[0][1], M[1][1], M[2][1], M[3][1],
			M[0][2], M[1][2], M[2][2], M[3][2],
			M[0][3], M[1][3], M[2][3], M[3][3]);
	}


	// useful tool function to calculate adjoint------------------------------------------
	// only used in DMatrix4
	inline static DReal
		MINOR(const DMatrix4& rM, const size_t r0, const size_t r1, const size_t r2, 
		const size_t c0, const size_t c1, const size_t c2)
	{
		return rM[r0][c0] * (rM[r1][c1] * rM[r2][c2] - rM[r2][c1] * rM[r1][c2]) -
			rM[r0][c1] * (rM[r1][c0] * rM[r2][c2] - rM[r2][c0] * rM[r1][c2]) +
			rM[r0][c2] * (rM[r1][c0] * rM[r2][c1] - rM[r2][c0] * rM[r1][c1]);
	}


	bool DMatrix4::isAffine() const
	{
		if( M[0][3] == 0.0f && M[1][3] == 0.0f && M[2][3] == 0.0f && M[3][3] == 1.0f)
		{
			return true;
		}
		return false;
	}


	DMatrix4 DMatrix4::getInverse() const
	{
		DReal m00 = M[0][0], m01 = M[0][1], m02 = M[0][2], m03 = M[0][3];
		DReal m10 = M[1][0], m11 = M[1][1], m12 = M[1][2], m13 = M[1][3];
		DReal m20 = M[2][0], m21 = M[2][1], m22 = M[2][2], m23 = M[2][3];
		DReal m30 = M[3][0], m31 = M[3][1], m32 = M[3][2], m33 = M[3][3];

		DReal v0 = m20 * m31 - m21 * m30;
		DReal v1 = m20 * m32 - m22 * m30;
		DReal v2 = m20 * m33 - m23 * m30;
		DReal v3 = m21 * m32 - m22 * m31;
		DReal v4 = m21 * m33 - m23 * m31;
		DReal v5 = m22 * m33 - m23 * m32;

		DReal t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
		DReal t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
		DReal t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
		DReal t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

		DReal Det =  (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

		// invertible matrix
		if(Det == 0)
		{
			return DMatrix4::IDENTITY;
		}

		DReal invDet = 1 / Det;

		DReal d00 = t00 * invDet;
		DReal d10 = t10 * invDet;
		DReal d20 = t20 * invDet;
		DReal d30 = t30 * invDet;

		DReal d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		DReal d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		DReal d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		DReal d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;

		DReal d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		DReal d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		DReal d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		DReal d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;

		DReal d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		DReal d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		DReal d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		DReal d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		return DMatrix4(
			d00, d01, d02, d03,
			d10, d11, d12, d13,
			d20, d21, d22, d23,
			d30, d31, d32, d33);
	}


	DMatrix4 DMatrix4::getInverseAffine() const
	{
		// if matrix is affine it is probably not a invertible matrix
		assert(isAffine());

		DReal m10 = M[1][0], m11 = M[1][1], m12 = M[1][2];
		DReal m20 = M[2][0], m21 = M[2][1], m22 = M[2][2];

		DReal t00 = m22 * m11 - m21 * m12;
		DReal t10 = m20 * m12 - m22 * m10;
		DReal t20 = m21 * m10 - m20 * m11;

		DReal m00 = M[0][0], m01 = M[0][1], m02 = M[0][2];

		DReal invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

		t00 *= invDet; t10 *= invDet; t20 *= invDet;

		m00 *= invDet; m01 *= invDet; m02 *= invDet;

		DReal r00 = t00;
		DReal r01 = m02 * m21 - m01 * m22;
		DReal r02 = m01 * m12 - m02 * m11;

		DReal r10 = t10;
		DReal r11 = m00 * m22 - m02 * m20;
		DReal r12 = m02 * m10 - m00 * m12;

		DReal r20 = t20;
		DReal r21 = m01 * m20 - m00 * m21;
		DReal r22 = m00 * m11 - m01 * m10;

		DReal m03 = M[0][3], m13 = M[1][3], m23 = M[2][3];

		DReal r03 = - (r00 * m03 + r01 * m13 + r02 * m23);
		DReal r13 = - (r10 * m03 + r11 * m13 + r12 * m23);
		DReal r23 = - (r20 * m03 + r21 * m13 + r22 * m23);

		return DMatrix4(
			r00, r01, r02, r03,
			r10, r11, r12, r13,
			r20, r21, r22, r23,
			0,   0,   0,   1);
	}


	DMatrix4 DMatrix4::getAdjoint() const
	{
		return DMatrix4(MINOR(*this, 1, 2, 3, 1, 2, 3),
					  -MINOR(*this, 0, 2, 3, 1, 2, 3),
					   MINOR(*this, 0, 1, 3, 1, 2, 3),
					  -MINOR(*this, 0, 1, 2, 1, 2, 3), 
					  
					  -MINOR(*this, 1, 2, 3, 0, 2, 3),
					   MINOR(*this, 0, 2, 3, 0, 2, 3),
					  -MINOR(*this, 0, 1, 3, 0, 2, 3),
					   MINOR(*this, 0, 1, 2, 0, 2, 3),
					   
					   MINOR(*this, 1, 2, 3, 0, 1, 3),
					  -MINOR(*this, 0, 2, 3, 0, 1, 3),
					   MINOR(*this, 0, 1, 3, 0, 1, 3),
					  -MINOR(*this, 0, 1, 2, 0, 1, 3),
					  
					  -MINOR(*this, 1, 2, 3, 0, 1, 2),
					   MINOR(*this, 0, 2, 3, 0, 1, 2),
					  -MINOR(*this, 0, 1, 3, 0, 1, 2),
					   MINOR(*this, 0, 1, 2, 0, 1, 2));
	}


	DReal DMatrix4::getDeterminant() const
	{
		DReal d00 = M[1][1] * (M[2][2]*M[3][3] - M[2][3]*M[3][2]) +
				   M[1][2] * (M[2][3]*M[3][1] - M[2][1]*M[3][3]) + 
				   M[1][3] * (M[2][1]*M[3][2] - M[2][2]*M[3][1]);
		DReal d01 = M[1][0] * (M[2][2]*M[3][3] - M[2][3]*M[3][2]) +
				   M[1][2] * (M[2][3]*M[3][0] - M[2][0]*M[3][3]) +
				   M[1][3] * (M[2][0]*M[3][2] - M[2][2]*M[3][0]);
		DReal d02 = M[1][0] * (M[2][1]*M[3][3] - M[2][3]*M[3][1]) +
				   M[1][1] * (M[2][3]*M[3][0] - M[2][0]*M[3][3]) +
				   M[1][3] * (M[2][0]*M[3][1] - M[2][1]*M[3][0]);
		DReal d03 = M[1][0] * (M[2][1]*M[3][2] - M[2][2]*M[3][1]) +
				   M[1][1] * (M[2][2]*M[3][0] - M[2][0]*M[3][2]) +
				   M[1][2] * (M[2][0]*M[3][1] - M[2][1]*M[3][0]);
		return d00*M[0][0] - d01*M[0][1] + d02*M[0][2] - d03*M[0][3];

	}


	// TRANSLATION PART----------------------------------------------
	void DMatrix4::setTranslate( const DVector3& rV )
	{
		M[3][0] = rV.x;
		M[3][1] = rV.y;
		M[3][2] = rV.z;
	}

	Duel::DVector3 DMatrix4::getTranslate() const
	{
		return DVector3( M[3][0], M[3][1], M[3][2] );
	}

	void DMatrix4::makeTranslate( const DVector3& rV )
	{
		M[0][0] = 1.0; M[0][1] = 0.0; M[0][2] = 0.0; M[0][3] = 0.0;
		M[1][0] = 0.0; M[1][1] = 1.0; M[1][2] = 0.0; M[1][3] = 0.0;
		M[2][0] = 0.0; M[2][1] = 0.0; M[2][2] = 1.0; M[2][3] = 0.0;
		M[3][0] = rV.x; M[3][1] = rV.y; M[3][2] = rV.z; M[3][3] = 1.0;
	}


	Duel::DMatrix4 DMatrix4::getTranslate( const DVector3& rV )
	{
		DMatrix4 retM;
		retM.M[0][0] = 1.0; retM.M[0][1] = 0.0; retM.M[0][2] = 0.0; retM.M[0][3] = 0.0;
		retM.M[1][0] = 0.0; retM.M[1][1] = 1.0; retM.M[1][2] = 0.0; retM.M[1][3] = 0.0;
		retM.M[2][0] = 0.0; retM.M[2][1] = 0.0; retM.M[2][2] = 1.0; retM.M[2][3] = 0.0;
		retM.M[3][0] = rV.x; retM.M[3][1] = rV.y; retM.M[3][2] = rV.z; retM.M[3][3] = 1.0;
		return	retM;
	}

	Duel::DMatrix4 DMatrix4::getTranslate( DReal x, DReal y, DReal z )
	{
		DMatrix4 retM;
		retM.M[0][0] = 1.0; retM.M[0][1] = 0.0; retM.M[0][2] = 0.0; retM.M[0][3] = 0.0;
		retM.M[1][0] = 0.0; retM.M[1][1] = 1.0; retM.M[1][2] = 0.0; retM.M[1][3] = 0.0;
		retM.M[2][0] = 0.0; retM.M[2][1] = 0.0; retM.M[2][2] = 1.0; retM.M[2][3] = 0.0;
		retM.M[3][0] = x; retM.M[3][1] = y; retM.M[3][2] = z; retM.M[3][3] = 1.0;
		return	retM;
	}

	// SCALE PART----------------------------------------------
	void DMatrix4::setScale( const DVector3& rV )
	{
		M[0][0]	= rV.x;
		M[1][1]	= rV.y;
		M[2][2]	= rV.z;
	}

	Duel::DMatrix4 DMatrix4::getScale( const DVector3& rV )
	{
		DMatrix4 retM;
		retM.M[0][0] = rV.x; retM.M[0][1] = 0.0; retM.M[0][2] = 0.0; retM.M[0][3] = 0.0;
		retM.M[1][0] = 0.0; retM.M[1][1] = rV.y; retM.M[1][2] = 0.0; retM.M[1][3] = 0.0;
		retM.M[2][0] = 0.0; retM.M[2][1] = 0.0; retM.M[2][2] = rV.z; retM.M[2][3] = 0.0;
		retM.M[3][0] = 0.0; retM.M[3][1] = 0.0; retM.M[3][2] = 0.0; retM.M[3][3] = 1.0;
		return	retM;
	}

	Duel::DMatrix4 DMatrix4::getScale( DReal x, DReal y, DReal z )
	{
		DMatrix4 retM;
		retM.M[0][0] = x; retM.M[0][1] = 0.0; retM.M[0][2] = 0.0; retM.M[0][3] = 0.0;
		retM.M[1][0] = 0.0; retM.M[1][1] = y; retM.M[1][2] = 0.0; retM.M[1][3] = 0.0;
		retM.M[2][0] = 0.0; retM.M[2][1] = 0.0; retM.M[2][2] = z; retM.M[2][3] = 0.0;
		retM.M[3][0] = 0.0; retM.M[3][1] = 0.0; retM.M[3][2] = 0.0; retM.M[3][3] = 1.0;
		return	retM;
	}

	Duel::DMatrix3 DMatrix4::getMatrix3() const
	{
		DMatrix3	retM;
		retM.M[0][0] = M[0][0];	retM.M[0][1] = M[0][1];	retM.M[0][2] = M[0][2];	
		retM.M[1][0] = M[1][0]; retM.M[1][1] = M[1][1]; retM.M[1][2] = M[1][2];	
		retM.M[2][0] = M[2][0]; retM.M[2][1] = M[2][1]; retM.M[2][2] = M[2][2];
		return	retM;
	}

	Duel::DQuaternion DMatrix4::getQuaternion() const
	{
		return	DQuaternion(getMatrix3());
	}

	void DMatrix4::compose(const DVector3& translate,const DVector3& scale, const DQuaternion& orientation)
	{
		// Ordering:
		//    1. Scale
		//    2. Rotate
		//    3. Translate

		DMatrix3 rot;
		orientation.toRotationMatrix(rot);

		// Set up final matrix with scale, rotation and translation
		M[0][0] = scale.x * rot[0][0]; M[1][0] = scale.y * rot[1][0]; M[2][0] = scale.z * rot[2][0];
		M[0][1] = scale.x * rot[0][1]; M[1][1] = scale.y * rot[1][1]; M[2][1] = scale.z * rot[2][1];
		M[0][2] = scale.x * rot[0][2]; M[1][2] = scale.y * rot[1][2]; M[2][2] = scale.z * rot[2][2];
		M[3][0] = translate.x; 
		M[3][1] = translate.y; 
		M[3][2] = translate.z;
		// No projection term
		M[0][3] = 0; M[1][3] = 0; M[2][3] = 0; M[3][3] = 1;
	}

	void DMatrix4::composeInversed( const DVector3& translate,const DVector3& scale, const DQuaternion& orientation )
	{
		// Invert the parameters
		DVector3 invTranslate = -translate;
		DVector3 invScale(1 / scale.x, 1 / scale.y, 1 / scale.z);
		DQuaternion invRot = orientation.getConjugate();

		// Because we're inverting, order is translation, rotation, scale
		// So make translation relative to scale & rotation
		invTranslate = invRot * invTranslate; // rotate
		invTranslate *= invScale; // scale

		// Next, make a 3x3 rotation matrix
		DMatrix3 rot3x3;
		invRot.toRotationMatrix(rot3x3);

		// Set up final matrix with scale, rotation and translation
		M[0][0] = invScale.x * rot3x3[0][0]; M[1][0] = invScale.x * rot3x3[1][0]; M[2][0] = invScale.x * rot3x3[2][0]; 
		M[0][1] = invScale.y * rot3x3[0][1]; M[1][1] = invScale.y * rot3x3[1][1]; M[2][1] = invScale.y * rot3x3[2][1];
		M[0][2] = invScale.z * rot3x3[0][2]; M[1][2] = invScale.z * rot3x3[1][2]; M[2][2] = invScale.z * rot3x3[2][2]; 	
		M[3][0] = invTranslate.x; 
		M[3][1] = invTranslate.y;
		M[3][2] = invTranslate.z;	
		// No projection term
		M[0][3] = 0; M[1][3] = 0; M[2][3] = 0; M[3][3] = 1;

	}

	void DMatrix4::decompose( DVector3& translate,DVector3& scale, DQuaternion& orientation )
	{
		assert(isAffine());

		translate.x = M[3][0];
		translate.y = M[3][1];
		translate.z = M[3][2];

		DMatrix3 m3x3;
		DVector3 vecU;
		getMatrix3().QDUDecomposition(m3x3, scale, vecU);

		orientation.fromRotationMatrix(m3x3);
		
	}


	

}