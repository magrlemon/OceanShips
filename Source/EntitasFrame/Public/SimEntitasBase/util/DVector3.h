// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreTypes.h"
#include "Misc/AssertionMacros.h"
#include "Math/NumericLimits.h"
#include "Misc/Crc.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "Misc/Parse.h"
#include "Math/Color.h"
#include "Math/IntPoint.h"
#include "Logging/LogMacros.h"
#include "Math/Vector2D.h"
#include "Misc/ByteSwap.h"
#include "Internationalization/Text.h"
#include "Internationalization/Internationalization.h"
#include "Math/IntVector.h"
#include "Math/Axis.h"

#if PLATFORM_VECTOR_CUBIC_INTERP_SSE
#include "Math/UnrealMathSSE.h"
#endif

/**
 * A vector in 3-D space composed of components (X, Y, Z) with floating point precision.
 */
struct DVector3 
{
public:

	/** Vector's X component. */
	double X;

	/** Vector's Y component. */
	double Y;

	/** Vector's Z component. */
	double Z;

public:

	/** A zero vector (0,0,0) */
	static ENTITASFRAME_API const DVector3 ZeroVector;

	/** One vector (1,1,1) */
	static ENTITASFRAME_API const DVector3 OneVector;

	/** Unreal up vector (0,0,1) */
	static ENTITASFRAME_API const DVector3 UpVector;

	/** Unreal down vector (0,0,-1) */
	static ENTITASFRAME_API const DVector3 DownVector;

	/** Unreal forward vector (1,0,0) */
	static ENTITASFRAME_API const DVector3 ForwardVector;

	/** Unreal backward vector (-1,0,0) */
	static ENTITASFRAME_API const DVector3 BackwardVector;

	/** Unreal right vector (0,1,0) */
	static ENTITASFRAME_API const DVector3 RightVector;

	/** Unreal left vector (0,-1,0) */
	static ENTITASFRAME_API const DVector3 LeftVector;

public:

#if ENABLE_NAN_DIAGNOSTIC
	FORCEINLINE void DiagnosticCheckNaN() const
	{
		if (ContainsNaN())
		{
			logOrEnsureNanError(TEXT("DVector3 contains NaN: %s"), *ToString());
			*const_cast<DVector3*>(this) = ZeroVector;
		}
	}

	FORCEINLINE void DiagnosticCheckNaN(const TCHAR* Message) const
	{
		if (ContainsNaN())
		{
			logOrEnsureNanError(TEXT("%s: DVector3 contains NaN: %s"), Message, *ToString());
			*const_cast<DVector3*>(this) = ZeroVector;
		}
	}
#else
	FORCEINLINE void DiagnosticCheckNaN() const {}
	FORCEINLINE void DiagnosticCheckNaN(const TCHAR* Message) const {}
#endif

	/** Default constructor (no initialization). */
	FORCEINLINE DVector3();

	/**
	 * Constructor initializing all components to a single double value.
	 *
	 * @param InF Value to set all components to.
	 */
	explicit FORCEINLINE DVector3(double InF);

	/**
	 * Constructor using initial values for each component.
	 *
	 * @param InX X Coordinate.
	 * @param InY Y Coordinate.
	 * @param InZ Z Coordinate.
	 */
	FORCEINLINE DVector3(double InX, double InY, double InZ);

	/**
	 * Constructs a vector from an FVector2D and Z value.
	 * 
	 * @param V Vector to copy from.
	 * @param InZ Z Coordinate.
	 */
	explicit FORCEINLINE DVector3(const FVector2D V, double InZ);

	/**
	 * Constructor using the XYZ components from a 4D vector.
	 *
	 * @param V 4D Vector to copy from.
	 */
	FORCEINLINE DVector3(const FVector4& V);

	/**
	 * Constructs a vector from an FLinearColor.
	 *
	 * @param InColor Color to copy from.
	 */
	explicit DVector3(const FLinearColor& InColor);

	/**
	 * Constructs a vector from an FIntVector.
	 *
	 * @param InVector FIntVector to copy from.
	 */
	explicit DVector3(FIntVector InVector);

	/**
	 * Constructs a vector from an FIntPoint.
	 *
	 * @param A Int Point used to set X and Y coordinates, Z is set to zero.
	 */
	explicit DVector3(FIntPoint A);

	/**
	 * Constructor which initializes all components to zero.
	 *
	 * @param EForceInit Force init enum
	 */
	explicit FORCEINLINE DVector3(EForceInit);

#ifdef IMPLEMENT_ASSIGNMENT_OPERATOR_MANUALLY
	/**
	* Copy another DVector3 into this one
	*
	* @param Other The other vector.
	* @return Reference to vector after copy.
	*/
	FORCEINLINE DVector3& operator=(const DVector3& Other);
#endif

	/**
	 * Calculate cross product between this and another vector.
	 *
	 * @param V The other vector.
	 * @return The cross product.
	 */
	FORCEINLINE DVector3 operator^(const DVector3& V) const;

	/**
	 * Calculate the cross product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The cross product.
	 */
	FORCEINLINE static DVector3 CrossProduct(const DVector3& A, const DVector3& B);

	/**
	 * Calculate the dot product between this and another vector.
	 *
	 * @param V The other vector.
	 * @return The dot product.
	 */
	FORCEINLINE double operator|(const DVector3& V) const;

	/**
	 * Calculate the dot product of two vectors.
	 *
	 * @param A The first vector.
	 * @param B The second vector.
	 * @return The dot product.
	 */
	FORCEINLINE static double DotProduct(const DVector3& A, const DVector3& B);

	/**
	 * Gets the result of component-wise addition of this and another vector.
	 *
	 * @param V The vector to add to this.
	 * @return The result of vector addition.
	 */
	FORCEINLINE DVector3 operator+(const DVector3& V) const;

	/**
	 * Gets the result of component-wise subtraction of this by another vector.
	 *
	 * @param V The vector to subtract from this.
	 * @return The result of vector subtraction.
	 */
	FORCEINLINE DVector3 operator-(const DVector3& V) const;

	/**
	 * Gets the result of subtracting from each component of the vector.
	 *
	 * @param Bias How much to subtract from each component.
	 * @return The result of subtraction.
	 */
	FORCEINLINE DVector3 operator-(double Bias) const;

	/**
	 * Gets the result of adding to each component of the vector.
	 *
	 * @param Bias How much to add to each component.
	 * @return The result of addition.
	 */
	FORCEINLINE DVector3 operator+(double Bias) const;

	/**
	 * Gets the result of scaling the vector (multiplying each component by a value).
	 *
	 * @param Scale What to multiply each component by.
	 * @return The result of multiplication.
	 */
	FORCEINLINE DVector3 operator*(double Scale) const;

	/**
	 * Gets the result of dividing each component of the vector by a value.
	 *
	 * @param Scale What to divide each component by.
	 * @return The result of division.
	 */
	DVector3 operator/(double Scale) const;

	/**
	 * Gets the result of component-wise multiplication of this vector by another.
	 *
	 * @param V The vector to multiply with.
	 * @return The result of multiplication.
	 */
	FORCEINLINE DVector3 operator*(const DVector3& V) const;

	/**
	 * Gets the result of component-wise division of this vector by another.
	 *
	 * @param V The vector to divide by.
	 * @return The result of division.
	 */
	FORCEINLINE DVector3 operator/(const DVector3& V) const;

	// Binary comparison operators.

	/**
	 * Check against another vector for equality.
	 *
	 * @param V The vector to check against.
	 * @return true if the vectors are equal, false otherwise.
	 */
	bool operator==(const DVector3& V) const;

	/**
	 * Check against another vector for inequality.
	 *
	 * @param V The vector to check against.
	 * @return true if the vectors are not equal, false otherwise.
	 */
	bool operator!=(const DVector3& V) const;

	/**
	 * Check against another vector for equality, within specified error limits.
	 *
	 * @param V The vector to check against.
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within tolerance limits, false otherwise.
	 */
	bool Equals(const DVector3& V, double Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether all components of this vector are the same, within a tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if the vectors are equal within tolerance limits, false otherwise.
	 */
	bool AllComponentsEqual(double Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Get a negated copy of the vector.
	 *
	 * @return A negated copy of the vector.
	 */
	FORCEINLINE DVector3 operator-() const;

	/**
	 * Adds another vector to this.
	 * Uses component-wise addition.
	 *
	 * @param V Vector to add to this.
	 * @return Copy of the vector after addition.
	 */
	FORCEINLINE DVector3 operator+=(const DVector3& V);

	/**
	 * Subtracts another vector from this.
	 * Uses component-wise subtraction.
	 *
	 * @param V Vector to subtract from this.
	 * @return Copy of the vector after subtraction.
	 */
	FORCEINLINE DVector3 operator-=(const DVector3& V);

	/**
	 * Scales the vector.
	 *
	 * @param Scale Amount to scale this vector by.
	 * @return Copy of the vector after scaling.
	 */
	FORCEINLINE DVector3 operator*=(double Scale);

	/**
	 * Divides the vector by a number.
	 *
	 * @param V What to divide this vector by.
	 * @return Copy of the vector after division.
	 */
	DVector3 operator/=(double V);

	/**
	 * Multiplies the vector with another vector, using component-wise multiplication.
	 *
	 * @param V What to multiply this vector with.
	 * @return Copy of the vector after multiplication.
	 */
	DVector3 operator*=(const DVector3& V);

	/**
	 * Divides the vector by another vector, using component-wise division.
	 *
	 * @param V What to divide vector by.
	 * @return Copy of the vector after division.
	 */
	DVector3 operator/=(const DVector3& V);

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
	 * @return Copy of the component.
	 */
	double operator[](int32 Index)const;

	/**
	* Gets a specific component of the vector.
	*
	* @param Index The index of the component required.
	*
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


	/** Get a specific component of the vector, given a specific axis by enum */
	double GetComponentForAxis(EAxis::Type Axis) const;

	/** Set a specified componet of the vector, given a specific axis by enum */
	void SetComponentForAxis(EAxis::Type Axis, double Component);

public:

	// Simple functions.

	/**
	 * Set the values of the vector directly.
	 *
	 * @param InX New X coordinate.
	 * @param InY New Y coordinate.
	 * @param InZ New Z coordinate.
	 */
	void Set(double InX, double InY, double InZ);

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
	 * Get the minimum absolute value of the vector's components.
	 *
	 * @return The minimum absolute value of the vector's components.
	 */
	double GetAbsMin() const;

	/** Gets the component-wise min of two vectors. */
	DVector3 ComponentMin(const DVector3& Other) const;

	/** Gets the component-wise max of two vectors. */
	DVector3 ComponentMax(const DVector3& Other) const;

	/**
	 * Get a copy of this vector with absolute value of each component.
	 *
	 * @return A copy of this vector with absolute value of each component.
	 */
	DVector3 GetAbs() const;

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
	 * Get the length of the 2D components of this vector.
	 *
	 * @return The 2D length of this vector.
	 */
	double Size2D() const ;

	/**
	 * Get the squared length of the 2D components of this vector.
	 *
	 * @return The squared 2D length of this vector.
	 */
	double SizeSquared2D() const ;

	/**
	 * Checks whether vector is near to zero within a specified tolerance.
	 *
	 * @param Tolerance Error tolerance.
	 * @return true if the vector is near to zero, false otherwise.
	 */
	bool IsNearlyZero(double Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether all components of the vector are exactly zero.
	 *
	 * @return true if the vector is exactly zero, false otherwise.
	 */
	bool IsZero() const;

	/**
	 * Check if the vector is of unit length, with specified tolerance.
	 *
	 * @param LengthSquaredTolerance Tolerance against squared length.
	 * @return true if the vector is a unit vector within the specified tolerance.
	 */
	FORCEINLINE bool IsUnit(double LengthSquaredTolerance = KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether vector is normalized.
	 *
	 * @return true if normalized, false otherwise.
	 */
	bool IsNormalized() const;

	/**
	 * Normalize this vector in-place if it is larger than a given tolerance. Leaves it unchanged if not.
	 *
	 * @param Tolerance Minimum squared length of vector for normalization.
	 * @return true if the vector was normalized correctly, false otherwise.
	 */
	bool Normalize(double Tolerance=SMALL_NUMBER);

	/**
	 * Calculates normalized version of vector without checking for zero length.
	 *
	 * @return Normalized version of vector.
	 * @see GetSafeNormal()
	 */
	FORCEINLINE DVector3 GetUnsafeNormal() const;

	/**
	 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
	 * Returns zero vector if vector length is too small to safely normalize.
	 *
	 * @param Tolerance Minimum squared vector length.
	 * @return A normalized copy if safe, (0,0,0) otherwise.
	 */
	DVector3 GetSafeNormal(double Tolerance=SMALL_NUMBER) const;

	/**
	 * Gets a normalized copy of the 2D components of the vector, checking it is safe to do so. Z is set to zero. 
	 * Returns zero vector if vector length is too small to normalize.
	 *
	 * @param Tolerance Minimum squared vector length.
	 * @return Normalized copy if safe, otherwise returns zero vector.
	 */
	DVector3 GetSafeNormal2D(double Tolerance=SMALL_NUMBER) const;

	/**
	 * Util to convert this vector into a unit direction vector and its original length.
	 *
	 * @param OutDir Reference passed in to store unit direction vector.
	 * @param OutLength Reference passed in to store length of the vector.
	 */
	void ToDirectionAndLength(DVector3 &OutDir, double &OutLength) const;

	/**
	 * Get a copy of the vector as sign only.
	 * Each component is set to +1 or -1, with the sign of zero treated as +1.
	 *
	 * @param A copy of the vector with each component set to +1 or -1
	 */
	FORCEINLINE DVector3 GetSignVector() const;

	/**
	 * Projects 2D components of vector based on Z.
	 *
	 * @return Projected version of vector based on Z.
	 */
	DVector3 Projection() const;

	/**
	* Calculates normalized 2D version of vector without checking for zero length.
	*
	* @return Normalized version of vector.
	* @see GetSafeNormal2D()
	*/
	FORCEINLINE DVector3 GetUnsafeNormal2D() const;

	/**
	 * Gets a copy of this vector snapped to a grid.
	 *
	 * @param GridSz Grid dimension.
	 * @return A copy of this vector snapped to a grid.
	 * @see FMath::GridSnap()
	 */
	DVector3 GridSnap(const double& GridSz) const;

	/**
	 * Get a copy of this vector, clamped inside of a cube.
	 *
	 * @param Radius Half size of the cube.
	 * @return A copy of this vector, bound by cube.
	 */
	DVector3 BoundToCube(double Radius) const;

	/** Get a copy of this vector, clamped inside of a cube. */
	DVector3 BoundToBox(const DVector3& Min, const DVector3 Max) const;

	/** Create a copy of this vector, with its magnitude clamped between Min and Max. */
	DVector3 GetClampedToSize(double Min, double Max) const;

	/** Create a copy of this vector, with the 2D magnitude clamped between Min and Max. Z is unchanged. */
	DVector3 GetClampedToSize2D(double Min, double Max) const;

	/** Create a copy of this vector, with its maximum magnitude clamped to MaxSize. */
	DVector3 GetClampedToMaxSize(double MaxSize) const;

	/** Create a copy of this vector, with the maximum 2D magnitude clamped to MaxSize. Z is unchanged. */
	DVector3 GetClampedToMaxSize2D(double MaxSize) const;

	/**
	 * Add a vector to this and clamp the result in a cube.
	 *
	 * @param V Vector to add.
	 * @param Radius Half size of the cube.
	 */
	void AddBounded(const DVector3& V, double Radius=MAX_int16);

	/**
	 * Gets the reciprocal of this vector, avoiding division by zero.
	 * Zero components are set to BIG_NUMBER.
	 *
	 * @return Reciprocal of this vector.
	 */
	DVector3 Reciprocal() const;

	/**
	 * Check whether X, Y and Z are nearly equal.
	 *
	 * @param Tolerance Specified Tolerance.
	 * @return true if X == Y == Z within the specified tolerance.
	 */
	bool IsUniform(double Tolerance=KINDA_SMALL_NUMBER) const;

	/**
	 * Mirror a vector about a normal vector.
	 *
	 * @param MirrorNormal Normal vector to mirror about.
	 * @return Mirrored vector.
	 */
	DVector3 MirrorByVector(const DVector3& MirrorNormal) const;

	/**
	 * Mirrors a vector about a plane.
	 *
	 * @param Plane Plane to mirror about.
	 * @return Mirrored vector.
	 */
	DVector3 MirrorByPlane(const FPlane& Plane) const;

	/**
	 * Rotates around Axis (assumes Axis.Size() == 1).
	 *
	 * @param Angle Angle to rotate (in degrees).
	 * @param Axis Axis to rotate around.
	 * @return Rotated Vector.
	 */
	DVector3 RotateAngleAxis(const double AngleDeg, const DVector3& Axis) const;

	/**
	 * Returns the cosine of the angle between this vector and another projected onto the XY plane (no Z).
	 *
	 * @param B the other vector to find the 2D cosine of the angle with.
	 * @return The cosine.
	 */
	FORCEINLINE double CosineAngle2D(DVector3 B) const;

	/**
	 * Gets a copy of this vector projected onto the input vector.
	 *
	 * @param A	Vector to project onto, does not assume it is normalized.
	 * @return Projected vector.
	 */
	FORCEINLINE DVector3 ProjectOnTo(const DVector3& A) const ;

	/**
	 * Gets a copy of this vector projected onto the input vector, which is assumed to be unit length.
	 * 
	 * @param  Normal Vector to project onto (assumed to be unit length).
	 * @return Projected vector.
	 */
	FORCEINLINE DVector3 ProjectOnToNormal(const DVector3& Normal) const;

	/**
	 * Return the FRotator orientation corresponding to the direction in which the vector points.
	 * Sets Yaw and Pitch to the proper numbers, and sets Roll to zero because the roll can't be determined from a vector.
	 *
	 * @return FRotator from the Vector's direction, without any roll.
	 * @see ToOrientationQuat()
	 */
	ENTITASFRAME_API FRotator ToOrientationRotator() const;

	/**
	 * Return the Quaternion orientation corresponding to the direction in which the vector points.
	 * Similar to the FRotator version, returns a result without roll such that it preserves the up vector.
	 *
	 * @note If you don't care about preserving the up vector and just want the most direct rotation, you can use the faster
	 * 'FQuat::FindBetweenVectors(DVector3::ForwardVector, YourVector)' or 'FQuat::FindBetweenNormals(...)' if you know the vector is of unit length.
	 *
	 * @return Quaternion from the Vector's direction, without any roll.
	 * @see ToOrientationRotator(), FQuat::FindBetweenVectors()
	 */
	ENTITASFRAME_API FQuat ToOrientationQuat() const;

	/**
	 * Return the FRotator orientation corresponding to the direction in which the vector points.
	 * Sets Yaw and Pitch to the proper numbers, and sets Roll to zero because the roll can't be determined from a vector.
	 * @note Identical to 'ToOrientationRotator()' and preserved for legacy reasons.
	 * @return FRotator from the Vector's direction.
	 * @see ToOrientationRotator(), ToOrientationQuat()
	 */
	ENTITASFRAME_API FRotator Rotation() const;

	/**
	 * Find good arbitrary axis vectors to represent U and V axes of a plane,
	 * using this vector as the normal of the plane.
	 *
	 * @param Axis1 Reference to first axis.
	 * @param Axis2 Reference to second axis.
	 */
	ENTITASFRAME_API void FindBestAxisVectors(DVector3& Axis1, DVector3& Axis2) const;

	/** When this vector contains Euler angles (degrees), ensure that angles are between +/-180 */
	ENTITASFRAME_API void UnwindEuler();

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this vector.
	 *
	 * @return true if there are any non-finite values in this vector, false otherwise.
	 */
	bool ContainsNaN() const;

	/**
	 * Get a textual representation of this vector.
	 *
	 * @return A string describing the vector.
	 */
	FString ToString() const;

	/**
	* Get a locale aware textual representation of this vector.
	*
	* @return A string describing the vector.
	*/
	FText ToText() const;

	/** Get a short textural representation of this vector, for compact readable logging. */
	FString ToCompactString() const;

	/** Get a short locale aware textural representation of this vector, for compact readable logging. */
	FText ToCompactText() const;

	/**
	 * Initialize this Vector based on an FString. The String is expected to contain X=, Y=, Z=.
	 * The DVector3 will be bogus when InitFromString returns false.
	 *
	 * @param	InSourceString	FString containing the vector values.
	 * @return true if the X,Y,Z values were read successfully; false otherwise.
	 */
	bool InitFromString(const FString& InSourceString);

	/** 
	 * Converts a Cartesian unit vector into spherical coordinates on the unit sphere.
	 * @return Output Theta will be in the range [0, PI], and output Phi will be in the range [-PI, PI]. 
	 */
	FVector2D UnitCartesianToSpherical() const;

	/**
	 * Convert a direction vector into a 'heading' angle.
	 *
	 * @return 'Heading' angle between +/-PI. 0 is pointing down +X.
	 */
	double HeadingAngle() const;

	/**
	 * Create an orthonormal basis from a basis with at least two orthogonal vectors.
	 * It may change the directions of the X and Y axes to make the basis orthogonal,
	 * but it won't change the direction of the Z axis.
	 * All axes will be normalized.
	 *
	 * @param XAxis The input basis' XAxis, and upon return the orthonormal basis' XAxis.
	 * @param YAxis The input basis' YAxis, and upon return the orthonormal basis' YAxis.
	 * @param ZAxis The input basis' ZAxis, and upon return the orthonormal basis' ZAxis.
	 */
	static ENTITASFRAME_API void CreateOrthonormalBasis(DVector3& XAxis,DVector3& YAxis,DVector3& ZAxis);

	/**
	 * Compare two points and see if they're the same, using a threshold.
	 *
	 * @param P First vector.
	 * @param Q Second vector.
	 * @return Whether points are the same within a threshold. Uses fast distance approximation (linear per-component distance).
	 */
	static bool PointsAreSame(const DVector3 &P, const DVector3 &Q);
	
	/**
	 * Compare two points and see if they're within specified distance.
	 *
	 * @param Point1 First vector.
	 * @param Point2 Second vector.
	 * @param Dist Specified distance.
	 * @return Whether two points are within the specified distance. Uses fast distance approximation (linear per-component distance).
	 */
	static bool PointsAreNear(const DVector3 &Point1, const DVector3 &Point2, double Dist);

	/**
	 * Calculate the signed distance (in the direction of the normal) between a point and a plane.
	 *
	 * @param Point The Point we are checking.
	 * @param PlaneBase The Base Point in the plane.
	 * @param PlaneNormal The Normal of the plane (assumed to be unit length).
	 * @return Signed distance between point and plane.
	 */
	static double PointPlaneDist(const DVector3 &Point, const DVector3 &PlaneBase, const DVector3 &PlaneNormal);

	/**
	 * Calculate the projection of a point on the given plane.
	 *
	 * @param Point The point to project onto the plane
	 * @param Plane The plane
	 * @return Projection of Point onto Plane
	 */
	static DVector3 PointPlaneProject(const DVector3& Point, const FPlane& Plane);

	/**
	 * Calculate the projection of a point on the plane defined by counter-clockwise (CCW) points A,B,C.
	 *
	 * @param Point The point to project onto the plane
	 * @param A 1st of three points in CCW order defining the plane 
	 * @param B 2nd of three points in CCW order defining the plane
	 * @param C 3rd of three points in CCW order defining the plane
	 * @return Projection of Point onto plane ABC
	 */
	static DVector3 PointPlaneProject(const DVector3& Point, const DVector3& A, const DVector3& B, const DVector3& C);

	/**
	* Calculate the projection of a point on the plane defined by PlaneBase and PlaneNormal.
	*
	* @param Point The point to project onto the plane
	* @param PlaneBase Point on the plane
	* @param PlaneNorm Normal of the plane (assumed to be unit length).
	* @return Projection of Point onto plane
	*/
	static DVector3 PointPlaneProject(const DVector3& Point, const DVector3& PlaneBase, const DVector3& PlaneNormal);

	/**
	 * Calculate the projection of a vector on the plane defined by PlaneNormal.
	 * 
	 * @param  V The vector to project onto the plane.
	 * @param  PlaneNormal Normal of the plane (assumed to be unit length).
	 * @return Projection of V onto plane.
	 */
	static DVector3 VectorPlaneProject(const DVector3& V, const DVector3& PlaneNormal);

	/**
	 * Euclidean distance between two points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The distance between two points.
	 */
	static FORCEINLINE double Dist( const DVector3 &V1, const DVector3 &V2 ) { return 0; };
	static FORCEINLINE double Distance(const DVector3 &V1, const DVector3 &V2) { return Dist(V1, V2); }

	/**
	* Euclidean distance between two points in the XY plane (ignoring Z).
	*
	* @param V1 The first point.
	* @param V2 The second point.
	* @return The distance between two points in the XY plane.
	*/
	static FORCEINLINE double DistXY( const DVector3 &V1, const DVector3 &V2 ) { return 0; };
	static FORCEINLINE double Dist2D(const DVector3 &V1, const DVector3 &V2) { return DistXY(V1, V2); }

	/**
	 * Squared distance between two points.
	 *
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two points.
	 */
	static FORCEINLINE double DistSquared(const DVector3 &V1, const DVector3 &V2);

	/**
	 * Squared distance between two points in the XY plane only.
	 *	
	 * @param V1 The first point.
	 * @param V2 The second point.
	 * @return The squared distance between two points in the XY plane
	 */
	static FORCEINLINE double DistSquaredXY( const DVector3 &V1, const DVector3 &V2 ) { return 0; };
	static FORCEINLINE double DistSquared2D(const DVector3 &V1, const DVector3 &V2) { return DistSquaredXY(V1, V2); }
	
	/**
	 * Compute pushout of a box from a plane.
	 *
	 * @param Normal The plane normal.
	 * @param Size The size of the box.
	 * @return Pushout required.
	 */
	static FORCEINLINE double BoxPushOut(const DVector3& Normal, const DVector3& Size);

	/**
	 * See if two normal vectors are nearly parallel, meaning the angle between them is close to 0 degrees.
	 *
	 * @param  Normal1 First normalized vector.
	 * @param  Normal1 Second normalized vector.
	 * @param  ParallelCosineThreshold Normals are parallel if absolute value of dot product (cosine of angle between them) is greater than or equal to this. For example: cos(1.0 degrees).
	 * @return true if vectors are nearly parallel, false otherwise.
	 */
	static bool Parallel(const DVector3& Normal1, const DVector3& Normal2, double ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL);

	/**
	 * See if two normal vectors are coincident (nearly parallel and point in the same direction).
	 * 
	 * @param  Normal1 First normalized vector.
	 * @param  Normal2 Second normalized vector.
	 * @param  ParallelCosineThreshold Normals are coincident if dot product (cosine of angle between them) is greater than or equal to this. For example: cos(1.0 degrees).
	 * @return true if vectors are coincident (nearly parallel and point in the same direction), false otherwise.
	 */
	static bool Coincident(const DVector3& Normal1, const DVector3& Normal2, double ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL);

	/**
	 * See if two normal vectors are nearly orthogonal (perpendicular), meaning the angle between them is close to 90 degrees.
	 * 
	 * @param  Normal1 First normalized vector.
	 * @param  Normal2 Second normalized vector.
	 * @param  OrthogonalCosineThreshold Normals are orthogonal if absolute value of dot product (cosine of angle between them) is less than or equal to this. For example: cos(89.0 degrees).
	 * @return true if vectors are orthogonal (perpendicular), false otherwise.
	 */
	static bool Orthogonal(const DVector3& Normal1, const DVector3& Normal2, double OrthogonalCosineThreshold = THRESH_NORMALS_ARE_ORTHOGONAL);

	/**
	 * See if two planes are coplanar. They are coplanar if the normals are nearly parallel and the planes include the same set of points.
	 *
	 * @param Base1 The base point in the first plane.
	 * @param Normal1 The normal of the first plane.
	 * @param Base2 The base point in the second plane.
	 * @param Normal2 The normal of the second plane.
	 * @param ParallelCosineThreshold Normals are parallel if absolute value of dot product is greater than or equal to this.
	 * @return true if the planes are coplanar, false otherwise.
	 */
	static bool Coplanar(const DVector3& Base1, const DVector3& Normal1, const DVector3& Base2, const DVector3& Normal2, double ParallelCosineThreshold = THRESH_NORMALS_ARE_PARALLEL);

	/**
	 * Triple product of three vectors: X dot (Y cross Z).
	 *
	 * @param X The first vector.
	 * @param Y The second vector.
	 * @param Z The third vector.
	 * @return The triple product: X dot (Y cross Z).
	 */
	static double Triple(const DVector3& X, const DVector3& Y, const DVector3& Z);

	/**
	 * Generates a list of sample points on a Bezier curve defined by 2 points.
	 *
	 * @param ControlPoints	Array of 4 FVectors (vert1, controlpoint1, controlpoint2, vert2).
	 * @param NumPoints Number of samples.
	 * @param OutPoints Receives the output samples.
	 * @return The path length.
	 */
	static ENTITASFRAME_API double EvaluateBezier(const DVector3* ControlPoints, int32 NumPoints, TArray<DVector3>& OutPoints);

	/**
	 * Converts a vector containing radian values to a vector containing degree values.
	 *
	 * @param RadVector	Vector containing radian values
	 * @return Vector  containing degree values
	 */
	static DVector3 RadiansToDegrees(const DVector3& RadVector);

	/**
	 * Converts a vector containing degree values to a vector containing radian values.
	 *
	 * @param DegVector	Vector containing degree values
	 * @return Vector containing radian values
	 */
	static DVector3 DegreesToRadians(const DVector3& DegVector);

	/**
	 * Given a current set of cluster centers, a set of points, iterate N times to move clusters to be central. 
	 *
	 * @param Clusters Reference to array of Clusters.
	 * @param Points Set of points.
	 * @param NumIterations Number of iterations.
	 * @param NumConnectionsToBeValid Sometimes you will have long strings that come off the mass of points
	 * which happen to have been chosen as Cluster starting points.  You want to be able to disregard those.
	 */
	static ENTITASFRAME_API void GenerateClusterCenters(TArray<DVector3>& Clusters, const TArray<DVector3>& Points, int32 NumIterations, int32 NumConnectionsToBeValid);

	/**
	 * Serializer.
	 *
	 * @param Ar Serialization Archive.
	 * @param V Vector to serialize.
	 * @return Reference to Archive after serialization.
	 */
	friend FArchive& operator<<(FArchive& Ar, DVector3& V)
	{
		// @warning BulkSerialize: DVector3 is serialized as memory dump
		// See TArray::BulkSerialize for detailed description of implied limitations.
		return Ar << V.X << V.Y << V.Z;
	}

	/**
	 * Structured archive slot serializer.
	 *
	 * @param Slot Structured archive slot.
	 * @param V Vector to serialize.
	 */
	friend void operator<<(FStructuredArchive::FSlot Slot, DVector3& V)
	{
		// @warning BulkSerialize: DVector3 is serialized as memory dump
		// See TArray::BulkSerialize for detailed description of implied limitations.
		FStructuredArchive::FStream Stream = Slot.EnterStream();
		Stream.EnterElement() << V.X;
		Stream.EnterElement() << V.Y;
		Stream.EnterElement() << V.Z;
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

	/** 
	 * Network serialization function.
	 * FVectors NetSerialize without quantization (ie exact values are serialized).
	 *
	 * @see FVector_NetQuantize, FVector_NetQuantize10, FVector_NetQuantize100, FVector_NetQuantizeNormal
	 */
	ENTITASFRAME_API bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};


/* DVector3 inline functions
 *****************************************************************************/

/**
 * Multiplies a vector by a scaling factor.
 *
 * @param Scale Scaling factor.
 * @param V Vector to scale.
 * @return Result of multiplication.
 */
FORCEINLINE DVector3 operator*(double Scale, const DVector3& V)
{
	return V.operator*(Scale);
}


/**
 * Creates a hash value from a DVector3. 
 *
 * @param Vector the vector to create a hash value for
 * @return The hash value from the components
 */
FORCEINLINE uint32 GetTypeHash(const DVector3& Vector)
{
	// Note: this assumes there's no padding in DVector3 that could contain uncompared data.
	return FCrc::MemCrc_DEPRECATED(&Vector,sizeof(Vector));
}


#if PLATFORM_LITTLE_ENDIAN
	#define INTEL_ORDER_VECTOR(x) (x)
#else
	static FORCEINLINE DVector3 INTEL_ORDER_VECTOR(DVector3 v)
	{
		return DVector3(INTEL_ORDERF(v.X), INTEL_ORDERF(v.Y), INTEL_ORDERF(v.Z));
	}
#endif



FORCEINLINE DVector3::DVector3(const FVector2D V, double InZ)
	: X(V.X), Y(V.Y), Z(InZ)
{
	DiagnosticCheckNaN();
}


inline bool DVector3::PointsAreSame(const DVector3 &P, const DVector3 &Q)
{
	double Temp;
	Temp=P.X-Q.X;
	if((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
	{
		Temp=P.Y-Q.Y;
		if((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
		{
			Temp=P.Z-Q.Z;
			if((Temp > -THRESH_POINTS_ARE_SAME) && (Temp < THRESH_POINTS_ARE_SAME))
			{
				return true;
			}
		}
	}
	return false;
}


inline double DVector3::PointPlaneDist
(
	const DVector3 &Point,
	const DVector3 &PlaneBase,
	const DVector3 &PlaneNormal
)
{
	return (Point - PlaneBase) | PlaneNormal;
}


inline DVector3 DVector3::PointPlaneProject(const DVector3& Point, const DVector3& PlaneBase, const DVector3& PlaneNorm)
{
	//Find the distance of X from the plane
	//Add the distance back along the normal from the point
	return Point - DVector3::PointPlaneDist(Point,PlaneBase,PlaneNorm) * PlaneNorm;
}

inline DVector3 DVector3::VectorPlaneProject(const DVector3& V, const DVector3& PlaneNormal)
{
	return V - V.ProjectOnToNormal(PlaneNormal);
}


inline bool DVector3::Coincident(const DVector3& Normal1, const DVector3& Normal2, double ParallelCosineThreshold)
{
	const double NormalDot = Normal1 | Normal2;
	return NormalDot >= ParallelCosineThreshold;
}

inline bool DVector3::Coplanar(const DVector3 &Base1, const DVector3 &Normal1, const DVector3 &Base2, const DVector3 &Normal2, double ParallelCosineThreshold)
{
	if      (!DVector3::Parallel(Normal1,Normal2,ParallelCosineThreshold)) return false;
	else if (DVector3::PointPlaneDist (Base2,Base1,Normal1) > THRESH_POINT_ON_PLANE) return false;
	else return true;
}

inline double DVector3::Triple(const DVector3& X, const DVector3& Y, const DVector3& Z)
{
	return
	(	(X.X * (Y.Y * Z.Z - Y.Z * Z.Y))
	+	(X.Y * (Y.Z * Z.X - Y.X * Z.Z))
	+	(X.Z * (Y.X * Z.Y - Y.Y * Z.X)));
}

inline DVector3 DVector3::RadiansToDegrees(const DVector3& RadVector)
{
	return RadVector * (180.f / PI);
}

inline DVector3 DVector3::DegreesToRadians(const DVector3& DegVector)
{
	return DegVector * (PI / 180.f);
}

FORCEINLINE DVector3::DVector3()
{}

FORCEINLINE DVector3::DVector3(double InF)
	: X(InF), Y(InF), Z(InF)
{
	DiagnosticCheckNaN();
}

FORCEINLINE DVector3::DVector3(double InX, double InY, double InZ)
	: X(InX), Y(InY), Z(InZ)
{
	DiagnosticCheckNaN();
}

FORCEINLINE DVector3::DVector3(const FLinearColor& InColor)
	: X(InColor.R), Y(InColor.G), Z(InColor.B)
{
	DiagnosticCheckNaN();
}

FORCEINLINE DVector3::DVector3(FIntVector InVector)
	: X(InVector.X), Y(InVector.Y), Z(InVector.Z)
{
	DiagnosticCheckNaN();
}

FORCEINLINE DVector3::DVector3(FIntPoint A)
	: X(A.X), Y(A.Y), Z(0.f)
{
	DiagnosticCheckNaN();
}

FORCEINLINE DVector3::DVector3(EForceInit)
	: X(0.0f), Y(0.0f), Z(0.0f)
{
	DiagnosticCheckNaN();
}

#ifdef IMPLEMENT_ASSIGNMENT_OPERATOR_MANUALLY
FORCEINLINE DVector3& DVector3::operator=(const DVector3& Other)
{
	this->X = Other.X;
	this->Y = Other.Y;
	this->Z = Other.Z;

	DiagnosticCheckNaN();

	return *this;
}
#endif

FORCEINLINE DVector3 DVector3::operator^(const DVector3& V) const
{
	return DVector3
		(
		Y * V.Z - Z * V.Y,
		Z * V.X - X * V.Z,
		X * V.Y - Y * V.X
		);
}

FORCEINLINE DVector3 DVector3::CrossProduct(const DVector3& A, const DVector3& B)
{
	return A ^ B;
}

FORCEINLINE double DVector3::operator|(const DVector3& V) const
{
	return X*V.X + Y*V.Y + Z*V.Z;
}

FORCEINLINE double DVector3::DotProduct(const DVector3& A, const DVector3& B)
{
	return A | B;
}

FORCEINLINE DVector3 DVector3::operator+(const DVector3& V) const
{
	return DVector3(X + V.X, Y + V.Y, Z + V.Z);
}

FORCEINLINE DVector3 DVector3::operator-(const DVector3& V) const
{
	return DVector3(X - V.X, Y - V.Y, Z - V.Z);
}

FORCEINLINE DVector3 DVector3::operator-(double Bias) const
{
	return DVector3(X - Bias, Y - Bias, Z - Bias);
}

FORCEINLINE DVector3 DVector3::operator+(double Bias) const
{
	return DVector3(X + Bias, Y + Bias, Z + Bias);
}

FORCEINLINE DVector3 DVector3::operator*(double Scale) const
{
	return DVector3(X * Scale, Y * Scale, Z * Scale);
}

FORCEINLINE DVector3 DVector3::operator/(double Scale) const
{
	const double RScale = 1.f/Scale;
	return DVector3(X * RScale, Y * RScale, Z * RScale);
}

FORCEINLINE DVector3 DVector3::operator*(const DVector3& V) const
{
	return DVector3(X * V.X, Y * V.Y, Z * V.Z);
}

FORCEINLINE DVector3 DVector3::operator/(const DVector3& V) const
{
	return DVector3(X / V.X, Y / V.Y, Z / V.Z);
}

FORCEINLINE bool DVector3::operator==(const DVector3& V) const
{
	return X==V.X && Y==V.Y && Z==V.Z;
}

FORCEINLINE bool DVector3::operator!=(const DVector3& V) const
{
	return X!=V.X || Y!=V.Y || Z!=V.Z;
}


FORCEINLINE DVector3 DVector3::operator-() const
{
	return DVector3(-X, -Y, -Z);
}


FORCEINLINE DVector3 DVector3::operator+=(const DVector3& V)
{
	X += V.X; Y += V.Y; Z += V.Z;
	DiagnosticCheckNaN();
	return *this;
}

FORCEINLINE DVector3 DVector3::operator-=(const DVector3& V)
{
	X -= V.X; Y -= V.Y; Z -= V.Z;
	DiagnosticCheckNaN();
	return *this;
}

FORCEINLINE DVector3 DVector3::operator*=(double Scale)
{
	X *= Scale; Y *= Scale; Z *= Scale;
	DiagnosticCheckNaN();
	return *this;
}

FORCEINLINE DVector3 DVector3::operator/=(double V)
{
	const double RV = 1.f/V;
	X *= RV; Y *= RV; Z *= RV;
	DiagnosticCheckNaN();
	return *this;
}

FORCEINLINE DVector3 DVector3::operator*=(const DVector3& V)
{
	X *= V.X; Y *= V.Y; Z *= V.Z;
	DiagnosticCheckNaN();
	return *this;
}

FORCEINLINE DVector3 DVector3::operator/=(const DVector3& V)
{
	X /= V.X; Y /= V.Y; Z /= V.Z;
	DiagnosticCheckNaN();
	return *this;
}

FORCEINLINE double& DVector3::operator[](int32 Index)
{
	check(Index >= 0 && Index < 3);
	return (&X)[Index];
}

FORCEINLINE double DVector3::operator[](int32 Index)const
{
	check(Index >= 0 && Index < 3);
	return (&X)[Index];
}

FORCEINLINE void DVector3::Set(double InX, double InY, double InZ)
{
	X = InX;
	Y = InY;
	Z = InZ;
	DiagnosticCheckNaN();
}





FORCEINLINE double DVector3::SizeSquared() const
{
	return X*X + Y*Y + Z*Z;
}



FORCEINLINE double DVector3::SizeSquared2D() const 
{
	return X*X + Y*Y;
}


FORCEINLINE bool DVector3::IsZero() const
{
	return X==0.f && Y==0.f && Z==0.f;
}

FORCEINLINE void DVector3::ToDirectionAndLength(DVector3 &OutDir, double &OutLength) const
{
	OutLength = Size();
	if (OutLength > SMALL_NUMBER)
	{
		double OneOverLength = 1.0f/OutLength;
		OutDir = DVector3(X*OneOverLength, Y*OneOverLength,
			Z*OneOverLength);
	}
	else
	{
		OutDir = DVector3::ZeroVector;
	}
}


FORCEINLINE DVector3 DVector3::Projection() const
{
	const double RZ = 1.f/Z;
	return DVector3(X*RZ, Y*RZ, 1);
}

FORCEINLINE void DVector3::AddBounded(const DVector3& V, double Radius)
{
	*this = (*this + V).BoundToCube(Radius);
}

FORCEINLINE double& DVector3::Component(int32 Index)
{
	return (&X)[Index];
}

FORCEINLINE double DVector3::Component(int32 Index) const
{
	return (&X)[Index];
}

FORCEINLINE double DVector3::GetComponentForAxis(EAxis::Type Axis) const
{
	switch (Axis)
	{
	case EAxis::X:
		return X;
	case EAxis::Y:
		return Y;
	case EAxis::Z:
		return Z;
	default:
		return 0.f;
	}
}

FORCEINLINE void DVector3::SetComponentForAxis(EAxis::Type Axis, double Component)
{
	switch (Axis)
	{
	case EAxis::X:
		X = Component;
		break;
	case EAxis::Y:
		Y = Component;
		break;
	case EAxis::Z:
		Z = Component;
		break;
	}
}

FORCEINLINE DVector3 DVector3::Reciprocal() const
{
	DVector3 RecVector;
	if (X!=0.f)
	{
		RecVector.X = 1.f/X;
	}
	else 
	{
		RecVector.X = BIG_NUMBER;
	}
	if (Y!=0.f)
	{
		RecVector.Y = 1.f/Y;
	}
	else 
	{
		RecVector.Y = BIG_NUMBER;
	}
	if (Z!=0.f)
	{
		RecVector.Z = 1.f/Z;
	}
	else 
	{
		RecVector.Z = BIG_NUMBER;
	}

	return RecVector;
}




FORCEINLINE bool DVector3::IsUniform(double Tolerance) const
{
	return AllComponentsEqual(Tolerance);
}

FORCEINLINE DVector3 DVector3::MirrorByVector(const DVector3& MirrorNormal) const
{
	return *this - MirrorNormal * (2.f * (*this | MirrorNormal));
}


FORCEINLINE double DVector3::CosineAngle2D(DVector3 B) const
{
	DVector3 A(*this);
	A.Z = 0.0f;
	B.Z = 0.0f;
	A.Normalize();
	B.Normalize();
	return A | B;
}

FORCEINLINE DVector3 DVector3::ProjectOnTo(const DVector3& A) const 
{ 
	return (A * ((*this | A) / (A | A))); 
}

FORCEINLINE DVector3 DVector3::ProjectOnToNormal(const DVector3& Normal) const
{
	return (Normal * (*this | Normal));
}



FORCEINLINE FString DVector3::ToString() const
{
	return FString::Printf(TEXT("X=%3.3f Y=%3.3f Z=%3.3f"), X, Y, Z);
}

FORCEINLINE FText DVector3::ToText() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("X"), X);
	Args.Add(TEXT("Y"), Y);
	Args.Add(TEXT("Z"), Z);

	return FText::Format(NSLOCTEXT("Core", "Vector3", "X={X} Y={Y} Z={Z}"), Args);
}



/* FVector2D inline functions
 *****************************************************************************/


#if PLATFORM_VECTOR_CUBIC_INTERP_SSE
template<>
FORCEINLINE_DEBUGGABLE DVector3 FMath::CubicInterp(const DVector3& P0, const DVector3& T0, const DVector3& P1, const DVector3& T1, const double& A)
{
	static_assert(PLATFORM_ENABLE_VECTORINTRINSICS == 1, "Requires SSE intrinsics.");
	DVector3 res;

	const double A2 = A  * A;
	const double A3 = A2 * A;

	double s0 = (2 * A3) - (3 * A2) + 1;
	double s1 = A3 - (2 * A2) + A;
	double s2 = (A3 - A2);
	double s3 = (-2 * A3) + (3 * A2);

	VectorRegister v0 = VectorMultiply(VectorLoadFloat1(&s0), VectorLoadFloat3(&P0));
	VectorRegister v1 = VectorMultiply(VectorLoadFloat1(&s1), VectorLoadFloat3(&T0));
	VectorRegister v2 = VectorMultiply(VectorLoadFloat1(&s2), VectorLoadFloat3(&T1));
	VectorRegister v3 = VectorMultiply(VectorLoadFloat1(&s3), VectorLoadFloat3(&P1));

	VectorStoreFloat3(VectorAdd(VectorAdd(v0, v1), VectorAdd(v2, v3)), &res);

	return res;
}
#endif
