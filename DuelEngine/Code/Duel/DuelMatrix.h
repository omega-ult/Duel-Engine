//  [7/15/2012 OMEGA] created

#ifndef	_DUELMATRIX_H_
#define _DUELMATRIX_H_

#include "DuelMath.h"
#include "DuelVector.h"
#include "DuelPlane.h"

namespace Duel {



	// pre-declaration
	class DQuaternion;
	

	// The (x,y,z) coordinate system is assumed to be LEFT-HANDED.  Coordinate
	
	//             (+)
	//              y|  (+)|far|
	//               |  / z
	//               | /
	//               |/
	//      (-)------#--------(+) 
	//              /|       x
	//             / |
	//            /  |
	//    |near|(-)  |
	//              (-)

	// ALL MATRIX USE COLUMN-MAJOR STYLE, (FOR EFFICIENCY?).
	
	// if there were two matrices: rotation R, and translation T,
	// assume we want to do: rotate then translate. the matrices should multiply
	// as R * T, that's identical to human's way of thinking. but when you 
	// want to apply it to a vector, use M * Vec to calculate the result.

	// the column-major matrix multiplication is done as:
	//                 | x |
	//                 | y |
	//                 | z |
	//                 | w |
	// | x1 x2 x3 xt | | x1*x + x2*y + x3*z + xt*w|
	// | y1 y2 y3 yt | | y1*x + y2*y + y3*z + yt*w|
	// | z1 z2 z3 zt | | z1*x + z2*y + z3*z + zt*w|
	// | 0  0  0  1  | | w|
	// 
	// BUT, in the memory, we use column-major storage, which means in the memory,
	// M[4][4] looks like:
	// { x1, y1, z1, 0, x2, y2, z2, 0, x3, y3, z3, 0, xt, yt, zt, 1}
	// it is identical to the row-major matrix with row-major storage.

	// axis rotation matrices are of the form (in memory)
	//   RX =    1       0       0
	//           0     cos(t)   sin(t)
	//           0    -sin(t)   cos(t)
	// where t > 0 indicates a clockwise rotation in the yz-plane
	//   RY =  cos(t)    0    -sin(t)
	//           0       1       0
	//         sin(t)    0     cos(t)
	// where t > 0 indicates a clockwise rotation in the zx-plane
	//   RZ =  cos(t)  sin(t)    0
	//        -sin(t)  cos(t)    0
	//           0       0       1
	// where t > 0 indicates a clockwise rotation in the xy-plane.


	class DUEL_API DMatrix3
	{
	public:
		// don't initialize for efficiency
		DMatrix3(){}
		~DMatrix3(){}

		
		inline DMatrix3( DReal m[3][3] )
		{
			memcpy(m,M,9*sizeof(DReal));
		}
		DMatrix3 (DReal m00, DReal m01, DReal m02, 
				 DReal m10, DReal m11, DReal m12,
				 DReal m20, DReal m21, DReal m22);

		// operator overload
		inline DMatrix3&	operator = (const DMatrix3& m)
		{
			memcpy(M,m.M,9*sizeof(DReal));
			return *this;
		}
		inline DMatrix3&	operator = (const DReal r[3][3])
		{
			memcpy(M,r,9*sizeof(DReal));
			return *this;
		}
		DMatrix3		operator +	(const DMatrix3& m);
		DMatrix3		operator -	(const DMatrix3&	m);
		DMatrix3		operator *	(const DMatrix3&	m);
		DMatrix3		operator *	(DReal scalar);

		
		// vector * matrix [1x3 * 3x3 = 1x3]
		DUEL_API friend DVector3 operator * (const DVector3& rV, const DMatrix3& rM);
		// matrix * vector [3x3 * 3x1 = 3x1]
		DVector3 operator * ( const DVector3& rV);
		


		bool operator == (const DMatrix3& rM);
		bool operator != (const DMatrix3& rM) { return !(operator==(rM)); }


		// member access, allows use of construct mat[r][c]
		inline DReal* operator[] (size_t iRow)
		{
			assert(iRow < 3);
			return (DReal*)M[iRow];
		}
        inline const DReal* operator[] (size_t iRow) const
		{
			assert(iRow < 3);
			return (DReal*)M[iRow];
		}


		DMatrix3		getTranspose() const;
		// if the matrix is invertible  the calculation will returns a Identity Matrix
		DMatrix3		getInverse() const;
		// get the matrix's adjoint
		DMatrix3		getAdjoint() const;
		DReal			getDeterminant() const;

		// orthogonal Q, diagonal D, upper triangular U stored as (u01,u02,u12)
		void			QDUDecomposition (DMatrix3& rkQ, DVector3& rkD, DVector3& rkU) const;

		inline void swap(DMatrix3& rM)
		{
			std::swap(M[0][0], rM[0][0]);
			std::swap(M[0][1], rM[0][1]);
			std::swap(M[0][2], rM[0][2]);
			std::swap(M[1][0], rM[1][0]);
			std::swap(M[1][1], rM[1][1]);
			std::swap(M[1][2], rM[1][2]);
			std::swap(M[2][0], rM[2][0]);
			std::swap(M[2][1], rM[2][1]);
			std::swap(M[2][2], rM[2][2]);
		}
		


		static const DReal EPSILON;
        static const DMatrix3 ZERO;
        static const DMatrix3 IDENTITY;


	protected:

		DReal	M[3][3];

		// for faster access
		friend class DMatrix4;

	};

	class DUEL_API DMatrix4
	{
	public:
		// do nothing for efficiency
		DMatrix4(){};

		inline DMatrix4( DReal m[4][4] )
		{
			memcpy(m,M,16*sizeof(DReal));
		}
		DMatrix4 (DReal m00, DReal m01, DReal m02, DReal m03,
				 DReal m10, DReal m11, DReal m12, DReal m13,
				 DReal m20, DReal m21, DReal m22, DReal m23,
				 DReal m30, DReal m31, DReal m32, DReal m33);

		DMatrix4(const DMatrix3& rM);

		// create a rotation matrix
		DMatrix4(const DQuaternion& qRot);


		// operator overload
		inline DMatrix4& operator = (const DMatrix4& rM)
		{
			memcpy(M, rM.M, 16*sizeof(DReal));
			return (*this);
		}
		inline DMatrix4& operator=( const DMatrix3& rM )
		{
			M[0][0] = rM.M[0][0];
			M[0][1] = rM.M[0][1];
			M[0][2] = rM.M[0][2];

			M[1][0] = rM.M[1][0];
			M[1][1] = rM.M[1][1];
			M[1][2] = rM.M[1][2];

			M[2][0] = rM.M[2][0];
			M[2][1] = rM.M[2][1];
			M[2][2] = rM.M[2][2];

			return *this;
		}
		DMatrix4		operator * (const DMatrix4& rM) const;
		DMatrix4		operator * (DReal scalar) const;

		DVector3		operator * (const DVector3& rV) const;
		DVector4		operator * (const DVector4& rV) const;
		DPlane		operator * (const DPlane& rP) const;

		DMatrix4 operator + (const DMatrix4& rM) const;
		DMatrix4 operator - (const DMatrix4& rM) const;
		

		bool	operator == (const DMatrix4& rM) const;
		bool	operator != (const DMatrix4& rM) const {	return !(operator==(rM)); }



		inline DReal*	operator [] (size_t iRow)
		{
			assert(iRow < 4);
			return M[iRow];
		}

		inline const DReal* operator [] (size_t iRow) const
		{
			assert(iRow < 4);
			return M[iRow];
		}


		// return a matrix's transpose
		DMatrix4			getTranpose() const;
		// whether the matrix is affine
		bool			isAffine() const;
		// if the matrix is invertible  the calculation will returns an Identity matrix;
		DMatrix4			getInverse() const;
		// affine inverse request that the matrix is affine matrix
		DMatrix4			getInverseAffine() const;
		// get the matrix's adjoint
		DMatrix4			getAdjoint() const;
		// determinant of the matrix
		DReal			getDeterminant() const;
		

		// TRANSLATION PART----------------------------------------------
		// set the matrix a translate matrix
		// notice this will only affect part of the matrix
		void			setTranslate(const DVector3& rV);

		// get translation included in the matrix
		DVector3			getTranslate() const;

		// make the matrix be a traslation matrix
		// using a traslation vector to indicate
		void			makeTranslate(const DVector3& rV);

		// get a traslation  matrix
		static DMatrix4	getTranslate(const DVector3& rV);
		static DMatrix4	getTranslate(DReal x, DReal y, DReal z);

		// SCALE PART----------------------------------------------
		// set the matrix a scale matrix
		// notice it will only affect part of the matrix
		void			setScale(const	DVector3& rV);

		// get a scale  matrix
		static DMatrix4	getScale(const DVector3& rV);
		static DMatrix4	getScale(DReal x, DReal y, DReal z);

		/** 3-D Vector transformation specially for an affine matrix.
            @remarks
                Transforms the given 3-D vector by the matrix, projecting the 
                result back into <i>w</i> = 1.
            @note
                The matrix must be an affine matrix. @see DMatrix4::isAffine.
        */
		inline DVector3 transformAffine(const DVector3& v) const
		{
			assert(isAffine());

			return DVector3(
				M[0][0] * v.x + M[0][1] * v.y + M[0][2] * v.z + M[0][3], 
				M[1][0] * v.x + M[1][1] * v.y + M[1][2] * v.z + M[1][3],
				M[2][0] * v.x + M[2][1] * v.y + M[2][2] * v.z + M[2][3]);
		}
		/** 4-D Vector transformation specially for an affine matrix.
            @note
                The matrix must be an affine matrix. @see DMatrix4::isAffine.
        */
        inline DVector4 transformAffine(const DVector4& v) const
        {
            assert(isAffine());

            return DVector4(
                M[0][0] * v.x + M[0][1] * v.y + M[0][2] * v.z + M[0][3] * v.w, 
                M[1][0] * v.x + M[1][1] * v.y + M[1][2] * v.z + M[1][3] * v.w,
                M[2][0] * v.x + M[2][1] * v.y + M[2][2] * v.z + M[2][3] * v.w,
                v.w);
        }

		// get the ratation and scaling part of the matrix
		// notice it will not always be valid rotation/scaling matrix
		DMatrix3			getMatrix3() const;
		// get a quaternion using its rotationg/scaling part
		DQuaternion		getQuaternion() const;
		// make a transform from given parameters.		
		// Ordering: Scale -> Rotate ->Translate
		void			compose(const DVector3& translate,const DVector3& scale, const DQuaternion& orientation);
		// make a inversed transformation matrix
		void			composeInversed(const DVector3& translate,const DVector3& scale, const DQuaternion& orientation);
		// decompose the matrix to orientation/scale/traslation vector
		void			decompose(DVector3& translate,DVector3& scale, DQuaternion& orientation);


		inline void swap(DMatrix4& rM)
		{
			std::swap(M[0][0], rM[0][0]);
			std::swap(M[0][1], rM[0][1]);
			std::swap(M[0][2], rM[0][2]);
			std::swap(M[0][3], rM[0][3]);
			std::swap(M[1][0], rM[1][0]);
			std::swap(M[1][1], rM[1][1]);
			std::swap(M[1][2], rM[1][2]);
			std::swap(M[1][3], rM[1][3]);
			std::swap(M[2][0], rM[2][0]);
			std::swap(M[2][1], rM[2][1]);
			std::swap(M[2][2], rM[2][2]);
			std::swap(M[2][3], rM[2][3]);
			std::swap(M[3][0], rM[3][0]);
			std::swap(M[3][1], rM[3][1]);
			std::swap(M[3][2], rM[3][2]);
			std::swap(M[3][3], rM[3][3]);
		}


		static const DMatrix4 ZERO;
		static const DMatrix4 IDENTITY;

	private:
		DReal M[4][4];


	};
}

#endif