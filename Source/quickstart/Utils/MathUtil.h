#pragma once


struct MathUtil {

	static FORCEINLINE int CircularMinus(int now, int length)
	{
		int temp = now - 1;
		if (temp < 0)
		{
			temp = length - 1;
		}
		return temp;
	}

	static FORCEINLINE int CircularPlus(int now, int length)
	{
		int temp = now + 1;
		if (temp >= length)
		{
			temp = 0;
		}
		return temp;
	}

	static FORCEINLINE float AngleBetweenTwoVectors_2D(FVector& A, FVector& B)
	{
		float cosine = FVector::DotProduct(A, B);
		FVector cross = FVector::CrossProduct(A, B);
		float sine = cross.Z;

		float tangent = sine / cosine;

		float angle = FMath::Atan2(sine, cosine);

		return angle;
	}
};