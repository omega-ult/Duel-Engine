//  [11/10/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelRay.h"
#include "DuelVector.h"

namespace Duel
{
	
	DRay DRay::getCommonPerpendicularTo( const DRay& b, DReal* length /* = NULL */)
	{
		// from http://www.cnblogs.com/chuzhouGIS/archive/2011/12/12/2284774.html
		if ( mDirection == DVector3::ZERO || b.mDirection == DVector3::ZERO)
		{
			return DRay(DVector3::ZERO, DVector3::ZERO);
		}
		DVector3 M, N;
		DVector3 A = mOrigin;
		DVector3	B = mOrigin + mDirection;
		DVector3	C = b.mOrigin;
		DVector3 D = b.mOrigin + b.mDirection;


		DReal	F1_ab = (B.x - A.x)*(B.x - A.x) + (B.y - A.y)*(B.y - A.y) + (B.z - A.z)*(B.z - A.z);
		DReal	F1_cd = (D.x - C.x)*(D.x - C.x) + (D.y - C.y)*(D.y - C.y) + (D.z - C.z)*(D.z - C.z);
		DReal	F2 = (B.x - A.x)*(D.x - C.x) + (B.y - A.y)*(D.y - C.y) + (B.z - A.z)*(D.z - C.z);
		DReal	F3_ab = (B.x - A.x)*(C.x - A.x) + (B.y - A.y)*(C.y - A.y) + (B.z - A.z)*(C.z - A.z);
		DReal	F3_cd = (D.x - C.x)*(C.x - C.x) + (D.y - C.y)*(C.y - C.y) + (D.z - C.z)*(C.z - C.z);
// 		F1(a,b)=[(Xb-Xa)*(Xb-Xa)+(Yb-Ya)*(Yb-Ya)+(Zb-Za)*(Zb-Za)]
// 
// 		F1(c,d)= [(Xd-Xc)*(Xd-Xc)+(Yd-Yc)*(Yd-Yc)+(Zd-Zc)*(Zd-Zc)]
// 
// 		F2()=[(Xb-Xa)*(Xd-Xc)+(Yb-Ya)*(Yd-Yc)+(Zb-Za)*(Zd-Zc)]
// 
// 		F3(a,b)=[(Xb-Xa)*(Xc-Xa)+(Yb-Ya)*(Yc-Ya)+(Zb-Za)*(Zc-Za)]
// 
// 		F3(c,d)=[(Xd-Xc)*(Xc-Xa)+(Yd-Yc)*(Yc-Ya)+(Zd-Zc)*(Zc-Za)]
		
		// two ray share the same plane.
		if ((F1_ab*F1_cd - F2*F2) == 0 || (F2*F2 - F1_ab*F1_cd))
		{
			return DRay(DVector3::ZERO, DVector3::ZERO);
		}

		M = A + (B-A)*(F3_ab*F1_cd - F3_cd*F2)/(F1_ab*F1_cd - F2*F2);
		N = C + (D-C)*(F3_cd*F1_ab -F2*F3_ab)/(F2*F2 - F1_ab*F1_cd);
		DVector3 dir = N-M;
		// two ray share the same plane.
		if (dir == DVector3::ZERO)
		{
			return DRay(DVector3::ZERO, DVector3::ZERO);
		}
		if (length != NULL)
		{
			*length = dir.length();
		}
		dir.normalize();
		return DRay(M, dir);
// 		由此得到两个垂足点的坐标：
//		t1=[F3(a,b)*F1(c,d)-F3(c,d)*F2()]/[F1(a,b)*F1(c,d)-F2()*F2()]
// 
// 		t2=[F3(c,d)*F1(a,b)-F2()*F3(a,b)]/[F2()*F2()-F1(a,b)*F1(c,d)]
// 
// 		M(Xm,Ym,Zm),
// 
// 		Xm=t1*(Xb-Xa)+Xa=(Xb-Xa)*[F3(a,b)*F1(c,d)-F3(c,d)*F2()]/[F1(a,b)*F1(c,d)-F2()*F2()]+Xa;
// 
// 		Ym=t1*(Yb-Ya)+Ya=(Yb-Ya)*[F3(a,b)*F1(c,d)-F3(c,d)*F2()]/[F1(a,b)*F1(c,d)-F2()*F2()]+Ya;
// 
// 		Zm=t1*(Zb-Za)+Za=(Zb-Za)*[F3(a,b)*F1(c,d)-F3(c,d)*F2()]/[F1(a,b)*F1(c,d)-F2()*F2()]+Za;
// 
// 
// 
// 		N(Xn,Yn,Zn),
// 
// 		Xn=t2*(Xd-Xc)+Xc=(Xd-Xc)*[F3(c,d)*F1(a,b)-F3(a,b)*F2()]/[F2()*F2()-F1(a,b)*F1(c,d)]+Xc;
// 
// 		Yn=t2*(Yd-Yc)+Yc=(Yd-Yc)*[F3(c,d)*F1(a,b)-F3(a,b)*F2()]/[F2()*F2()-F1(a,b)*F1(c,d)]+Yc;
// 
// 		Zn=t2*(Zd-Zc)+Zc=(Zd-Zc)*[F3(c,d)*F1(a,b)-F3(a,b)*F2()]/[F2()*F2()-F1(a,b)*F1(c,d)]+Zc;

	}

	void DRay::pointTo( const DVector3& p )
	{
		if (p == mOrigin)
		{
			return;
		}
		DVector3 dir = p - mOrigin;
		dir.normalize();
		mDirection = dir;
	}

}