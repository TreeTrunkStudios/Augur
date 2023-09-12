// TreeTrunkStudios (c) 2023


// 
#include "BaseAnimInstance.h"
//#include "DelayAction.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimNode_StateMachine.h"


// 
void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {

	// Call the parent function first
	Super::NativeUpdateAnimation(DeltaSeconds);

#ifdef UE_BUILD_DEBUG
	// If delta seconds is invalid or the owning actor does not implement UMovementAnimationData, then simply return
	if (DeltaSeconds <= 0.0f || !this->GetOwningActor()->Implements<UMovementAnimationData>() || !Cast<IMovementAnimationData>(this->GetOwningActor())) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Black, FString(TEXT("This UBaseAnimInstance does not have a valid DeltaSeconds value or does not implement UMovementAnimationData!!")));
		return;
	}
#endif

	//// Update Character Information
	// Collect the movement animation data from the owning actor now and store it all in MovementAnimationData
	IMovementAnimationData::Execute_GetMovementAnimationData(this->GetOwningActor(), MovementAnimationData);

	// 
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Black, MovementAnimationData.ToString());
}


// 
void UBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds) {

	// 
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	// 
	DynamicMontageGateTimer -= DeltaSeconds;
	if (DynamicMontageGateTimer <= 0.0f) {
		IsDynamicMontageGateOpen = true;
	}

	// 
	PivotTimer -= DeltaSeconds;
	if (PivotTimer <= 0.0f) {
		Pivot = false;
	}

	// 
	JumpedTimer -= DeltaSeconds;
	if (JumpedTimer <= 0.0f) {
		Jumped = false;
	}

	//// Update Aiming Values
	// 
	SmoothedAimingRotation = FMath::RInterpTo(SmoothedAimingRotation, MovementAnimationData.AimingRotation, DeltaSeconds, 10.0f);

	// 
	FRotator LocalStore = (MovementAnimationData.AimingRotation - MovementAnimationData.ActorControlRotation);
	LocalStore.Normalize();
	AimingAngle.X = LocalStore.Yaw;
	AimingAngle.Y = LocalStore.Pitch;

	// 
	LocalStore = (SmoothedAimingRotation - MovementAnimationData.ActorControlRotation);
	LocalStore.Normalize();
	SmoothedAimingAngle.X = LocalStore.Yaw;
	SmoothedAimingAngle.Y = LocalStore.Pitch;

	// 
	if (MovementAnimationData.RotationMode != ERotationState::VelocityDirection) {

		// 
		AimSweepTime = FMath::GetMappedRangeValueClamped(FVector2D(-90.0f, 90.0f), FVector2D(1.0f, 0.0f), AimingAngle.Y);

		// 
		//SpineRotation = FRotator(0.0f, AimingAngle.X * 0.25f, 0.0f);
	}

	// 
	else if (MovementAnimationData.HasMovementInput) {

		// 
		InputYawOffsetTime = FMath::FInterpTo(InputYawOffsetTime, FMath::GetMappedRangeValueClamped(FVector2D(-180.0f, 180.0f), FVector2D(0.0f, 1.0f), (MovementAnimationData.MovementInput.ToOrientationRotator() - MovementAnimationData.ActorControlRotation).GetNormalized().Yaw), DeltaSeconds, 8.0f);
	}


	//// Update Foot IK
	// 
	switch (MovementAnimationData.MovementState) {

		// 
		case EMovementState::Grounded:

			//// Handle left foot locking and IK
			if (GetCurveValue(FName(TEXT("Enable_FootIK_L"))) > 0.0f) {

				// 
				const float FootLockCurveValue = GetCurveValue(FName(TEXT("FootLock_L")));

				// 
				if (FootLockCurveValue >= 0.99f || FootLockCurveValue < LeftFootLockAlpha) {
					LeftFootLockAlpha = FootLockCurveValue;
				}

				// 
				if (LeftFootLockAlpha >= 0.99f) {

					// 
					const FTransform CurrentBoneTransform = this->GetSkelMeshComponent()->GetSocketTransform(FName(TEXT("ik_foot_l")), RTS_Component);

					// 
					LeftFootLockLocation = CurrentBoneTransform.GetLocation();
					LeftFootLockRotation = CurrentBoneTransform.GetRotation().Rotator();
				}

				// 
				if (LeftFootLockAlpha > 0.0f) {

					// 
					FRotator RotationDifference = FRotator::ZeroRotator;

					// 
					if (MovementAnimationData.IsMoving) {
						RotationDifference = (MovementAnimationData.ActorControlRotation - MovementAnimationData.ActorPreviousRotation);
						RotationDifference.Normalize();
					}

					// 
					LeftFootLockLocation = (LeftFootLockLocation -
						MovementAnimationData.ActorControlRotation.UnrotateVector(
							MovementAnimationData.Velocity * DeltaSeconds)).RotateAngleAxis(
								RotationDifference.Yaw, FVector(0.0f, 0.0f, -1.0f));

					// 
					LeftFootLockRotation -= RotationDifference;
					LeftFootLockRotation.Normalize();
				}

				// 
				FVector CurrentFootFloorLocationIK = GetSkelMeshComponent()->GetSocketLocation(FName(TEXT("ik_foot_l")));
				CurrentFootFloorLocationIK.Z = (GetSkelMeshComponent()->GetSocketLocation(FName(TEXT("root"))).Z + 50.0f);

				// 
				const FCollisionQueryParams CollisionQueryParams{NAME_None, true, GetOwningActor()};

				// 
				if (FPhysicsInterface::RaycastSingle(GetWorld(), CollisionTestResults, CurrentFootFloorLocationIK, CurrentFootFloorLocationIK - FVector(0.0f, 0.0f, 90.0f), ECollisionChannel::ECC_Visibility, CollisionQueryParams, FCollisionResponseParams::DefaultResponseParam)) {

					// 
					const FVector CurrentFootOffsetTarget = ((CollisionTestResults.ImpactPoint + (CollisionTestResults.ImpactNormal * 13.5f)) - (CurrentFootFloorLocationIK - FVector(0.0f, 0.0f, (50.0f - 13.5f))));

					// 
					LeftFootOffsetLocation = FMath::VInterpTo(LeftFootOffsetLocation, CurrentFootOffsetTarget, DeltaSeconds, ((LeftFootOffsetLocation.Z > CurrentFootOffsetTarget.Z) ? 30.0f : 15.0f));

					// 
					LeftFootOffsetRotation = FMath::RInterpTo(LeftFootOffsetRotation, FRotator((-180.0) / UE_DOUBLE_PI * FMath::Atan2(CollisionTestResults.ImpactNormal.X, CollisionTestResults.ImpactNormal.Z), 0.0f, (180.0) / UE_DOUBLE_PI * FMath::Atan2(CollisionTestResults.ImpactNormal.Y, CollisionTestResults.ImpactNormal.Z)), DeltaSeconds, 30.0f);
				}

				// Else, reset the offset information back to zeros
				else {
					LeftFootOffsetLocation = FVector::ZeroVector;
					LeftFootOffsetRotation = FRotator::ZeroRotator;
				}
			}

			// Else, reset the offset information back to zeros
			else {
				LeftFootOffsetLocation = FVector::ZeroVector;
				LeftFootOffsetRotation = FRotator::ZeroRotator;
			}

			//// Handle right foot locking and IK
			if (GetCurveValue(FName(TEXT("Enable_FootIK_R"))) > 0.0f) {

				// 
				const float FootLockCurveValue = GetCurveValue(FName(TEXT("FootLock_R")));

				// 
				if (FootLockCurveValue >= 0.99f || FootLockCurveValue < RightFootLockAlpha) {
					RightFootLockAlpha = FootLockCurveValue;
				}

				// 
				if (RightFootLockAlpha >= 0.99f) {

					// 
					const FTransform CurrentBoneTransform = this->GetSkelMeshComponent()->GetSocketTransform(FName(TEXT("ik_foot_r")), RTS_Component);

					// 
					RightFootLockLocation = CurrentBoneTransform.GetLocation();
					RightFootLockRotation = CurrentBoneTransform.GetRotation().Rotator();
				}

				// 
				if (RightFootLockAlpha > 0.0f) {

					// 
					FRotator RotationDifference = FRotator::ZeroRotator;

					// 
					if (MovementAnimationData.IsMoving) {
						RotationDifference = (MovementAnimationData.ActorControlRotation - MovementAnimationData.ActorPreviousRotation);
						RotationDifference.Normalize();
					}

					// 
					RightFootLockLocation = (RightFootLockLocation -
						MovementAnimationData.ActorControlRotation.UnrotateVector(
							MovementAnimationData.Velocity * DeltaSeconds)).RotateAngleAxis(
								RotationDifference.Yaw, FVector(0.0f, 0.0f, -1.0f));

					// 
					RightFootLockRotation -= RotationDifference;
					RightFootLockRotation.Normalize();
				}

				// 
				FVector CurrentFootFloorLocationIK = GetSkelMeshComponent()->GetSocketLocation(FName(TEXT("ik_foot_r")));
				CurrentFootFloorLocationIK.Z = (GetSkelMeshComponent()->GetSocketLocation(FName(TEXT("root"))).Z + 50.0f);

				// 
				const FCollisionQueryParams CollisionQueryParams{NAME_None, true, GetOwningActor()};

				// 
				if (FPhysicsInterface::RaycastSingle(GetWorld(), CollisionTestResults, CurrentFootFloorLocationIK, CurrentFootFloorLocationIK - FVector(0.0f, 0.0f, 90.0f), ECollisionChannel::ECC_Visibility, CollisionQueryParams, FCollisionResponseParams::DefaultResponseParam)) {

					// 
					const FVector CurrentFootOffsetTarget = ((CollisionTestResults.ImpactPoint + (CollisionTestResults.ImpactNormal * 13.5f)) - (CurrentFootFloorLocationIK - FVector(0.0f, 0.0f, (50.0f - 13.5f))));

					// 
					RightFootOffsetLocation = FMath::VInterpTo(RightFootOffsetLocation, CurrentFootOffsetTarget, DeltaSeconds, ((RightFootOffsetLocation.Z > CurrentFootOffsetTarget.Z) ? 30.0f : 15.0f));

					// 
					RightFootOffsetRotation = FMath::RInterpTo(RightFootOffsetRotation, FRotator((-180.0) / UE_DOUBLE_PI * FMath::Atan2(CollisionTestResults.ImpactNormal.X, CollisionTestResults.ImpactNormal.Z), 0.0f, (180.0) / UE_DOUBLE_PI * FMath::Atan2(CollisionTestResults.ImpactNormal.Y, CollisionTestResults.ImpactNormal.Z)), DeltaSeconds, 30.0f);
				}

				// Else, reset the offset information back to zeros
				else {
					RightFootOffsetLocation = FVector::ZeroVector;
					RightFootOffsetRotation = FRotator::ZeroRotator;
				}
			}

			// Else, reset the offset information back to zeros
			else {
				RightFootOffsetLocation = FVector::ZeroVector;
				RightFootOffsetRotation = FRotator::ZeroRotator;
			}

			//// Set Pelvis IK Offset
			// 
			PelvisAlpha = ((GetCurveValue(FName(TEXT("Enable_FootIK_L"))) + GetCurveValue(FName(TEXT("Enable_FootIK_R")))) * 0.5f);

			// 
			if (PelvisAlpha > 0.0f) {

				// 
				const FVector PelvisTarget = ((LeftFootOffsetLocation.Z < RightFootOffsetLocation.Z) ? LeftFootOffsetLocation : RightFootOffsetLocation);

				// 
				PelvisOffset = FMath::VInterpTo(PelvisOffset, PelvisTarget, DeltaSeconds, (PelvisTarget.Z > PelvisOffset.Z) ? 10.0f : 15.0f);
			}

			// 
			else {
				PelvisOffset = FVector::ZeroVector;
			}

			// 
			break;

		// 
		case EMovementState::Airborne:

			//// Set Pelvis IK Offset
			// 
			PelvisAlpha = ((GetCurveValue(FName(TEXT("Enable_FootIK_L"))) + GetCurveValue(FName(TEXT("Enable_FootIK_R")))) * 0.5f);

			// 
			if (PelvisAlpha > 0.0f) {
				PelvisOffset = FMath::VInterpTo(PelvisOffset, FVector::ZeroVector, DeltaSeconds, (0.0f > PelvisOffset.Z) ? 10.0f : 15.0f);
			}

			// 
			else {
				PelvisOffset = FVector::ZeroVector;
			}

			//// Reset IK Offsets
			LeftFootOffsetLocation = FMath::VInterpTo(LeftFootOffsetLocation, FVector::ZeroVector, DeltaSeconds, 15.0f);
			RightFootOffsetLocation = FMath::VInterpTo(RightFootOffsetLocation, FVector::ZeroVector, DeltaSeconds, 15.0f);
			LeftFootOffsetRotation = FMath::RInterpTo(LeftFootOffsetRotation, FRotator::ZeroRotator, DeltaSeconds, 15.0f);
			RightFootOffsetRotation = FMath::RInterpTo(RightFootOffsetRotation, FRotator::ZeroRotator, DeltaSeconds, 15.0f);

			// 
			break;

		// 
		default:
			break;
	}


	//// Movement Mode Specific Functionality
	switch (MovementAnimationData.MovementState) {

		// Do this functionality only while grounded
		case EMovementState::Grounded:

			// Calculate whether we should be moving or not
			bool NewShouldMove;
			NewShouldMove = ((MovementAnimationData.IsMoving && MovementAnimationData.HasMovementInput) || (MovementAnimationData.Speed > 150.0f));

			// If we are changing should move states, then...
			if (ShouldMove != NewShouldMove) {

				// Set the current state to the new state
				ShouldMove = NewShouldMove;

				// If we have changed to be true, then...
				if (ShouldMove) {

					// Reset the rotation values
					ElapsedDelayTime = 0.0f;
					//RotateLeft = false;
					//RotateRight = false;
				}
			}

			// Else, if we are staying as a true state, then...
			else if (ShouldMove) {

				//// Do while moving
				//// Update Movement Values
				// 
				const FVector LocalRelativeVelocityDirection = MovementAnimationData.ActorControlRotation.UnrotateVector(MovementAnimationData.Velocity.GetSafeNormal(0.1f));

				// 
				const FVector LocalRelativeDirection = (LocalRelativeVelocityDirection * (1.0f / (FMath::Abs(LocalRelativeVelocityDirection.X) + FMath::Abs(LocalRelativeVelocityDirection.Y) + FMath::Abs(LocalRelativeVelocityDirection.Z))));

				// 
				VelocityBlend.X = FMath::FInterpTo(VelocityBlend.X, FMath::Clamp(LocalRelativeDirection.X, 0.0f, 1.0f), DeltaSeconds, 12.0f);
				VelocityBlend.Y = FMath::FInterpTo(VelocityBlend.Y, FMath::Abs(FMath::Clamp(LocalRelativeDirection.X, -1.0f, 0.0f)), DeltaSeconds, 12.0f);
				VelocityBlend.Z = FMath::FInterpTo(VelocityBlend.Z, FMath::Abs(FMath::Clamp(LocalRelativeDirection.Y, -1.0f, 0.0f)), DeltaSeconds, 12.0f);
				VelocityBlend.W = FMath::FInterpTo(VelocityBlend.W, FMath::Clamp(LocalRelativeDirection.Y, 0.0f, 1.0f), DeltaSeconds, 12.0f);

				// 
				DiagonalScaleAmount = DiagonalScaleAmountCurve->GetFloatValue(FMath::Abs(VelocityBlend.X + VelocityBlend.Y));

				// 
				RelativeAccelerationAmount = MovementAnimationData.ActorControlRotation.UnrotateVector(MovementAnimationData.Acceleration.GetClampedToMaxSize(MovementAnimationData.MaxAcceleration) * (1.0f / MovementAnimationData.MaxAcceleration));

				// 
				LeanAmount = FMath::Vector2DInterpTo(LeanAmount, FVector2D(RelativeAccelerationAmount.Y, RelativeAccelerationAmount.X), DeltaSeconds, 4.0f);

				// 
				WalkRunBlend = ((MovementAnimationData.Gait != EGaitState::Walking) ? 1.0f : 0.0f);
				StrideBlend = FMath::Lerp(StrideBlendNormalWalkCurve->GetFloatValue(MovementAnimationData.Speed), StrideBlendNormalRunCurve->GetFloatValue(MovementAnimationData.Speed), FMath::Clamp(GetCurveValue(FName(TEXT("Weight_Gait"))) - 1.0f, 0.0f, 1.0f));
				StrideBlend = FMath::Lerp(StrideBlend, StrideBlendCrouchedWalkCurve->GetFloatValue(MovementAnimationData.Speed), GetCurveValue(FName(TEXT("BasePose_CLF"))));

				// 
				StandingPlayRate = FMath::Lerp(MovementAnimationData.Speed * (1.0f / 150.0f), MovementAnimationData.Speed * (1.0f / 350.0f), FMath::Clamp(GetCurveValue(FName(TEXT("Weight_Gait"))) - 1.0f, 0.0f, 1.0f));
				StandingPlayRate = FMath::Lerp(StandingPlayRate, MovementAnimationData.Speed * (1.0f / 600.0f), FMath::Clamp(GetCurveValue(FName(TEXT("Weight_Gait"))) - 2.0f, 0.0f, 1.0f));
				StandingPlayRate = FMath::Clamp((StandingPlayRate / StrideBlend) / GetSkelMeshComponent()->GetComponentScale().Z, 0.0f, 3.0f);

				// 
				CrouchingPlayRate = FMath::Clamp(((MovementAnimationData.Speed * (1.0f / 150.0f)) / StrideBlend) / GetSkelMeshComponent()->GetComponentScale().Z, 0.0f, 2.0f);


				//// Update Rotation Values
				// 
				if (MovementAnimationData.Gait == EGaitState::Running || MovementAnimationData.RotationMode == ERotationState::VelocityDirection) {
					MovementDirection = EMovementDirection::Forward;
				}

				// 
				else {

					// 
					static const float Buffer = 5.0f;
					static const float FRThreshold = 70.0f;
					static const float FLThreshold = -70.0f;
					static const float BRThreshold = 110.0f;
					static const float BLThreshold = -110.0f;

					// 
					const float Angle = (MovementAnimationData.Velocity.ToOrientationRotator() - MovementAnimationData.AimingRotation).GetNormalized().Yaw;

					// 
					if ((MovementDirection != EMovementDirection::Forward || MovementDirection != EMovementDirection::Backward) ?
						((Angle >= (FLThreshold - Buffer)) && (Angle <= (FRThreshold + Buffer))) :
						((Angle >= (FLThreshold + Buffer)) && (Angle <= (FRThreshold - Buffer)))) {
						MovementDirection = EMovementDirection::Forward;
					}

					// 
					else if ((MovementDirection != EMovementDirection::Right || MovementDirection != EMovementDirection::Left) ?
						((Angle >= (FRThreshold - Buffer)) && (Angle <= (BRThreshold + Buffer))) :
						((Angle >= (FRThreshold + Buffer)) && (Angle <= (BRThreshold - Buffer)))) {
						MovementDirection = EMovementDirection::Right;
					}

					// 
					else if ((MovementDirection != EMovementDirection::Right || MovementDirection != EMovementDirection::Left) ?
						((Angle >= (BLThreshold - Buffer)) && (Angle <= (FLThreshold + Buffer))) :
						((Angle >= (BLThreshold + Buffer)) && (Angle <= (FLThreshold - Buffer)))) {
						MovementDirection = EMovementDirection::Left;
					}

					// 
					else {
						MovementDirection = EMovementDirection::Backward;
					}
				}


				// 
				const float LocalInTime = (MovementAnimationData.Velocity.ToOrientationRotator() - MovementAnimationData.ActorControlRotation).GetNormalized().Yaw;

				// 
				FVector LocalYaw = YawOffsetFrontBack->GetVectorValue(LocalInTime);
				Yaw.X = LocalYaw.X;
				Yaw.Y = LocalYaw.Y;

				// 
				LocalYaw = YawOffsetLeftRight->GetVectorValue(LocalInTime);
				Yaw.Z = LocalYaw.X;
				Yaw.W = LocalYaw.Y;
			}

			// Else, we are staying as a false state, so...
			else {

				//// Do while NOT moving
				//// Can Rotate In Place
				if (MovementAnimationData.ViewMode == EViewMode::FirstPerson || MovementAnimationData.RotationMode == ERotationState::Aiming) {

					//// Rotate In Place Check
					// 
					//RotateLeft = (AimingAngle.X < -50.0f);
					//RotateRight = (AimingAngle.X > 50.0f);

					// 
					//if (RotateLeft || RotateRight) {
					//	RotateRate = FMath::GetMappedRangeValueClamped(FVector2D(90.0f, 270.0f), FVector2D(1.15f, 3.0f), MovementAnimationData.AimYawRate);
					//}
				}

				// 
				else {
					//RotateLeft = false;
					//RotateRight = false;
				}

				//// Can Turn In Place
				if (MovementAnimationData.ViewMode == EViewMode::ThirdPerson &&
					MovementAnimationData.RotationMode == ERotationState::LookingDirection &&
					this->GetCurveValue(FName(TEXT("Enable_Transition"))) > 0.99f) {

					//// Turn In Place Check
					// 
					if ((FMath::Abs(AimingAngle.X) > 45.0f) && (MovementAnimationData.AimYawRate < 50.0f)) {

						// 
						ElapsedDelayTime += DeltaSeconds;

						// 
						if (ElapsedDelayTime > FMath::GetMappedRangeValueClamped(FVector2D(45.0f, 180.0f), FVector2D(0.75f, 0.0f), FMath::Abs(AimingAngle.X))) {

							//
							this->TurnInPlace(FRotator(0.0f, MovementAnimationData.AimingRotation.Yaw, 0.0f), 1.0f, 0.0f, false);
						}
					}

					// 
					else {
						ElapsedDelayTime = 0.0f;
					}
				}

				// 
				else {
					ElapsedDelayTime = 0.0f;
				}

				//// Can Dynamic Transition
				if (FMath::IsNearlyEqualByULP(this->GetCurveValue(FName(TEXT("Enable_Transition"))), 1.0f)) {

					//// Dynamic Transition Check
					// Check dynamic transition for the left foot
					if ((GetSkelMeshComponent()->GetSocketTransform(FName(TEXT("VB foot_target_l")), RTS_Component).GetLocation() -
						GetSkelMeshComponent()->GetSocketTransform(FName(TEXT("ik_foot_l")), RTS_Component).GetLocation()).SizeSquared() > 64.0f) {

						//// Play Dynamic Transition
						this->PlayDynamicTransition(0.1f, NormalTransitionRightDynamicAnim, 0.2f, 0.2f, 1.5f, 0.8f);
					}

					//// Dynamic Transition Check
					// Check dynamic transition for the right foot
					if ((GetSkelMeshComponent()->GetSocketTransform(FName(TEXT("VB foot_target_r")), RTS_Component).GetLocation() -
						GetSkelMeshComponent()->GetSocketTransform(FName(TEXT("ik_foot_r")), RTS_Component).GetLocation()).SizeSquared() > 64.0f) {

						//// Play Dynamic Transition
						this->PlayDynamicTransition(0.1f, NormalTransitionLeftDynamicAnim, 0.2f, 0.2f, 1.5f, 0.8f);
					}
				}
			}

			// 
			break;

		// Do this functionality only while airborne
		case EMovementState::Airborne:

			//// Update Airborne Values

			// If we have an airborne landing prediction time, then we lerp to that, else we set it to 0.0f, as we have no prediction
			LandPrediction = ((MovementAnimationData.InAirPredictionTime >= 0.0f) ? FMath::Lerp(LandPredictionCurve->GetFloatValue(MovementAnimationData.InAirPredictionTime), 0.0f, GetCurveValue(FName(TEXT("Mask_LandPrediction")))) : 0.0f);

			// If we are falling at a large negative speed, and we get a hit for the land prediction shape trace, then we calculate our land prediction float
			/*if (MovementAnimationData.InAirPredictionTime >= 0.0f) {
				LandPrediction = FMath::Lerp(LandPredictionCurve->GetFloatValue(MovementAnimationData.InAirPredictionTime), 0.0f, GetCurveValue(FName(TEXT("Mask_LandPrediction"))));
			}

			// Else, we set the land prediction float to be zero, as we have no prediction whatsoever
			else {
				LandPrediction = 0.0f;
			}*/

			// 
			LeanAmount = FMath::Vector2DInterpTo(LeanAmount, FVector2D(MovementAnimationData.ActorControlRotation.Vector()) * LeanInAirCurve->GetFloatValue(MovementAnimationData.Velocity.Z), DeltaSeconds, 4.0f);

			// 
			break;

		// Do this functionality only while ragdolling
		case EMovementState::Ragdoll:

			//// Update Ragdoll Values
			// 
			//FlailRate = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1000.0f), FVector2D(0.0f, 1.0f), GetSkelMeshComponent()->GetPhysicsLinearVelocity(FName(TEXT("root"))).Size());
			//FlailRate = FMath::Clamp(GetSkelMeshComponent()->GetPhysicsLinearVelocity(FName(TEXT("root"))).Size() * (1.0f / 1000.0f), 0.0f, 1.0f);

			// 
			break;

		// Else, do nothing
		default:
			break;
	}



	//// Simplistic calculations to remove a bunch of the calculation overhead/burden from above into more simplistic thread-able formats
	// 
	if (this->IsGrounded() && (ShouldMove == false) && (MovementAnimationData.ViewMode == EViewMode::FirstPerson || MovementAnimationData.RotationMode == ERotationState::Aiming)) {

		// 
		RotateLeft = (AimingAngle.X < -50.0f);
		RotateRight = (AimingAngle.X > 50.0f);

		// 
		//if (RotateLeft || RotateRight) {
		//	RotateRate = FMath::GetMappedRangeValueClamped(FVector2D(90.0f, 270.0f), FVector2D(1.15f, 3.0f), MovementAnimationData.AimYawRate);
		//}
	}

	// 
	else {
		RotateLeft = false;
		RotateRight = false;
	}


	//// Finally, set all relevant state transition variables for quick pathing at the cost of a bit of memory
	// 
	const FAnimInstanceProxy & LocalAnimInstanceProxy = GetProxyOnAnyThread<FAnimInstanceProxy>();
	const FAnimNode_StateMachine * LocalStateMachine;
	int32 LocalStateMachineIndex;

	// Collect all necessary state weights from the target state machine of "Main Grounded States"
	LocalStateMachine = this->GetStateMachineInstance(this->GetStateMachineIndex(FName(TEXT("Main Grounded States"))));

	// 
	MainGroundedStateMachineWeight = LocalAnimInstanceProxy.GetRecordedMachineWeight(LocalStateMachine->StateMachineIndexInClass);

	// Collect all necessary state weights from the target state machine of "Look Towards Camera States"
	LocalStateMachine = this->GetStateMachineInstance(this->GetStateMachineIndex(FName(TEXT("Look Towards Camera States"))));

	// 
	LookTowardsCameraStatesLookingForwardsStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Looking Forwards"))));
	LookTowardsCameraStatesLookingLeftAndBackStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Looking Left and Back"))));
	LookTowardsCameraStatesLookingRightAndBackStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Looking Right and Back"))));

	// 
	LookTowardsCameraStatesCurrentStateTime = LocalStateMachine->GetCurrentStateElapsedTime();

	// Collect all necessary state weights from the target state machine of "Look Towards Camera States"
	LocalStateMachine = this->GetStateMachineInstance(this->GetStateMachineIndex(FName(TEXT("Standing Locomotion States"))));

	// 
	StandingLocomotionStatesMovingStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Moving"))));
	StandingLocomotionStatesStopStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Stop"))));

	// Collect all necessary state weights from the target state machine of "Look Towards Camera States"
	LocalStateMachineIndex = this->GetStateMachineIndex(FName(TEXT("Standing Locomotion Detail")));
	LocalStateMachine = this->GetStateMachineInstance(LocalStateMachineIndex);

	// 
	StandingLocomotionDetailMachineWeight = LocalAnimInstanceProxy.GetRecordedMachineWeight(LocalStateMachine->StateMachineIndexInClass);

	// 
	StandingLocomotionDetailRunStartStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Run Start"))));
	StandingLocomotionDetailWalkToRunStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Walk To Run"))));

	// 
	StandingLocomotionDetailCurrentStateTime = LocalStateMachine->GetCurrentStateElapsedTime();

	// 
	StandingLocomotionDetailFirstPivotRelevantAnimTimeRemaining = this->GetRelevantAnimTimeRemaining(LocalStateMachineIndex, LocalStateMachine->GetStateIndex(FName(TEXT("First Pivot"))));
	StandingLocomotionDetailSecondPivotRelevantAnimTimeRemaining = this->GetRelevantAnimTimeRemaining(LocalStateMachineIndex, LocalStateMachine->GetStateIndex(FName(TEXT("Second Pivot"))));
	StandingLocomotionDetailWalkToRunRelevantAnimTimeRemaining = this->GetRelevantAnimTimeRemaining(LocalStateMachineIndex, LocalStateMachine->GetStateIndex(FName(TEXT("Walk To Run"))));

	// Collect all necessary state weights from the target state machine of "Standing Directional States"
	LocalStateMachine = this->GetStateMachineInstance(this->GetStateMachineIndex(FName(TEXT("Standing Directional States"))));

	// 
	StandingDirectionalStatesMoveLeftBackwardStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Move LB"))));
	StandingDirectionalStatesMoveRightBackwardStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Move RB"))));

	// Collect all necessary state weights from the target state machine of "Crouching Locomotion States"
	LocalStateMachine = this->GetStateMachineInstance(this->GetStateMachineIndex(FName(TEXT("Crouching Locomotion States"))));

	// 
	CrouchingLocomotionStatesMovingStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Moving"))));
	CrouchingLocomotionStatesStopStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Stop"))));

	// Collect all necessary state weights from the target state machine of "Crouching Directional States"
	LocalStateMachine = this->GetStateMachineInstance(this->GetStateMachineIndex(FName(TEXT("Crouching Directional States"))));

	// 
	CrouchingDirectionalStatesMoveLeftForwardStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Move LF"))));
	CrouchingDirectionalStatesMoveRightBackwardStateWeight = LocalStateMachine->GetStateWeight(LocalStateMachine->GetStateIndex(FName(TEXT("Move RB"))));

	// Collect all necessary state weights from the target state machine of "Jump States"
	LocalStateMachineIndex = this->GetStateMachineIndex(FName(TEXT("Jump States")));
	LocalStateMachine = this->GetStateMachineInstance(LocalStateMachineIndex);

	// 
	JumpStatesJumpLeftFootRelevantAnimTimeRemaining = this->GetRelevantAnimTimeRemaining(LocalStateMachineIndex, LocalStateMachine->GetStateIndex(FName(TEXT("Jump Left Foot"))));
	JumpStatesJumpRightFootRelevantAnimTimeRemaining = this->GetRelevantAnimTimeRemaining(LocalStateMachineIndex, LocalStateMachine->GetStateIndex(FName(TEXT("Jump Right Foot"))));

	// Collect all necessary state weights from the target state machine of "Main Movement States"
	LocalStateMachineIndex = this->GetStateMachineIndex(FName(TEXT("Main Movement States")));
	LocalStateMachine = this->GetStateMachineInstance(LocalStateMachineIndex);

	// 
	MainMovementStatesLandRelevantAnimTimeRemaining = this->GetRelevantAnimTimeRemaining(LocalStateMachineIndex, LocalStateMachine->GetStateIndex(FName(TEXT("Land"))));
}


// 
void UBaseAnimInstance::PivotTriggered() {

	// 
	if (Pivot == false) {

		// 
		Pivot = (MovementAnimationData.Speed < 200.0f);

		// 
		if (Pivot == true) {
			PivotTimer = 0.1f;
		}
	}
}


// 
void UBaseAnimInstance::JumpTriggered() {

	// 
	if (Jumped == false) {

		// 
		Jumped = true;

		// 
		JumpPlayRate = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 600.0f), FVector2D(1.2f, 1.5f), MovementAnimationData.Speed);

		// 
		JumpedTimer = 0.1f;
	}
}


// 
void UBaseAnimInstance::PlayTransition(UAnimSequenceBase * Asset, const float BlendInTime, const float BlendOutTime, const float PlayRate, const float StartTime) {
	this->PlaySlotAnimationAsDynamicMontage(Asset, FName(TEXT("Grounded Slot")), BlendInTime, BlendOutTime, PlayRate, 1, -1.0f, StartTime);
}


// 
void UBaseAnimInstance::TurnInPlace(const FRotator TargetRotation, const float PlayRateScale, const float StartTime, const bool OverrideCurrent) {

	//// Step 1: Set Turn Angle
	// 
	const float TurnAngle = (TargetRotation - MovementAnimationData.ActorControlRotation).GetNormalized().Yaw;

	// 
	UAnimSequenceBase * Animation;
	float InvertedAnimatedAngle;

	// If we are standing, then this is true, else it is false
	const bool ScaleTurnAngle = (MovementAnimationData.Stance == EStance::Standing);

	// If true, we want the (N) variant, else we want the (CLF) variant
	const FName SlotName = FName(ScaleTurnAngle ? TEXT("(N) Turn/Rotate") : TEXT("(CLF) Turn/Rotate"));

	// Play rate is currently fixed at 1.2f, with the scale multiplied in now
	const float ScaledPlayRate = (1.2f * PlayRateScale);

	//// Step 2: Choose Turn Asset based on the Turn Angle and Stance
	// 
	if (FMath::Abs(TurnAngle) < 130.0f) {

		// 
		if (FMath::IsNegativeOrNegativeZero(TurnAngle)) {
			InvertedAnimatedAngle = (1.0f / -90.0f);
			Animation = (ScaleTurnAngle ? StandingNormalTurnLeft90 : CrouchingNormalTurnLeft90);
		}

		// 
		else {
			InvertedAnimatedAngle = (1.0f / 90.0f);
			Animation = (ScaleTurnAngle ? StandingNormalTurnRight90 : CrouchingNormalTurnRight90);
		}
	}

	// 
	else {

		// 
		if (FMath::IsNegativeOrNegativeZero(TurnAngle)) {
			InvertedAnimatedAngle = (1.0f / -180.0f);
			Animation = (ScaleTurnAngle ? StandingNormalTurnLeft180 : CrouchingNormalTurnLeft180);
		}

		// 
		else {
			InvertedAnimatedAngle = (1.0f / 180.0f);
			Animation = (ScaleTurnAngle ? StandingNormalTurnRight180 : CrouchingNormalTurnRight180);
		}
	}

	//// Step 3: If the Target Turn Animation is not playing or set to be overriden, play the turn animation as a dynamic montage
	// 
	if (OverrideCurrent || (this->IsPlayingSlotAnimation(Animation, SlotName)) == false) {

		// 
		this->PlaySlotAnimationAsDynamicMontage(Animation, SlotName, 0.2f, 0.2f, ScaledPlayRate, 1, -1.0f, StartTime);

		//// Step 4: Scale the rotation amount (gets scaled in animgraph) to compensate for turn angle (If Allowed) and play rate
		// 
		RotationScale = (ScaleTurnAngle ? (TurnAngle * InvertedAnimatedAngle * ScaledPlayRate) : ScaledPlayRate);
	}
}


// 
void UBaseAnimInstance::PlayDynamicTransition(const float RetriggerDelay, UAnimSequenceBase * Asset, const float BlendInTime, const float BlendOutTime, const float PlayRate, const float StartTime) {

	// 
	if (IsDynamicMontageGateOpen == true) {

		// 
		IsDynamicMontageGateOpen = false;

		// 
		DynamicMontageGateTimer = RetriggerDelay;

		// 
		this->PlaySlotAnimationAsDynamicMontage(Asset, FName(TEXT("Grounded Slot")), BlendInTime, BlendOutTime, PlayRate, 1, -1.0f, StartTime);
	}
}
