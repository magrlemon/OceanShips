// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "Misc/AssertionMacros.h"
#include "Misc/Crc.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "Misc/Parse.h"
#include "Math/IntPoint.h"
#include "Logging/LogMacros.h"

/**
 * A vector in 2-D space composed of components (X, Y) with floating point precision.
 */
struct DVector2 
{
	/** Vector's X component. */
	double X;

	/** Vector's Y component. */
	double Y;

public:
	
	/** Global 2D zero vector constant (0,0) */
	static ENTITASFRAME_API const DVector2 ZeroVector;

	/**
	* Global 2D one vector (poorly named) constant (1,1).
	*
	* @note Incorrectly named "unit" vector though its magnitude/length/size is not one. Would fix, though likely used all over the world. Use `Unit45Deg` below for an actual unit vector.
	*/
	static ENTITASFRAME_API const DVector2 UnitVector;

	/**
	* Global 2D unit vector constant along the 45 degree angle or symmetrical positive axes (sqrt(.5),sqrt(.5)) or (.707,.707). https://en.wikipedia.org/wiki/Unit_vector
	*
	* @note The `UnitVector` above is actually a value with axes of 1 rather than a magnitude of one.
	*/
	static ENTITASFRAME_API const DVector2 Unit45Deg;

public:

	/** Default constructor (no initialization). */
	FORCEINLINE DVector2() { }

	/**
	 * Constructor using initial values for each component.
	 *
	 * @param InX X coordinate.
	 * @param InY Y coordinate.
	 */
	FORCEINLINE DVector2(double InX, double InY);

	/**
	 * Constructs a vector from an FIntPoint.
	 *
	 * @param InPos Integer point used to set this vector.
	 */
	FORCEINLINE DVector2(FIntPoint InPos);

	/**
	 * Constructor which initializes all components to zero.
	 *
	 * @param EForceInit Force init enum
	 */
	explicit FORCEINLINE DVector2(EForceInit);

	/**
	 * Constructor that does not initialize.  More explicit than the default constructor.
	 *
	 * @param ENoInit Don't init
	 */
	explicit FORCEINLINE DVector2(ENoInit) { }

	/**
	 * Constructs a vector from an FVector.
	 * Copies the X and Y components from the FVector.
	 *
	 * @param V Vector to copy from.
	 */
	explicit FORCEINLINE DVector2(const FVector& V);

public:

	/**
	 * Gets the result of adding two vectors together.
	 *
	 * @param V The other vector to add to this.
	 * @return The result of adding the vectors together.
	 */
	FORCEINLINE DVector2 operator+(const DVector2& V) const;

	/**
	 * Gets the result of subtracting a vector from this one.
	 *
	 * @param V The other vector to subtract from this.
	 * @return The result of the subtraction.
	 */
	FORCEINLINE DVector2 operator-(const DVector2& V) const;

	/**
	 * Gets the result of scaling the vector (multiplying each component by a value).
	 *
	 * @param Scale How much to scale the vector by.
	 * @return The result of scaling this vector.
	 */
	FORCEINLINE DVector2 operator*(double Scale) const;

	/**
	 * Gets the result of dividing each component of the vector by a value.
	 *
	 * @param Scale How much to divide the vector by.
	 * @return The result of division on this vector.
	 */
	DVector2 operator/(double Scale) const;

	/**
	 * Gets the result of adding A to each component of the vector.
	 *
	 * @param A Float to add to each component.
	 * @return The result of adding A to each component.
	 */
	FORCEINLINE DVector2 operator+(double A) const;

	/**
	 * Gets the result of subtracting A from each component of the vector.
	 *
	 * @param A Float to subtract from each component
	 * @return The result of subtracting A from each component.
	 */
	FORCEINLINE DVector2 operator-(double A) const;

	/**
	 * Gets the result of component-wise multiplication of this vector by another.
	 *
	 * @param V The other vector to multiply this by.
	 * @return The result of the multiplication.
	 */
	FORCEINLINE DVector2 operator*(const DVector2& V) const;

	/**
	 * Gets the result of component-wise division of this vector by another.
	 *
	 * @param V The other vector to divide this by.
	 * @return The result of the division.
	 */
	DVector2 operator/(const DVector2& V) const;

	/**
	 * Calculates dot product of this vector and another.
	 *
	 * @param V The other vector.
	 * @return The dot product.
	 */
	FORCEINLINE double operator|(const DVector2& V) const;

	/**
	 * Calculates cross product of this vector and another.
	 *
	 * @param V The other vector.
	 * @return The cross product.
	 */
	FORCEINLINE double operator^(const DVector2& V) const;

public:

	/**
	 * Compares this vector against another for equality.
	 *
	 * @param V The vector to compare against.
	 * @return true if the two vectors are equal, otherwise false.
	 */
	bool operator==(const DVector2& V) const;

	/**
	 * Compares this vector against another for inequality.
	 *
	 * @param V The vector to compare against.
	 * @return true if the two vectors are not equal, otherwise false.
	 */
	bool operator!=(const DVector2& V) const;

	/**
	 * Checks whether both components of this vector are less than another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this is the smaller vector, otherwise false.
	 */
	bool operator<(const DVector2& Other) const;

	/**
	 * Checks whether both components of this vector are greater than another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this is the larger vector, otherwise false.
	 */
	bool operator>(const DVector2& Other) const;

	/**
	 * Checks whether both components of this vector are less than or equal to another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this vector is less than or equal to the other vector, otherwise false.
	 */
	bool operator<=(const DVector2& Other) const;

	/**
	 * Checks whether both components of this vector are greater than or equal to another.
	 *
	 * @param Other The vector to compare against.
	 * @return true if this vector is greater than or equal to the other vector, otherwise false.
	 */
	bool operator>=(const DVector2& Other) const;

	/**
	 * Gets a negated copy of the vector.
	 *
	 * @return A negated copy of the vector.
	 */
	FORCEINLINE DVector2 operator-() const;

	/**
	 * Adds another vector to this.
	 *
	 * @param V The other vector to add.
	 * @return Copy of the vector after addition.
	 */
	FORCEINLINE DVector2 operator+=(const DVector2& V);

	/**
	 * Subtracts another vector from this.
	 *
	 * @param V The other vector to subtract.
	 * @return Copy of the vector after subtraction.
	 */
	FORCEINLINE DVector2 operator-=(const DVector2& V);

	/**
	 * Scales this vector.
	 *
	 * @param Scale The scale to multiply vector by.
	 * @return Copy of the vector after scaling.
	 */
	FORCEINLINE DVector2 operator*=(double Scale);

	/**
	 * Divides this vector.
	 *
	 * @param V What to divide vector by.
	 * @return Copy of the vector after division.
	 */
	DVector2 operator/=(double V);

	/**
	 * Multiplies this vector with another vector, using component-wise multiplication.
	 *
	 * @param V The vector to multiply with.
	 * @return Copy of the vector after multiplication.
	 */
	DVector2 operator*=(const DVector2& V);

	/**
	 * Divides this vector by another vector, using component-wise division.
	 *
	 * @param V The vector to divide by.
	 * @return Copy of the vector after division.
	 */
	DVector2 operator/=(const DVector2& V);

	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 * @return reference to component.
	 */
	double& operator[](int32 Index);

	/**
	 * Gets specific component of the vector.
	 *
	 * @param Index the index of vector component
	 * @return copy of component value.
	 */
	double operator[](int32 Index) const;

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component required.
	* @return Reference to the specified component.
	*/
	double& Component(int32 Index);

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component required.
	* @return Copy of the specified component.
	*/
	double Component(int32 Index) const;

public:

	/**
	 * Calculates the dot product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The dot product.
	 */
	FORCEINLINE static double DotProduct(const DVector2& A, const DVector2& B);

	/**
	 * Squared distance between two 2D points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two 2D points.
	 */
	FORCEINLINE static double DistSquared(const DVector2& V1, const DVector2& V2);

	/**
	 * Distance between two 2D points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The distance between two 2D points.
	 */
	FORCEINLINE static double Distance(const DVector2& V1, const DVector2& V2);

	/**
	 * Calculate the cross product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The cross product.
	 */
	FORCEINLINE static double CrossProduct(const DVector2& A, const DVector2& B);

	/**
	 * Returns a vector with the maximum component for each dimension from the pair of vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The max vector.
	 */
	FORCEINLINE static DVector2 Max(const DVector2& A, const DVector2& B);

	/**
	 * Returns a vector with the minimum component for each dimension from the pair of vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The min vector.
	 */
	FORCEINLINE static DVector2 Min(const DVector2& A, const DVector2& B);

	/**
	 * Checks for equality with error-tolerant comparison.
	 *
	 * @param V The vector to compare.
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within specified tolerance, otherwise false.
	 */
	bool Equals(const DVector2& V, double Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Set the values of the vector directly.
	 *
	 * @param InX New X coordinate.
	 * @param InY New Y coordinate.
	 */
	void Set(double InX, double InY);

	/**
	 * Get the maximum value of the vector's components.
	 *
	 * @return The maximum value of the vector's components.
	 */
	double GetMax() const;

	/**
	 * Get the maximum absolute value of the vector's components.
	 *
	 * @return The maximum absolute value of the vector's components.
	 */
	double GetAbsMax() const;

	/**
	 * Get the minimum value of the vector's components.
	 *
	 * @return The minimum value of the vector's components.
	 */
	double GetMin() const;

	/**
	 * Get the length (magnitude) of this vector.
	 *
	 * @return The length of this vector.
	 */
	double Size() const;

	/**
	 * Get the squared length of this vector.
	 *
	 * @return The squared length of this vector.
	 */
	double SizeSquared() const;

	/**
	 * Rotates around axis (0,0,1)
	 *
	 * @param AngleDeg Angle to rotate (in degrees)
	 * @return Rotated Vector
	 */
	DVector2 GetRotated(double AngleDeg) const;

	/**
	 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
	 * Returns zero vector if vector length is too small to safely normalize.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @return A normalized copy of the vector if safe, (0,0) otherwise.
	 */
	DVector2 GetSafeNormal(double Tolerance=SMALL_NUMBER) const;

	/**
	 * Normalize this vector in-place if it is large enough, set it to (0,0) otherwise.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @see GetSafeNormal()
	 */
	void Normalize(double Tolerance=SMALL_NUMBER);

	/**
	 * Checks whether vector is near to zero within a specified tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if vector is in tolerance to zero, otherwise false.
	 */
	bool IsNearlyZero(double Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Util to convert this vector into a unit direction vector and its original length.
	 *
	 * @param OutDir Reference passed in to store unit direction vector.
	 * @param OutLength Reference passed in to store length of the vector.
	 */
	void ToDirectionAndLength(DVector2 &OutDir, double &OutLength) const;

	/**
	 * Checks whether all components of the vector are exactly zero.
	 *
	 * @return true if vector is exactly zero, otherwise false.
	 */
	bool IsZero() const;

	/**
	 * Get this vector as an Int Point.
	 *
	 * @return New Int Point from this vector.
	 */
	FIntPoint IntPoint() const;

	/**
	 * Get this vector as a vector where each component has been rounded to the nearest int.
	 *
	 * @return New DVector2 from this vector that is rounded.
	 */
	DVector2 RoundToVector() const;

	/**
	 * Creates a copy of this vector with both axes clamped to the given range.
	 * @return New vector with clamped axes.
	 */
	DVector2 ClampAxes(double MinAxisVal, double MaxAxisVal) const;

	/**
	* Get a copy of the vector as sign only.
	* Each component is set to +1 or -1, with the sign of zero treated as +1.
	*
	* @param A copy of the vector with each component set to +1 or -1
	*/
	FORCEINLINE DVector2 GetSignVector() const;

	/**
	* Get a copy of this vector with absolute value of each component.
	*
	* @return A copy of this vector with absolute value of each component.
	*/
	FORCEINLINE DVector2 GetAbs() const;

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	FString ToString() const;

	/**
	 * Initialize this Vector based on an FString. The String is expected to contain X=, Y=.
	 * The DVector2 will be bogus when InitFromString returns false.
	 *
	 * @param	InSourceString	FString containing the vector values.
	 * @return true if the X,Y values were read successfully; false otherwise.
	 */
	bool InitFromString(const FString& InSourceString);

	/**
	 * Serialize a vector.
	 *
	 * @param Ar Serialization archive.
	 * @param V Vector being serialized.
	 * @return Reference to Archive after serialization.
	 */
	friend FArchive& operator<<(FArchive& Ar, DVector2& V)
	{
		// @warning BulkSerialize: DVector2 is serialized as memory dump
		// See TArray::BulkSerialize for detailed description of implied limitations.
		return Ar << V.X << V.Y;
	}

	friend void operator<<(FStructuredArchive::FSlot Slot, DVector2& V)
	{
		// @warning BulkSerialize: DVector2 is serialized as memory dump
		// See TArray::BulkSerialize for detailed description of implied limitations.
		FStructuredArchive::FStream Stream = Slot.EnterStream();
		Stream.EnterElement() << V.X;
		Stream.EnterElement() << V.Y;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}

	bool Serialize(FStructuredArchive::FSlot Slot)
	{
		Slot << *this;
		return true;
	}

#if ENABLE_NAN_DIAGNOSTIC
	FORCEINLINE void DiagnosticCheckNaN()
	{
		if (ContainsNaN())
		{
			logOrEnsureNanError(TEXT("FVector contains NaN: %s"), *ToString());
			*this = DVector2::ZeroVector;
		}
	}
#else
	FORCEINLINE void DiagnosticCheckNaN() {}
#endif

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this vector.
	 *
	 * @return true if there are any non-finite values in this vector, false otherwise.
	 */
	FORCEINLINE bool ContainsNaN() const
	{
		return (!FMath::IsFinite(X) || 
				!FMath::IsFinite(Y));
	}

	/**
	 * Network serialization function.
	 * FVectors NetSerialize without quantization (ie exact values are serialized).
	 */
	ENTITASFRAME_API bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	/** Converts spherical coordinates on the unit sphere into a Cartesian unit length vector. */
	inline FVector SphericalToUnitCartesian() const;
};

/**
 * Creates a hash value from a DVector2. 
 *
 * @param Vector the vector to create a hash value for
 * @return The hash value from the components
 */
FORCEINLINE uint32 GetTypeHash(const DVector2& Vector)
{
	// Note: this assumes there's no padding in DVector2 that could contain uncompared data.
	return FCrc::MemCrc_DEPRECATED(&Vector,sizeof(Vector));
}

/* DVector2 inline functions
 *****************************************************************************/

FORCEINLINE DVector2 operator*(double Scale, const DVector2& V)
{
	return V.operator*(Scale);
}

template <> struct TIsPODType<DVector2> { enum { Value = true }; };


FORCEINLINE DVector2::DVector2(double InX,double InY)
	:	X(InX), Y(InY)
{ }


FORCEINLINE DVector2::DVector2(FIntPoint InPos)
{
	X = (double)InPos.X;
	Y = (double)InPos.Y;
}


FORCEINLINE DVector2::DVector2(EForceInit)
	: X(0), Y(0)
{
}

FORCEINLINE DVector2 DVector2::operator+(const DVector2& V) const
{
	return DVector2(X + V.X, Y + V.Y);
}


FORCEINLINE DVector2 DVector2::operator-(const DVector2& V) const
{
	return DVector2(X - V.X, Y - V.Y);
}


FORCEINLINE DVector2 DVector2::operator*(double Scale) const
{
	return DVector2(X * Scale, Y * Scale);
}


FORCEINLINE DVector2 DVector2::operator/(double Scale) const
{
	const double RScale = 1.f/Scale;
	return DVector2(X * RScale, Y * RScale);
}


FORCEINLINE DVector2 DVector2::operator+(double A) const
{
	return DVector2(X + A, Y + A);
}


FORCEINLINE DVector2 DVector2::operator-(double A) const
{
	return DVector2(X - A, Y - A);
}


FORCEINLINE DVector2 DVector2::operator*(const DVector2& V) const
{
	return DVector2(X * V.X, Y * V.Y);
}


FORCEINLINE DVector2 DVector2::operator/(const DVector2& V) const
{
	return DVector2(X / V.X, Y / V.Y);
}


FORCEINLINE double DVector2::operator|(const DVector2& V) const
{
	return X*V.X + Y*V.Y;
}


FORCEINLINE double DVector2::operator^(const DVector2& V) const
{
	return X*V.Y - Y*V.X;
}


FORCEINLINE double DVector2::DotProduct(const DVector2& A, const DVector2& B)
{
	return A | B;
}


FORCEINLINE double DVector2::DistSquared(const DVector2 &V1, const DVector2 &V2)
{
	return FMath::Square(V2.X-V1.X) + FMath::Square(V2.Y-V1.Y);
}


FORCEINLINE double DVector2::Distance(const DVector2& V1, const DVector2& V2)
{
	return FMath::Sqrt(DVector2::DistSquared(V1, V2));
}


FORCEINLINE double DVector2::CrossProduct(const DVector2& A, const DVector2& B)
{
	return A ^ B;
}

FORCEINLINE DVector2 DVector2::Max(const DVector2& A, const DVector2& B)
{
	return DVector2(FMath::Max(A.X, B.X), FMath::Max(A.Y, B.Y));
}

FORCEINLINE DVector2 DVector2::Min(const DVector2& A, const DVector2& B)
{
	return DVector2(FMath::Min(A.X, B.X), FMath::Min(A.Y, B.Y));
}

FORCEINLINE bool DVector2::operator==(const DVector2& V) const
{
	return X==V.X && Y==V.Y;
}


FORCEINLINE bool DVector2::operator!=(const DVector2& V) const
{
	return X!=V.X || Y!=V.Y;
}


FORCEINLINE bool DVector2::operator<(const DVector2& Other) const
{
	return X < Other.X && Y < Other.Y;
}


FORCEINLINE bool DVector2::operator>(const DVector2& Other) const
{
	return X > Other.X && Y > Other.Y;
}


FORCEINLINE bool DVector2::operator<=(const DVector2& Other) const
{
	return X <= Other.X && Y <= Other.Y;
}


FORCEINLINE bool DVector2::operator>=(const DVector2& Other) const
{
	return X >= Other.X && Y >= Other.Y;
}


FORCEINLINE bool DVector2::Equals(const DVector2& V, double Tolerance) const
{
	return FMath::Abs(X-V.X) <= Tolerance && FMath::Abs(Y-V.Y) <= Tolerance;
}


FORCEINLINE DVector2 DVector2::operator-() const
{
	return DVector2(-X, -Y);
}


FORCEINLINE DVector2 DVector2::operator+=(const DVector2& V)
{
	X += V.X; Y += V.Y;
	return *this;
}


FORCEINLINE DVector2 DVector2::operator-=(const DVector2& V)
{
	X -= V.X; Y -= V.Y;
	return *this;
}


FORCEINLINE DVector2 DVector2::operator*=(double Scale)
{
	X *= Scale; Y *= Scale;
	return *this;
}


FORCEINLINE DVector2 DVector2::operator/=(double V)
{
	const double RV = 1.f/V;
	X *= RV; Y *= RV;
	return *this;
}


FORCEINLINE DVector2 DVector2::operator*=(const DVector2& V)
{
	X *= V.X; Y *= V.Y;
	return *this;
}


FORCEINLINE DVector2 DVector2::operator/=(const DVector2& V)
{
	X /= V.X; Y /= V.Y;
	return *this;
}


FORCEINLINE double& DVector2::operator[](int32 Index)
{
	check(Index>=0 && Index<2);
	return ((Index == 0) ? X : Y);
}


FORCEINLINE double DVector2::operator[](int32 Index) const
{
	check(Index>=0 && Index<2);
	return ((Index == 0) ? X : Y);
}


FORCEINLINE void DVector2::Set(double InX, double InY)
{
	X = InX;
	Y = InY;
}


FORCEINLINE double DVector2::GetMax() const
{
	return FMath::Max(X,Y);
}


FORCEINLINE double DVector2::GetAbsMax() const
{
	return FMath::Max(FMath::Abs(X),FMath::Abs(Y));
}


FORCEINLINE double DVector2::GetMin() const
{
	return FMath::Min(X,Y);
}


FORCEINLINE double DVector2::Size() const
{
	return FMath::Sqrt(X*X + Y*Y);
}


FORCEINLINE double DVector2::SizeSquared() const
{
	return X*X + Y*Y;
}



FORCEINLINE DVector2 DVector2::GetSafeNormal(double Tolerance) const
{	
	const double SquareSum = X*X + Y*Y;
	if(SquareSum > Tolerance)
	{
		const double Scale = FMath::InvSqrt(SquareSum);
		return DVector2(X*Scale, Y*Scale);
	}
	return DVector2(0.f, 0.f);
}


FORCEINLINE void DVector2::Normalize(double Tolerance)
{
	const double SquareSum = X*X + Y*Y;
	if(SquareSum > Tolerance)
	{
		const double Scale = FMath::InvSqrt(SquareSum);
		X *= Scale;
		Y *= Scale;
		return;
	}
	X = 0.0f;
	Y = 0.0f;
}


FORCEINLINE void DVector2::ToDirectionAndLength(DVector2 &OutDir, double &OutLength) const
{
	OutLength = Size();
	if (OutLength > SMALL_NUMBER)
	{
		double OneOverLength = 1.0f / OutLength;
		OutDir = DVector2(X*OneOverLength, Y*OneOverLength);
	}
	else
	{
		OutDir = DVector2::ZeroVector;
	}
}


FORCEINLINE bool DVector2::IsNearlyZero(double Tolerance) const
{
	return	FMath::Abs(X)<=Tolerance
		&&	FMath::Abs(Y)<=Tolerance;
}


FORCEINLINE bool DVector2::IsZero() const
{
	return X==0.f && Y==0.f;
}


FORCEINLINE double& DVector2::Component(int32 Index)
{
	return (&X)[Index];
}


FORCEINLINE double DVector2::Component(int32 Index) const
{
	return (&X)[Index];
}


FORCEINLINE FIntPoint DVector2::IntPoint() const
{
	return FIntPoint(FMath::RoundToInt(X), FMath::RoundToInt(Y));
}

FORCEINLINE DVector2 DVector2::RoundToVector() const
{
	return DVector2(FMath::RoundToInt(X), FMath::RoundToInt(Y));
}

FORCEINLINE DVector2 DVector2::ClampAxes(double MinAxisVal, double MaxAxisVal) const
{
	return DVector2(FMath::Clamp(X, MinAxisVal, MaxAxisVal), FMath::Clamp(Y, MinAxisVal, MaxAxisVal));
}



FORCEINLINE FString DVector2::ToString() const
{
	return FString::Printf(TEXT("X=%3.6f Y=%3.6f"), X, Y);
}


